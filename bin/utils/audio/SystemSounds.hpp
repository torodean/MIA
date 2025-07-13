/**
 * @file SystemSounds.hpp
 * @author Antonius Torode
 * @date 03/09/2021
 * Description:
 *     This file provides static methods for emitting basic system-level audio cues 
 *     such as beeps or alerts. This interface serves as a lightweight abstraction 
 *     over platform-specific sound signaling, and is intended for use in diagnostics, 
 *     notifications, or simple user feedback.
 */
#pragma once

namespace SystemSounds
{
    /**
     * Sends a beep sound.
     * @throw [MIAException] - Throws an exception when not on a supported OS.
     */
    void beep();
} // namespace SystemSounds
