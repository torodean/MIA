/**
 * @file: KeyListenerTask.hpp
 * @author: Antonius Torode
 * @date: 06/25/2025
 * Description:
 *     TODO
 */
#pragma once

#include <limits>
#include "BackgroundTask.hpp"

#if defined(__linux__)
    #include <X11/Xlib.h>
#endif

/**
 * This class provides a threaded task which will listen for a key-press in order
 * to monitor a conditional state, which is toggled by the key-press.
 */
class KeyListenerTask : public threading::BackgroundTask
{
public:

    /**
     * @brief Default constructor for KeyListenerTask.
     *
     * Constructs an inactive listener with no key bound. Useful when the object is
     * conditionally configured later. The task will be inert unless a keyCode is set
     * and initialize is completed. If this constructor is used, the user must call
     * setKeyCode() then initialize().
     */
    KeyListenerTask();

    /**
     * @brief Constructs a KeyListenerTask with a specific key binding.
     *
     * Initializes the listener with the specified key code. When started, the task
     * will monitor for the key press and toggle its internal condition state.
     *
     * @param [unsigned int] - keyCode The key code to listen for (platform-specific).
     */
    KeyListenerTask(unsigned int keyCode);
    
    /**
     * @brief Destructor for KeyListenerTask.
     *
     * Cleans up any platform-specific resources used by the listener. On Linux,
     * this includes ungrabbing the key from the X server (if previously grabbed)
     * and closing the X display connection.
     */
    ~KeyListenerTask();
    
    /**
     * @brief Sets the key code to be monitored by the key listener.
     *
     * Updates the internal keyCode value, enabling the listener to monitor
     * the specified key for toggling behavior. If this method is needed, a call to
     * initialize() will most likely also be needed.
     *
     * @param code The key code to be assigned for listening.
     */
    void setKeyCode(unsigned int code)
    { keyCode = code; }
    
    /**
     * @brief Indicates whether the listener is active and bound to a valid key.
     *
     * @return true if a valid keyCode has been set (non-default); otherwise false.
     */
    bool isActive()
    { return keyCode != std::numeric_limits<unsigned int>::max(); }
    
    /**
     * @brief Initializes the key listener by registering the key grab with the system.
     *
     * This method sets up platform-specific key listening behavior. On Linux, it grabs the
     * specified key globally using XGrabKey and selects for key press events on the root window.
     * The method requires the listener to be in an active (constructed) state.
     * 
     * @note If the listener is not active (i.e., no key has been set), the method exits early.
     *       Intended exception handling for this case is marked as a TODO.
     */
    void initialize();
    
protected:
    /**
     * @brief Monitors the configured key for press events and toggles a condition when detected.
     *
     * This method is repeatedly invoked by the BackgroundTask base class. It checks whether the 
     * specified key has been pressed, and if so, toggles the internal condition flag. On Windows,
     * it uses GetAsyncKeyState to detect key state changes with edge detection. On Linux, it processes
     * X11 KeyPress events from the event queue if the key has been grabbed using XGrabKey.
     *
     * If the listener is not active (i.e. keyCode not set), the method returns early.
     *
     * @note This method assumes `initialize()` has been called on Linux to grab the key.
     */
    void run() override;
    
private:

    /// Storage for the key-code used by this listener.
    unsigned int keyCode{std::numeric_limits<unsigned int>::max()};
    
#if defined(IS_WINDOWS)

    bool lastPressed{false};
    
#elif defined(__linux__)       
    /**
     * @brief Pointer to the X11 display connection used for listening to input events.
     *
     * Initialized with XOpenDisplay(nullptr) to connect to the default display.
     * Used internally for issuing fake mouse events via the XTest extension.
     */
    Display *display;
      
    static constexpr unsigned int modifiers = ControlMask;    
    Window root;
    bool grabbed{false}; // To track if key was grabbed
#endif
}; // class KeyListenerTask
