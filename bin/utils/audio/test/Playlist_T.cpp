/**
 * @file Playlist_T.cpp
 * @author Antonius Torode
 * @date 09/18/2026
 * @brief Unit tests for the Playlist class using Google Test.
 *
 * The tests use track metadata with distinct file paths to tell the tracks
 * apart. The shuffle tests assert bounds and non-repetition properties rather
 * than exact picks since the picks are randomized.
 */

#include <gtest/gtest.h>

// Include the associated header file for methods to test.
#include "Playlist.hpp"

namespace audio
{
    /**
     * @brief Constructs the metadata for a test track.
     *
     * @param n The index used to generate a distinct file path.
     * @return The metadata of the test track.
     */
    static files::FileMetaData makeTrack(int n)
    {
        files::FileMetaData track;
        track.fullFilePath = "/music/track" + std::to_string(n) + ".mp3";
        track.fileName = "track" + std::to_string(n);
        track.type = files::FileType::Mp3;
        return track;
    }


    /**
     * @brief Verifies a default constructed playlist is empty and cannot move its index.
     */
    TEST(Playlist, DefaultIsEmpty)
    {
        Playlist playlist;

        EXPECT_EQ(playlist.current().type, files::FileType::Unknown);

        // An empty playlist cannot advance or restore from memory.
        EXPECT_FALSE(playlist.advance(false, false));
        EXPECT_FALSE(playlist.back());
    }


    /**
     * @brief Verifies setList replaces the tracks and resets the cursor state.
     */
    TEST(Playlist, SetListResetsState)
    {
        std::vector<files::FileMetaData> first = {makeTrack(0), makeTrack(1)};
        std::vector<files::FileMetaData> second = {makeTrack(2)};

        Playlist playlist(first);

        // Advance so the index and history have state to reset from.
        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_EQ(playlist.current().fileName, "track1");

        EXPECT_TRUE(playlist.setList(second));
        EXPECT_EQ(playlist.current().fileName, "track2");

        // The reset means the first track is the current one and there is no memory.
        EXPECT_FALSE(playlist.back());
    }


    /**
     * @brief Verifies setList fails and stores nothing when given no tracks.
     */
    TEST(Playlist, SetListEmptyFails)
    {
        Playlist playlist;
        std::vector<files::FileMetaData> empty;

        EXPECT_FALSE(playlist.setList(empty));
        EXPECT_EQ(playlist.current().type, files::FileType::Unknown);
    }


    /**
     * @brief Verifies add appends a track without disturbing the current index.
     */
    TEST(Playlist, AddAppendsTrack)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1)};
        Playlist playlist(tracks);

        // Move to the second track, then add a new one.
        EXPECT_TRUE(playlist.advance(false, false));
        playlist.add(makeTrack(2));

        // The current track is unaffected by the addition.
        EXPECT_EQ(playlist.current().fileName, "track1");

        // The new track is reachable by advancing again.
        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_EQ(playlist.current().fileName, "track2");
    }


    /**
     * @brief Verifies clear removes the tracks and the shuffle memory.
     */
    TEST(Playlist, ClearResetsEverything)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1)};
        Playlist playlist(tracks);

        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_TRUE(playlist.back());

        playlist.clear();
        EXPECT_EQ(playlist.current().type, files::FileType::Unknown);
        EXPECT_FALSE(playlist.back());
    }


    /**
     * @brief Verifies current returns a default track when the playlist is empty.
     */
    TEST(Playlist, CurrentOnEmptyPlaylistIsEmpty)
    {
        Playlist playlist;

        EXPECT_EQ(playlist.current().type, files::FileType::Unknown);
    }


    /**
     * @brief Verifies advance moves through the tracks sequentially without looping.
     */
    TEST(Playlist, AdvanceSequential)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1), makeTrack(2)};
        Playlist playlist(tracks);

        EXPECT_EQ(playlist.current().fileName, "track0");
        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_EQ(playlist.current().fileName, "track1");
        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_EQ(playlist.current().fileName, "track2");

        // The last track without looping cannot advance, so the index stays put.
        EXPECT_FALSE(playlist.advance(false, false));
        EXPECT_EQ(playlist.current().fileName, "track2");
    }


    /**
     * @brief Verifies advance wraps the index around to the first track when looping.
     */
    TEST(Playlist, AdvanceWithLoop)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1)};
        Playlist playlist(tracks);

        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_TRUE(playlist.advance(false, true));
        EXPECT_EQ(playlist.current().fileName, "track0");
    }


    /**
     * @brief Verifies shuffle picks stay within the playlist bounds and never repeat.
     *
     * The picks are random, so this asserts properties over many iterations
     * instead of an exact sequence.
     */
    TEST(Playlist, AdvanceShuffleStaysInBounds)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1), makeTrack(2)};
        Playlist playlist(tracks);

        for (int i = 0; i < 100; ++i)
        {
            int previous = static_cast<int>(playlist.current().fileName.back() - '0');

            EXPECT_TRUE(playlist.advance(true, true));
            int current = static_cast<int>(playlist.current().fileName.back() - '0');

            EXPECT_GE(current, 0);
            EXPECT_LE(current, 2);

            // Shuffle never picks the index being moved away from.
            EXPECT_NE(current, previous);
        }
    }


    /**
     * @brief Verifies back restores the index to the track left behind by advancing.
     */
    TEST(Playlist, BackReturnsToPreviousTrack)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1)};
        Playlist playlist(tracks);

        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_EQ(playlist.current().fileName, "track1");

        EXPECT_TRUE(playlist.back());
        EXPECT_EQ(playlist.current().fileName, "track0");
    }


    /**
     * @brief Verifies back fails when the shuffle memory is empty.
     */
    TEST(Playlist, BackWithEmptyMemoryFails)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1)};
        Playlist playlist(tracks);

        // No track was left behind yet, so there is no memory to restore from.
        EXPECT_FALSE(playlist.back());

        // The memory is used up after the one entry is consumed.
        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_TRUE(playlist.back());
        EXPECT_FALSE(playlist.back());
    }


    /**
     * @brief Verifies back restores the memory entries in most-recent-first order.
     */
    TEST(Playlist, BackRestoresHistoryInOrder)
    {
        std::vector<files::FileMetaData> tracks = {makeTrack(0), makeTrack(1), makeTrack(2)};
        Playlist playlist(tracks);

        EXPECT_TRUE(playlist.advance(false, false));
        EXPECT_TRUE(playlist.advance(false, true));
        EXPECT_EQ(playlist.current().fileName, "track2");

        // The memory unwinds in reverse order of the tracks which were left.
        EXPECT_TRUE(playlist.back());
        EXPECT_EQ(playlist.current().fileName, "track1");
        EXPECT_TRUE(playlist.back());
        EXPECT_EQ(playlist.current().fileName, "track0");
    }
} // namespace audio
