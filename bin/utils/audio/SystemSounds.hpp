/**
 * File: SystemSounds.hpp
 * Author: Antonius Torode
 * Created on: 03/09/2021
 * Description:
 *     This file provides static methods for emitting  basic system-level audio cues 
 *     such as beeps or alerts. This interface serves  as a lightweight abstraction 
 *     over platform-specific sound signaling, and is  intended for use in diagnostics, 
 *     notifications, or simple user feedback.
 */

/**
 * This class is for system sounds and methods related to such.
 */
class SystemSounds
{
public:

    /**
     * Main default constructor for the SystemSounds class.
     */
    SystemSounds() = default;

    /**
     * Main destructor for the SystemSounds class.
     */
    ~SystemSounds() = default;

    /**
     * Sends a beep sound.
     */
    static void beep();
};

