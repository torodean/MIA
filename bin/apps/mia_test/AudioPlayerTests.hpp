/**
 * @file AudioPlayerTests.hpp
 * @author Antonius Torode
 * @date 09/18/2026
 * @brief Various tests for the AudioPlayer class.
 */
#pragma once

/**
 * @brief Tests the basic playback features of the AudioPlayer class.
 *
 * Constructs an AudioPlayer, verifies the file is accepted, and lets the audio
 * play to completion while polling isAudioPlaying().
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerBasicPlayback(bool verboseMode);

/**
 * @brief Tests stopping an AudioPlayer after 5 seconds of playback.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerWithStop(bool verboseMode);

/**
 * @brief Tests stopping an AudioPlayer with a 5 second fade out.
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerWithFade(bool verboseMode);

/**
 * @brief Tests the repeatAudioTrack feature of the AudioPlayer class.
 *
 * Plays the short audio file with track repeating enabled for long enough
 * that it repeats a couple of times, then stops it with stopAudio().
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerWithRepeat(bool verboseMode);

/**
 * @brief Tests the restartAudio() feature of the AudioPlayer class.
 *
 * Lets the audio play for a few seconds, restarts it, and verifies it keeps
 * playing from the beginning.
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerRestart(bool verboseMode);

/**
 * @brief Tests changing the audio file while the audio is playing.
 *
 * Starts playing a short file, swaps in a long file mid-play, and verifies the
 * swap is accepted and the player reports it as playing.
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerChangeFileWhilePlaying(bool verboseMode);

/**
 * @brief Tests playing through a multi-track playlist to the end.
 *
 * Builds a playlist of the two test files, plays it through without looping,
 * and verifies the player finishes on its own after the last track.
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerPlaylistSequential(bool verboseMode);

/**
 * @brief Tests the playlist looping feature.
 *
 * Plays a playlist of short files with looping enabled for long enough that
 * it wraps past the end, then stops it with stopAudio().
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerPlaylistLoop(bool verboseMode);

/**
 * @brief Tests the playlist shuffle feature.
 *
 * Plays a playlist with shuffle enabled for long enough that several random
 * picks happen, then stops it with stopAudio().
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerPlaylistShuffle(bool verboseMode);

/**
 * @brief Tests the nextAudio() and previousAudio() skip features.
 *
 * Skips forward and back through a playlist and verifies the player keeps
 * playing and then reports stopped after being stopped.
 *
 * @param verboseMode Whether or not to enable verbose output.
 * @return Returns constants::SUCCESS on success, constants::FAILURE on failure.
 */
int testAudioPlayerPlaylistSkip(bool verboseMode);