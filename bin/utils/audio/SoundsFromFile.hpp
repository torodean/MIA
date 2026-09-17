/**
 * @file SoundsFromFile.hpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief An interface for playing sounds from files.
 */
#pragma once

#include <string>

namespace sounds
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
} // namespace sounds