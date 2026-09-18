/**
 * @file SoundsFromFile.hpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief An interface for playing sounds from files.
 */
#pragma once

#include <string>
#include <cstdint>

// Used for file and metadata handling.
#include "FileUtils.hpp"

namespace audio
{
	/**
	 * This method returns if the audio file is a supported type or not.
	 * This delegates to the isASupportedType(const FileMetaData&) overload.
	 * @param fileName The file name to check for support. This uses the
	 *        file extension to perform the check.
     * @param verboseMode Enables verboseMode output (default = false).
	 * @return true if the type is supported.
	 */
	bool isASupportedType(const std::string& fileName,
	                      bool verboseMode = false);
	
	/**
	 * This method returns if the audio file is a supported type or not.
	 * When a new type is supported, it must be added to this method.
	 * @param data The metaData of the file to check. This can be
	 *        constructed with the getFileMetaData() method.
     * @param verboseMode Enables verboseMode output (default = false).
	 * @return true if the type is supported.
	 */
	bool isASupportedType(const files::FileMetaData& data,
	                      bool verboseMode = false);
	
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
	bool stopSound(uint32_t fadeOutMs = 0, bool verboseMode = false);
} // namespace audio