/**
 * @file: BackgroundTask.hpp
 * @author: Antonius Torode
 * @date: 06/25/2025
 * Description:
 *     Defines the BackgroundTask abstract base class, which provides a reusable interface
 *     for running background tasks in a separate thread. It manages thread lifecycle, 
 *     stop signaling via an atomic flag, and requires derived classes to implement the
 *     task logic in the run() method. The class ensures proper cleanup and supports 
 *     controlled stopping of long-running or repeating tasks.
 */
#pragma once

#include <thread>
#include <atomic>
#include <iostream>

namespace threading
{
    /**
     * @brief Abstract base class for running background tasks in a separate thread.
     *
     * Derived classes should override the run() method to implement specific task behavior.
     * The task starts on a separate thread via start(), and can be stopped via stop().
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
        virtual ~BackgroundTask() 
        { stop(); }
        
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
        void start() 
        { worker = std::thread([this] { this->threadLoop(); }); }

        /**
         * @brief Requests the background task to stop.
         *
         * This sets the stopRequested flag. The threadLoop() method checks this flag.
         */
        void stop() 
        {     
            if (!stopRequested.exchange(true)) 
                if (worker.joinable())
                    worker.join();
        }

        /**
         * @brief Checks whether the task is still running.
         * @return [bool] - true if the task has not been requested to stop.
         */
        bool isRunning() const 
        { return !stopRequested.load(); }
        
        
        /**
         * @brief Sets the condition met flag.
         * @param value[bool] - The new value to set for conditionMet.
         */
        void setConditionMet(bool value) 
        {  conditionMet.store(value); }

        /**
         * @brief Checks whether the condition has been met.
         * @return [bool] - true if the conditionMet flag is set; otherwise false.
         */
        bool isConditionMet() const 
        { return conditionMet.load(); }
        
        /**
         * @brief Toggles the conditionMet flag between true and false.
         * Useful for handling state changes triggered by events such as key presses.
         */
        void toggleCondition()
        { setConditionMet(!isConditionMet()); std::cout << "TOGGLE CONDITION: " << (isConditionMet() ? "ON" : "OFF") << std::endl; }

    protected:
        /**
         * @brief Task logic should be implemented here in derived classes.
         *
         * Should periodically check stopRequested to exit cleanly.
         */
        virtual void run() = 0;

        /**
         * @brief Optional hook for when the task exits.
         */
        virtual void doWhenStopped() {}

        /// Flag indicating if a stop has been requested.
        std::atomic<bool> stopRequested{false};
        
        /**
         * Flag indicating if some condition has been met. This can be used to toggle some
         * conditional state or value without having to request the thread to stop.
         */
        std::atomic<bool> conditionMet{false};

    private:
        /**
         * @brief Internal method that repeatedly invokes run() until stopped.
         */
        void threadLoop() 
        { 
            while (!stopRequested.load()) 
                run();
            doWhenStopped();
        }

        /// The worker thread that executes the task.
        std::thread worker;
    }; // class BackgroundTask 
} // namespace threading
