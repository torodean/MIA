/**
 * @file AudioPlayer.hpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief An interface for an interuptible audio player.
 */
#pragma once

#include <string>
#include <cstdint>
#include <mutex>
#include <atomic>

// Used for creating a threaded task.
#include "BackgroundTask.hpp"

namespace audio
{
	/**
	 * This is an audio player which is setup to play audio in a
	 * separate thread which the caller can manage as needed.
	 * 
	 * TODO - In development!
	 */
	class AudioPlayer : public threading::BackgroundTask
	{
	public:
		/**
		 * Default constructor.
		 */
		AudioPlayer() = default;
		
		/**
		 * Construct this object from an audio file.
		 * @param fileName The file to try playing a sound from. This
		 *        must be a valid audio file of a supported type.
		 */
		AudioPlayer(const std::string& fileName);
		
		/**
		 * Sets the audio file for this player to use. If the audio file
		 * is currently playing, this will change the file for the next time
		 * the audio is played (either through startAudio(), restartAudio(),
		 * or audio looping).
		 *
		 * @param fileName The file to try playing a sound from. This
		 *        must be a valid audio file of a supported type.
		 * @return true on success, false otherwise (invalid file type).
		 */
		bool setAudioFile(const std::string& fileName);
		
		/**
		 * Provides a check to see if the audio file is currently playing.
		 */
		bool isAudioPlaying() const;
		
		/**
		 * Starts playing the audio file. If the audio is already playing,
		 * this will do nothing. This will call start() to begin the background
		 * thread which will play the audio.
		 * @return true when successfully playing the audio, false otherwise.
		 */
		bool startAudio();
		
		/**
		 * This will restart the audio (if it is playing). If the audio is
		 * not already playing, this will start it. 
		 * @return true if the sound was successfully started/restarted,
		 *         false otherwise.
		 */
		bool restartAudio();
		
		/**
		 * Stops playing the audio file. This will call stop() to also stop the
		 * audio player thread that is running.
		 * @param fadeOutMS The time (in ms) over which to fade the audio file out.
		 * @return true if the audio was successfully stopped, false otherwise.
		 */
		bool stopAudio(uint32_t fadeOutMS = 0);
		
		/**
		 * Enable or disable the audioLoop feature which loops the audio file.
		 * This can be set while the audio is being played.
		 * @param loop Whether or not to loop the audio (false by default).
		 */
		void setAudioLoop(bool loop);
		
		/// Getter for the current audio loop setting.
		bool getAudioLoop() const;
		
	protected:
		
		/**
		 * Overriding this so it hides it from the public API. This is to avoid
		 * any unintentional shadowing of startAudio(). This is intended to simply 
		 * call the BackgroundTask::start() unless later expanded. The startAudio()
		 * will handle calling BackgroundTask::start() itself.
		 */
		void start() override;
		
		/**
		 * Overriding this so it hides it from the public API. This is to avoid
		 * any unintentional shadowing of stopAudio(). This is intended to simply 
		 * call the BackgroundTask::stop() unless later expanded. The stopAudio()
		 * will handle calling BackgroundTask::stop() itself.
		 */
		void stop() override;
		
		/**
		 * The threaded task that plays the sound.
		 */
		void run() override;
		
	private:
		/// The audio file to use for this player.
		std::string fileName;
		/// Mutex so the fileName can safely change during play.
		std::mutex fileNameMutex;
		
		/// Whether or not to repeat/loop the audio file until stop is called.
		std::atomic<bool> audioLoop{false};
		/// Tracker of whether or not the audio is currently playing.
		std::atomic<bool> audioPlaying{false};
	}; // class AudioPlayer
} // namespace audio