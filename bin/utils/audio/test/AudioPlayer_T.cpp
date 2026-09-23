/**
 * @file AudioPlayer_T.cpp
 * @author Antonius Torode
 * @date 09/22/2026
 * @brief Unit tests for the AudioPlayer class using Google Test.
 *
 * The tests cover the player's state-level behavior (file and playlist
 * validation, volume clamping, mode flags, and the empty-playlist guards).
 * Audible playback itself is covered by the interactive tests in the mia_test
 * app, so these tests never start a playback thread.
 */

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <cstdio>

// Include the associated header file for methods to test.
#include "AudioPlayer.hpp"
// Used for logging setup and file reading in the VLC logging test.
#include "Logger.hpp"
#include "FileUtils.hpp"
// Used for the default log dir lookup in the VLC logging tests.
#include "Paths.hpp"
// Used for thread sleeps in the VLC logging test.
#include "Timing.hpp"

namespace audio
{
    /**
     * @brief Grants the unit tests access to set the fallback log file.
     *
     * AudioPlayer declares this class as a friend so the fallback test can
     * redirect the fallback log file without exposing it on the public API.
     */
    class AudioPlayerFallbackLogTestAccess
    {
    public:
        /**
         * @brief Sets the fallback log file name on the given player.
         * @param player The player to set the fallback log file on.
         * @param fileName The log file name to set.
         */
        static void setFallbackLogFile(AudioPlayer& player, const std::string& fileName)
        {
            player.fallbackLogFileName = fileName;
        }
    };

    /**
     * @brief Verifies a default constructed player starts in a quiet, default state.
     */
    TEST(AudioPlayer, DefaultConstructorState)
    {
        AudioPlayer player;

        EXPECT_FALSE(player.isAudioPlaying());
        EXPECT_EQ(player.getVolume(), 100u);
        EXPECT_FALSE(player.getRepeatAudioTrack());
        EXPECT_FALSE(player.getPlaylistLoop());
        EXPECT_FALSE(player.getPlaylistShuffle());
    }


    /**
     * @brief Verifies constructing a player from a file name succeeds for a supported type.
     */
    TEST(AudioPlayer, ConstructFromSupportedFile)
    {
        // The file does not need to exist since the type check only uses the extension.
        AudioPlayer player("/music/test_sound.mp3");

        EXPECT_FALSE(player.isAudioPlaying());
    }


    /**
     * @brief Verifies setAudioFile accepts a supported type and rejects an unsupported one.
     */
    TEST(AudioPlayer, SetAudioFileTypeCheck)
    {
        AudioPlayer player;

        EXPECT_TRUE(player.setAudioFile("/music/test_sound.mp3"));
        EXPECT_FALSE(player.setAudioFile("/docs/readme.txt"));
    }


    /**
     * @brief Verifies setPlaylist accepts a mix of files but fails when none are supported.
     */
    TEST(AudioPlayer, SetPlaylistFiltersUnsupportedFiles)
    {
        AudioPlayer player;

        // Unsupported entries are skipped, but the supported ones are kept.
        EXPECT_TRUE(player.setPlaylist({"/docs/readme.txt", "/music/track0.mp3"}));

        // A playlist with no supported entries is rejected.
        EXPECT_FALSE(player.setPlaylist({"/docs/readme.txt", "/images/pic.png"}));
    }


    /**
     * @brief Verifies addToPlaylist rejects unsupported files.
     */
    TEST(AudioPlayer, AddToPlaylistRejectsUnsupportedFile)
    {
        AudioPlayer player;

        EXPECT_TRUE(player.addToPlaylist("/music/track0.mp3"));
        EXPECT_FALSE(player.addToPlaylist("/docs/readme.txt"));
    }


    /**
     * @brief Verifies startAudio fails when no audio file was ever set.
     */
    TEST(AudioPlayer, StartAudioWithNoFileFails)
    {
        AudioPlayer player;

        EXPECT_FALSE(player.startAudio());
        EXPECT_FALSE(player.isAudioPlaying());

        // The failed start leaves the player in a usable state.
        EXPECT_TRUE(player.stopAudio());
    }


    /**
     * @brief Verifies startAudio fails after the playlist is cleared.
     */
    TEST(AudioPlayer, StartAudioAfterClearingPlaylistFails)
    {
        AudioPlayer player;
        EXPECT_TRUE(player.setAudioFile("/music/track0.mp3"));

        player.clearPlaylist();

        EXPECT_FALSE(player.startAudio());
        EXPECT_FALSE(player.isAudioPlaying());
    }


    /**
     * @brief Verifies nextAudio and previousAudio fail on a player with no tracks.
     */
    TEST(AudioPlayer, SkipWithNoTracksFails)
    {
        AudioPlayer player;

        EXPECT_FALSE(player.nextAudio());
        EXPECT_FALSE(player.previousAudio());
        EXPECT_FALSE(player.isAudioPlaying());
    }


    /**
     * @brief Verifies setVolume clamps out of range values into the 0 - 100 range.
     */
    TEST(AudioPlayer, VolumeClampsToValidRange)
    {
        AudioPlayer player;

        player.setVolume(50);
        EXPECT_EQ(player.getVolume(), 50u);

        player.setVolume(0);
        EXPECT_EQ(player.getVolume(), 0u);

        // Values above 100 clamp to the top of the range instead of wrapping.
        player.setVolume(150);
        EXPECT_EQ(player.getVolume(), 100u);
    }


    /**
     * @brief Verifies the mode setters store the value given to each of them.
     */
    TEST(AudioPlayer, ModeSettersRoundTrip)
    {
        AudioPlayer player;

        player.setRepeatAudioTrack(true);
        player.setPlaylistLoop(true);
        player.setPlaylistShuffle(true);
        EXPECT_TRUE(player.getRepeatAudioTrack());
        EXPECT_TRUE(player.getPlaylistLoop());
        EXPECT_TRUE(player.getPlaylistShuffle());

        // The same setters disable each mode again.
        player.setRepeatAudioTrack(false);
        player.setPlaylistLoop(false);
        player.setPlaylistShuffle(false);
        EXPECT_FALSE(player.getRepeatAudioTrack());
        EXPECT_FALSE(player.getPlaylistLoop());
        EXPECT_FALSE(player.getPlaylistShuffle());
    }


    /**
     * @brief Verifies stopAudio succeeds on a player which was never started.
     */
    TEST(AudioPlayer, StopWithoutPlaybackSucceeds)
    {
        AudioPlayer player;

        EXPECT_TRUE(player.stopAudio());
        EXPECT_FALSE(player.isAudioPlaying());
    }


    /**
     * @brief Verifies the libVLC logging callback routes diagnostics to the logger.
     *
     * The test creates a file with a supported audio extension but non-audio
     * contents, which makes libVLC emit warnings and errors while probing the
     * format. The callback routes those through the logger, so the log file
     * should contain the VLC-tagged lines rather than the player staying silent.
     * This test is Linux only since the callback is a Linux feature.
     */
    #if defined(IS_LINUX)
    TEST(AudioPlayer, VlcDiagnosticsAreLogged)
    {
        const std::string logFile = std::filesystem::absolute("vlcLogTest.log").string();
        const std::string badAudioFile = std::filesystem::absolute("not_actually_audio.mp3").string();
        std::remove(logFile.c_str());
        std::remove(badAudioFile.c_str());

        // The contents are not an audio format, so libVLC fails to probe the file.
        { std::ofstream out(badAudioFile); out << "this is not an audio file"; }

        /*
         * The context must outlive the player since BackgroundTask stores a
         * pointer to it, hence the declaration order here.
         */
        RuntimeContext context;
        context.logger.setLogFile(logFile);

        audio::AudioPlayer player;
        player.setContext(context);
        player.setAudioFile(badAudioFile);
        player.startAudio();

        // The probe fails fast, so a short wait is enough.
        timing::sleepMilliseconds(500);
        player.stopAudio();

        const std::string contents = files::readEntireFile(logFile);
        EXPECT_NE(contents.find("[VLC"), std::string::npos);

        std::remove(logFile.c_str());
        std::remove(badAudioFile.c_str());
    }


    /**
     * @brief Verifies the VLC logging fallback writes to the fallback log file.
     *
     * The test plays a non-audio file with a player which has no context set. The
     * callback then falls back to the fallback log file, so it should contain the
     * VLC-tagged lines. The fallback file is redirected through the friend access
     * class because the libVLC message text cannot be labeled as test output, so
     * this keeps the production default log free of unlabeled test entries.
     */
    TEST(AudioPlayer, VlcDiagnosticsFallBackToDefaultFile)
    {
        const std::string logFile = std::filesystem::absolute("vlcLogFallbackTest.log").string();
        const std::string badAudioFile = std::filesystem::absolute("not_actually_audio_fallback.mp3").string();
        std::remove(logFile.c_str());
        std::remove(badAudioFile.c_str());

        // The contents are not an audio format, so libVLC fails to probe the file.
        { std::ofstream out(badAudioFile); out << "this is not an audio file"; }

        audio::AudioPlayer player;
        AudioPlayerFallbackLogTestAccess::setFallbackLogFile(player, logFile);

        player.setAudioFile(badAudioFile);
        player.startAudio();

        // The probe fails fast, so a short wait is enough.
        timing::sleepMilliseconds(500);
        player.stopAudio();

        const std::string contents = files::readEntireFile(logFile);
        EXPECT_NE(contents.find("[VLC, WARNING]"), std::string::npos);

        std::remove(logFile.c_str());
        std::remove(badAudioFile.c_str());
    }
    #endif
} // namespace audio
