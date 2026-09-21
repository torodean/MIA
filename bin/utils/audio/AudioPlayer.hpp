/**
 * @file AudioPlayer.hpp
 * @author Antonius Torode
 * @date 09/17/2026
 * @brief An interface for an interruptible audio player.
 */
#pragma once

#include <string>
#include <cstdint>
#include <cstddef>
#include <mutex>
#include <atomic>
#include <vector>

// Used for creating a threaded task.
#include "BackgroundTask.hpp"
// Used for the playlist this player plays through.
#include "Playlist.hpp"
// Used for preprocessor macros.
#include "Constants.hpp"

// Linux uses vlc.
#if defined(IS_LINUX)
    #include <vlc/vlc.h>
#endif

namespace audio
{
    // @TODO - Implement on Linux.
    /**
     * @brief An audio player which is setup to play audio in a separate
     *        thread which the caller can manage as needed.
     *
     * @note This class inherits from threading::BackgroundTask. Any exception
     *       thrown while playing audio is captured in the background thread.
     *       Callers of this class should periodically check hasFailed() and
     *       rethrow any exceptions that are thrown. This class is intended to
     *       be designed in a non-terminating way, where all of the methods can
     *       signal failure in a recoverable manner. An exception signals a non-
     *       recoverable pattern that should end the application.
     */
    class AudioPlayer : public threading::BackgroundTask
    {
    public:
        /**
         * @brief Default constructor.
         * @throws MIAException if VLC fails to create an instance (Linux only).
         */
        AudioPlayer();

        /**
         * @brief Constructs this object from an audio file.
         * @param fileName The file to try playing a sound from. This must
         *        be a valid audio file of a supported type.
         * @throws MIAException if VLC fails to create an instance (Linux only).
         */
        AudioPlayer(const std::string& fileName);

        /**
         * @brief Destroys the audio player and releases all platform-specific resources.
         *
         * On Linux, stops playback and releases the libVLC media list player,
         * media list, and VLC instance. On Windows, no additional cleanup is
         * required here.
         */
         ~AudioPlayer();

        /**
         * @brief Sets the audio file for this player to use.
         *
         * The player behaves as if it has a playlist at all times, so this
         * simply replaces the playlist with a single-entry playlist.
         *
         * If the audio file is currently playing, this will change the file
         * for the next time the audio is played (either through startAudio(),
         * restartAudio(), or audio looping).
         *
         * @param fileName The file to try playing a sound from. This must
         *        be a valid audio file of a supported type.
         * @return true on success, false otherwise (invalid file type).
         */
        bool setAudioFile(const std::string& fileName);

        /**
         * @brief Sets the playlist for this player to use.
         *
         * Unsupported files are skipped. If the audio is currently playing,
         * this takes effect on the next track rather than interrupting the
         * current playback. The playlist index is reset to the first entry.
         *
         * @param fileNames The list of files to try playing. Unsupported
         *        entries are skipped.
         * @return true if at least one file was added, false otherwise.
         */
        bool setPlaylist(const std::vector<std::string>& fileNames);

        /**
         * @brief Adds a single audio file to the end of the playlist.
         *
         * If the audio is currently playing, this takes effect on the next
         * track rather than interrupting the current playback.
         *
         * @param fileName The file to add. This must be a valid audio file
         *        of a supported type.
         * @return true on success, false otherwise (invalid file type).
         */
        bool addToPlaylist(const std::string& fileName);

        /**
         * @brief Clears the playlist and resets the playlist index.
         */
        void clearPlaylist();

        /**
         * @brief Plays the next track in the playlist.
         *
         * If the audio is currently playing, the current playback is stopped
         * and the next track starts playing. With shuffle enabled this picks
         * a random track instead of the next sequential one.
         *
         * @return true if the next track was started, false otherwise (empty
         *         playlist or the player failed to start the track).
         */
        bool nextAudio();

        /**
         * @brief Plays the previously played track.
         *
         * This uses the shuffle memory, so it plays the track which was
         * playing before the current one regardless of the shuffle setting.
         * If the audio is currently playing, the current playback is stopped
         * and the previous track starts playing.
         *
         * @return true if the previous track was started, false otherwise
         *         (empty shuffle memory, an empty playlist, or the player
         *         failed to start the track).
         */
        bool previousAudio();

        /**
         * @brief Provides a check to see if the audio file is currently playing.
         * @return true if the audio is currently playing, false otherwise.
         */
        bool isAudioPlaying() const;

        /**
         * @brief Starts playing the audio file.
         *
         * If the audio is already playing, this will do nothing. This will
         * call start() to begin the background thread which will play the audio.
         *
         * @return true when successfully playing the audio, false otherwise.
         */
        bool startAudio();

        /**
         * @brief Restarts the audio (if it is playing).
         *
         * If the audio is not already playing, this will start it.
         *
         * @return true if the sound was successfully started/restarted,
         *         false otherwise.
         */
        bool restartAudio();

        /**
         * @brief Stops playing the audio file.
         *
         * This will call stop() to also stop the audio player thread that
         * is running.
         *
         * @param fadeOutMs The time (in ms) over which to fade the audio file out.
         * @return true if the audio was successfully stopped, false otherwise.
         */
        bool stopAudio(uint32_t fadeOutMs = 0);

        /**
         * @brief Enable or disable the repeatAudioTrack feature which repeats
         *        the current audio track until stop is called.
         * @param repeat Whether or not to repeat the audio track (false by default).
         */
        void setRepeatAudioTrack(bool repeat);

        /// Getter for the current repeatAudioTrack setting.
        bool getRepeatAudioTrack() const;

        /**
         * @brief Enable or disable the playlistLoop feature which loops the
         *        playlist back to the first track after the last track ends.
         * @note On Linux, this sets the vlc playback mode to libvlc_playback_mode_loop.
         * @param loop Whether or not to loop the playlist (false by default).
         * @return true on success, false otherwise.
         */
        void setPlaylistLoop(bool loop);

        /// Getter for the current playlistLoop setting.
        bool getPlaylistLoop() const;

        /**
         * @brief Enable or disable the playlist shuffle feature.
         *
         * When enabled, advancing to the next track picks a random track
         * instead of the next sequential one.
         *
         * @param shuffle Whether or not to shuffle the playlist (false by default).
         */
        void setPlaylistShuffle(bool shuffle);

        /// Getter for the current playlist shuffle setting.
        bool getPlaylistShuffle() const;

    protected:

        /**
         * @brief Overrides this so it hides it from the public API.
         *
         * This is to avoid any unintentional shadowing of startAudio(). This is
         * intended to simply call the BackgroundTask::start() unless later
         * expanded. The startAudio() will handle calling BackgroundTask::start()
         * itself.
         */
        void start() override;

        /**
         * @brief Overrides this so it hides it from the public API.
         *
         * This is to avoid any unintentional shadowing of stopAudio(). This is
         * intended to simply call the BackgroundTask::stop() unless later
         * expanded. The stopAudio() will handle calling BackgroundTask::stop()
         * itself.
         */
        void stop() override;

        /**
         * @brief The threaded task that plays the sound.
         * @note On Linux, this method does nothing since the vlc libraries create
         *       their own asynchronous threads.
         */
        void run() override;

    private:
        /**
         * @brief Stops the current playback and plays the next track in the
         *        playlist.
         *
         * @return true if the next track was started, false otherwise.
         */
        bool playNextTrack();

        /**
         * @brief Stops the current playback and plays the most recent entry
         *        in the shuffle memory.
         *
         * @return true if the previous track was started, false otherwise.
         */
        bool playPreviousTrack();

        /// The playlist of audio files this player plays through.
        Playlist playlist;
        /// Mutex so the playlist can safely change during play.
        std::mutex playlistMutex;

        /// Whether or not to repeat the current audio track until stop is called.
        std::atomic<bool> repeatAudioTrack{false};
        /// Whether or not to loop the playlist back to the first track after the last.
        std::atomic<bool> playlistLoop{false};
        /// Whether or not to shuffle through the playlist tracks.
        std::atomic<bool> playlistShuffle{false};

    #if defined(IS_WINDOWS)
    
        /// Tracker of whether or not the audio is currently playing.
        std::atomic<bool> audioPlaying{false};
        /// Whether or not a fade-out was requested for the current playback.
        std::atomic<bool> fadeRequested{false};
        /// The time (in ms) over which the current fade-out should run.
        std::atomic<uint32_t> fadeTimeMS{0};
        
    #elif defined(IS_LINUX)
    
        /**
         * @brief Rebuilds the libVLC media list from the current C++ Playlist.
         *
         * Releases the existing media list, creates a new one, and re-adds every
         * track currently stored in @c playlist. Must be called whenever the
         * playlist content changes.
         *
         * @note The caller must hold both @c vlcMutex and @c playlistMutex
         *       (or guarantee equivalent exclusive access).
         */
        void rebuildVlcMediaList();

        /**
         * @brief Applies the current repeat / loop settings to the VLC list player.
         *
         * Maps the atomic flags @c repeatAudioTrack and @c playlistLoop onto
         * the corresponding @c libvlc_playback_mode_t value.
         *
         * @note Must be called while holding @c vlcMutex.
         */
        void applyPlaybackMode();
        
        /// Top-level libVLC instance (lifetime of this object).
        libvlc_instance_t* vlcInstance = nullptr;
        /// The media list that VLC plays through.
        libvlc_media_list_t* vlcPlaylist = nullptr;
        /// List player that owns playback control.
        libvlc_media_list_player_t* vlcPlayer = nullptr;
        /// Protects all VLC objects above.
        mutable std::mutex vlcMutex;
        
    #endif
    }; // class AudioPlayer
} // namespace audio
