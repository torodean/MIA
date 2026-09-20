/**
 * @file BackgroundTask.hpp
 * @author Antonius Torode
 * @date 06/25/2025
 *
 * Defines the BackgroundTask abstract base class, which provides a reusable interface
 * for running background tasks in a separate thread. It manages thread lifecycle, 
 * stop signaling via an atomic flag, and requires derived classes to implement the
 * task logic in the run() method. The class ensures proper cleanup and supports 
 * controlled stopping of long-running or repeating tasks.
 */
#pragma once

#include <thread>
#include <atomic>
#include <exception>

#include "RuntimeContext.hpp"

namespace threading
{
    /**
     * @brief Abstract base class for running background tasks in a separate thread.
     *
     * Derived classes should override the run() method to implement specific task behavior.
     * The task starts on a separate thread via start(), and can be stopped via stop().
     *
     * @note Exceptions thrown from the overridden run() method are captured
     *       and stored. After calling any method which may throw (in the worker thread),
     *       the caller should invoke hasFailed() and/or rethrowExceptionIfAny() to check 
     *       for and propagate any exception that occurred in the background thread back 
     *       to the calling thread.
     */
    class BackgroundTask 
    {
    public:
        /**
         * @brief Constructs the BackgroundTask object.
         */
        BackgroundTask() = default;

        /**
         * @brief Destructor ensures the thread is properly stopped and joined.
         */
        virtual ~BackgroundTask();
        
        /**
         * @brief Deleted copy and move constructors and assignment operators.
         *
         * Prevents copying or moving of BackgroundTask instances to ensure thread ownership 
         * and internal state (e.g., atomic flags and threads) remain safe and unambiguous.
         */
        BackgroundTask(const BackgroundTask&) = delete;
        BackgroundTask& operator=(const BackgroundTask&) = delete;
        BackgroundTask(BackgroundTask&&) = delete;
        BackgroundTask& operator=(BackgroundTask&&) = delete;

        /**
         * @brief Starts the background thread and invokes the threadLoop() method.
         */
        virtual void start();

        /**
         * @brief Requests the background task to stop.
         *
         * This sets the stopRequested flag. The threadLoop() method checks this flag.
         */
        virtual void stop();

        /**
         * @brief Checks whether the task is still running.
         * @return true if the task has not been requested to stop.
         */
        bool isRunning() const;

        /**
         * @brief Sets the condition met flag.
         * @param value The new value to set for conditionMet.
         */
        void setConditionMet(bool value);

        /**
         * @brief Checks whether the condition has been met.
         * @return true if the conditionMet flag is set; otherwise false.
         */
        bool isConditionMet() const;
        
        /**
         * @brief Toggles the conditionMet flag between true and false.
         * Useful for handling state changes triggered by events such as key presses.
         */
        void toggleCondition();
        
        /**
         * @brief Sets the given RuntimeContext into this instance's context pointer.
         * 
         * Allows this class to update its runtime variables by pointing to another
         * RuntimeContext instance.
         * 
         * @param ctx The RuntimeContext to point to.
         */
        void setContext(const RuntimeContext& ctx);
        
        /**
         * @brief Returns true if the background task has failed due to an uncaught exception.
         *
         * The flag becomes true as soon as an exception is captured in the worker thread.
         * Callers (game loops, framework tick functions, etc.) can poll this at any safe
         * point and then call rethrowExceptionIfAny() to propagate the error onto the
         * main thread if desired.
         *
         * @return true if an exception was stored from the worker thread.
         */
        bool hasFailed() const noexcept;
        
        /**
         * @brief Returns any exception that was thrown in the worker thread.
         *
         * The returned pointer is empty if no exception occurred. 
         *
         * @return std::exception_ptr holding the captured exception, or nullptr.
         */
        std::exception_ptr getException() const;

        /**
         * @brief Rethrows the exception that occurred in the worker thread, if any.
         *
         * If the background task has failed (see hasFailed()), this method rethrows
         * the original exception on the calling thread so it can be handled by normal
         * exception-handling logic (for example the try/catch in AppFramework's runApp).
         *
         * It is safe to call at any time. If no exception has been stored the method
         * does nothing. Typical usage is to poll hasFailed() from the main thread
         * (or framework loop) and then call this method at a safe point when you
         * want the exception to propagate.
         *
         * @throws The original exception that was thrown inside run(), if one exists.
         */
        void rethrowExceptionIfAny() const;

    protected:
        
        /// The runtime context object to enable application output.
        const RuntimeContext* context{nullptr};
        
        /**
         * @brief Task logic should be implemented here in derived classes.
         *
         * Should periodically check stopRequested to exit cleanly.
         */
        virtual void run() = 0;

        /**
         * @brief Optional hook for when the task exits.
         */
        virtual void doWhenStopped();

        /// Flag indicating if a stop has been requested.
        std::atomic<bool> stopRequested{false};
        
        /**
         * Flag indicating if some condition has been met. This can be used to toggle some
         * conditional state or value without having to request the thread to stop.
         */
        std::atomic<bool> conditionMet{false};
        
        /**
         * @brief Sets the taskName for this object.
         * @param newTaskName The name to set the taskName to. 
         */
        void setTaskName(const std::string& newTaskName);

    private:
        /// An optional name to mark this task - used in error and status messages.
        std::string taskName;
        
        /**
         * @brief Internal method that repeatedly invokes run() until stopped.
         */
        void threadLoop();

        /// The worker thread that executes the task.
        std::thread worker;
        
        /// Stores any exception that were thrown in the worker thread.
        std::exception_ptr storedException;
    }; // class BackgroundTask 
} // namespace threading
