/**
 * @file SoundsFromFile.hpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief An interface for playing sounds from files.
 */
#pragma once

#include <string>
#include <cstdint>

namespace audio
{
	
	
	/**
	 * Attempts to play a sound from a file.
	 * @note This is a blocking call and will block the thread until
	 *       the audio file is finished playing.
	 * @param fileName The file to try playing a sound from. This
	 *        must be a valid audio file of a supported type.
	 * @return true on success, false otherwise.
	 * @throws MIAException when ran on an unsupported OS.
	 */
	bool playSoundFromFile(const std::string& fileName);
	
	/**
	 * Attempts to play a sound from a file.
	 * @note This is a non-blocking call. Playback continues in the background
	 *       until the audio file finishes or stopSound() is called.
	 * @param fileName The file to try playing a sound from. This must be a
	 *        valid audio file of a supported type.
	 * @return true on success, false otherwise.
	 * @throws MIAException when ran on an unsupported OS.
	 * TODO - IN DEVELOPMENT!
	 */
	bool playSoundFromFileAsync(const std::string& fileName);

	/**
	 * Stops the currently playing sound.
	 * @param fadeOutMs The duration of the fade-out in milliseconds. A value
	 *        of 0 stops playback immediately.
	 * @return true if successful or if no sound was playing, false otherwise.
	 * @throws MIAException when ran on an unsupported OS.
	 * TODO - IN DEVELOPMENT!
	 */
	bool stopSound(uint32_t fadeOutMs = 0);
} // namespace audio