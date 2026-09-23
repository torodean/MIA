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

// Include the associated header file for methods to test.
#include "AudioPlayer.hpp"

namespace audio
{
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
} // namespace audio
