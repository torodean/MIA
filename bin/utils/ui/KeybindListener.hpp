/**
 * @file KeybindListener.hpp
 * @author Antonius Torode
 * @date 09/15/2026
 * @brief Declares a background listener which watches keys and key combinations 
 *        and pushes events into a queue.
 */
#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

// Used for creating a threaded task for listening.
#include "BackgroundTask.hpp"
// Used for loading keybinds from a configuration file.
#include "MIAConfig.hpp"
// The event types passed between the listener and the reader.
#include "Event.hpp"
// The shared queue this listener pushes detected events into.
#include "EventQueue.hpp"
// Used for preprocessor definitions.
#include "Constants.hpp"

#if defined(IS_LINUX)
    #include <X11/Xlib.h>
#endif

namespace ui
{
    /**
     * The named modifiers a binding can require.
     * The values are single-bit flags so a combination of required modifiers
     * can be built by ORing them together.
     *
     * @note Each value must keep its own bit (the next free values are 8, 16,
     *     32, ...). A composite value such as 5 would alias the combination
     *     ALT | SHIFT and make bindings requiring it match those modifiers.
     */
    enum class Modifier : unsigned int
    {
        NONE = 0,  ///< The binding requires no modifier.
        ALT = 1,   ///< The alt modifier.
        CTRL = 2,  ///< The ctrl modifier.
        SHIFT = 4  ///< The shift modifier.
    };

    /**
     * The key combination of one binding: its characters plus any required
     * named modifiers.
     */
    struct Keybind
    {
        /// The non-modifier keys of the binding.
        std::set<char> keys;

        /*
         * The modifiers required by the binding, or Modifier::NONE. A single
         * Modifier value holds the whole set: each modifier is one bit of the
         * enum, so the required modifiers are ORed together (e.g. ALT | SHIFT
         * becomes 5) and a binding matches only when every required bit is
         * held. This is why no member like std::set<Modifier> is needed.
         */
        Modifier modifier{Modifier::NONE};

        /**
         * Orders keybinds so they can be used as map keys.
         *
         * @param other The keybind to compare against.
         * @return true if this keybind orders before the other.
         */
        bool operator<(const Keybind& other) const
        {
            if (modifier != other.modifier)
                return modifier < other.modifier;
            return keys < other.keys;
        }
    };

    /**
     * Maps a keybind to the event pushed into the queue when all of the
     * keybind's keys (and its modifier, if any) are held down at the same
     * time. A single-key binding is a set of one character.
     */
    using KeybindEventMap = std::map<Keybind, Event>;

    /**
     * A background task which watches keys and key combinations and pushes an
     * event into an EventQueue when a binding completes.
     *
     * Each binding is a set of characters: the event is pushed when every key
     * in the set is held at the same time, in any order. A single-key binding
     * therefore fires on that key's press. The event is pushed once per
     * completion; the listener does not push it again until one of the
     * binding's keys is released, which re-arms the binding. So holding a key
     * down does not repeat its event, and holding a combination down does not
     * repeat its event either.
     *
     * The queue is passed in and shared rather than owned so that other event
     * sources (e.g., a python_ui::PythonUIListener) can push into the same
     * queue, giving the reader a single place to wait. The queue must outlive
     * this listener.
     *
     * On Linux, detection comes from polling the physical keyboard state, so
     * presses are detected no matter which window has focus. Whether the
     * listener's keys are also captured depends on captureGlobally: when
     * true, each key is grabbed globally with XGrabKey, which both detects
     * presses and swallows them, so no other application receives the keys
     * while the listener runs; when false (the default), the keys are never
     * grabbed and pass through to the focused window normally. On Windows,
     * the keys are polled with GetAsyncKeyState and never captured.
     *
     * By default there is a small pause between polls, since key presses are
     * far slower than the poll loop; the pause keeps the idle listener from
     * spinning. A pollIntervalMs of zero polls continuously.
     *
     * A binding can also carry named modifiers ('alt', 'ctrl', 'shift')
     * joined by '+', e.g. "keybind_alt+t" or "keybind_ctrl+alt+del". A
     * modifier binding fires only when its modifiers and keys are held
     * together, so "alt+t" does not fire when t is pressed alone.
     *
     * Note that a binding whose keys overlap another binding's keys fires
     * when its own keys are all held, even if keys from the other binding are
     * held too. Bindings should use disjoint key sets to keep the produced
     * events unambiguous.
     */
    class KeybindListener : public threading::BackgroundTask
    {
    public:

        /**
         * @brief Constructs a KeybindListener from bindings read out of a
         *        configuration file.
         *
         * The configuration must be a KEY_VALUE config whose keys start with
         * keybindPrefix. Each matching pair binds the tokens after the prefix
         * to the event named by the value. A single character binds that key
         * alone, and tokens separated by '+' bind those keys as a combination,
         * e.g., the pair "keybind_A" = "increment" pushes an "increment" event
         * whenever the A key is pressed, and the pair "keybind_a+b" =
         * "increment" pushes it whenever the A and B keys are held together.
         * A token may also name a modifier ('alt', 'ctrl', or 'shift'), any
         * number of them, e.g. "keybind_alt+shift+t". Pairs which do not start
         * with the prefix, and pairs whose tokens are not characters or
         * modifier names, are skipped. If a verbose RuntimeContext is set,
         * each skipped pair is reported to standard error.
         *
         * @param config The configuration to read bindings from. The config
         *        must be initialized before it is passed in; this constructor
         *        only reads from it and does not load or reload the file.
         * @param queue The event queue this listener pushes detected events
         *        into. The referenced queue must outlive this listener.
         * @param keybindPrefix The prefix which marks a config pair as a
         *        keybind for this listener.
         * @param pollIntervalMs The time to sleep after each poll, in
         *        milliseconds. Zero polls continuously.
         * @param captureGlobally Whether to grab the keys with XGrabKey (see
         *        the class comment for what that means).
         * @throws error::MIAException If reading the config pairs fails (e.g.,
         *         the config is not a KEY_VALUE config).
         */
        KeybindListener(const config::MIAConfig& config,
                         ui::EventQueue& queue,
                         const std::string& keybindPrefix = "keybind_",
                         int pollIntervalMs = 5,
                         bool captureGlobally = false);

        /**
         * @brief Constructs a KeybindListener from a map of key sets and
         *        events.
         *
         * @param eventMappings A mapping of key sets to the events they push.
         * @param queue The event queue this listener pushes detected events
         *        into. The referenced queue must outlive this listener.
         * @param pollIntervalMs The time to sleep after each poll, in
         *        milliseconds. Zero polls continuously.
         * @param captureGlobally Whether to grab the keys with XGrabKey (see
         *        the class comment for what that means).
         */
        KeybindListener(const KeybindEventMap& eventMappings,
                         ui::EventQueue& queue,
                         int pollIntervalMs = 5,
                         bool captureGlobally = false);

        /**
         * @brief Destructor. Stops the listener, releases the key grabs, and
         *        joins the worker thread.
         */
        ~KeybindListener() override;

        /**
         * @brief Indicates whether this listener has any keys to watch.
         *
         * @return true if at least one binding is set; otherwise false.
         */
        bool isActive() const;

        /**
         * @brief Indicates whether a key is currently held down.
         *
         * Only keys which appear in a binding are tracked. On Linux this
         * reads the worker's last poll (at most pollIntervalMs old); on
         * Windows the key state is queried live.
         *
         * @param key The character to check.
         * @return true if the key is currently held; otherwise false.
         */
        bool isKeyHeld(char key) const;

        /**
         * @brief Indicates whether every key of a binding is currently held,
         *        including its modifier when it has one.
         *
         * This is the state query for movement-style input, e.g.,
         * isBindingHeld({'w'}) each frame to check forward movement, or
         * isBindingHeld({'a', 'd'}) for a chord.
         *
         * @param keys The set of characters to check.
         * @param modifier The modifier to also require. Modifier::NONE (the
         *        default) checks the keys only.
         * @return true if every key in the set (and the modifier) is currently
         *         held; otherwise false.
         */
        bool isBindingHeld(const std::set<char>& keys,
                           Modifier modifier = Modifier::NONE) const;

        /**
         * @brief Initializes the key listener by registering key grabs with
         *        the system.
         *
         * This method sets up platform-specific key listening behavior. On
         * Linux, it grabs each key used by any binding globally using XGrabKey
         * for all lock-key combinations and enables detectable auto-repeat.
         * Keys whose character has no corresponding X11 keycode are skipped,
         * along with any bindings which use them. On Windows, no registration
         * is needed since the keys are polled.
         */
        void initialize();

        /**
         * @brief Requests the listener to stop, closes the shared event
         *        queue, and joins the worker thread.
         *
         * Closing the queue wakes any thread blocked in waitForEvents() so it
         * can observe the shutdown, matching the python UI listener's stop
         * behavior.
         */
        void stop() override;

    protected:

        /**
         * @brief Performs one poll of key states and pushes an event for each
         *        newly completed binding into the shared queue.
         *
         * Called repeatedly by the BackgroundTask loop. Both platforms detect
         * completions as state transitions against the physical keyboard
         * state, so a lost event cannot leave a key stuck held or silently
         * re-fire a binding. On Linux the grabs still deliver events, which
         * the poll drains; on Windows the key states are polled directly.
         */
        void run() override;

    private:

        /**
         * @brief Gets the set of keys currently held, as of the last poll.
         *
         * @return The held keys, as characters.
         */
        std::set<char> getHeldKeys() const;

        /**
         * @brief Gets the modifiers currently held, as a Modifier bit mask.
         *
         * @return The held modifiers, or 0 when none are held.
         */
        unsigned int getHeldModifiers() const;

        /**
         * @brief Checks every binding against the held-key set and the held
         *        modifiers, and pushes events for bindings which just
         *        completed.
         *
         * @param heldKeys The keys currently held.
         * @param heldModifiers The modifiers currently held.
         * @param pressedEvents The vector to append completed binding events
         *        to.
         */
        void checkCompletions(const std::set<char>& heldKeys,
                              unsigned int heldModifiers,
                              std::vector<Event>& pressedEvents);

        /**
         * @brief Converts a single ASCII character to its corresponding X11
         *        keycode.
         *
         * @param c The character to convert (must be a single printable ASCII
         *        character).
         * @return The X11 keycode, or 0 if the character has no corresponding
         *         keycode.
         */
        unsigned int charToKeyCode(char c);

        /**
         * @brief Converts a modifier name to its Modifier value.
         *
         * @param token The token to convert, compared case-insensitively.
         * @return The Modifier for the token, or Modifier::NONE if the token
         *         is not a modifier name.
         */
        static Modifier modifierFromToken(const std::string& token);

        /// The keybind mappings this listener watches.
        KeybindEventMap keybinds;

        /// Every distinct character this listener watches, for held-key queries.
        std::set<char> watchedKeys;

        /// The bindings currently completed; a binding is re-armed when it stops being held.
        std::set<Keybind> completedBindings;

        /// The event queue shared with the reading thread. Owned by the caller.
        ui::EventQueue& eventQueue;

        /// The time to sleep after each poll. Zero polls continuously.
        int pollIntervalMs;

        /// Whether the bound keys are grabbed globally (Linux only).
        bool captureGlobally{false};

#if defined(IS_LINUX)
        /**
         * @brief Rebuilds the held-key and held-modifier state from the
         *        keyboard's physical state.
         *
         * Called each poll. This keeps the held state in sync with the real
         * keyboard even when a release event is lost, which otherwise leaves
         * a key stuck in the held state.
         */
        void reconcileHeldKeys();
        
        /// Maps each grabbed X11 keycode back to its character, filled by initialize().
        std::map<unsigned int, char> charByKeycode;

        /// The X11 keycodes currently held, rebuilt from the physical keyboard each poll.
        std::set<unsigned int> heldKeycodes;

        /// The modifiers currently held, rebuilt from the physical keyboard each poll.
        unsigned int heldModifiers{0};

        /// Maps each Modifier value to the X11 keycodes which produce it, filled by initialize().
        std::map<unsigned int, std::set<unsigned int>> modifierKeycodes;

        /// Guards the held-key state, which the reader thread queries while the worker updates it.
        mutable std::mutex heldKeysMutex;

        /// Pointer to the X11 display connection used for listening to input events.
        Display* display;

        Window root;
        bool grabbed{false};
#endif
    }; // class KeybindListener
} // namespace ui
