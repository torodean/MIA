/**
 * @file SystemSounds.hpp
 * @author Antonius Torode
 * @date 03/09/2021
 * @brief Provides static methods for emitting basic system-level audio cues
 *        such as beeps or alerts.
 *
 * This interface serves as a lightweight abstraction over platform-specific
 * sound signaling, and is intended for use in diagnostics, notifications, or
 * simple user feedback.
 */
#pragma once

#include <stdint.h>


namespace system_sounds
{
    /**
     * @brief Sends a beep sound.
     * @param frequency The frequency of the beep to use.
     * @param time The time to hold the beep sound.
     * @return true on success, false otherwise.
     * @throws MIAException when not on a supported OS.
     */
    bool beep(uint32_t frequency, uint32_t time);
} // namespace system_sounds
