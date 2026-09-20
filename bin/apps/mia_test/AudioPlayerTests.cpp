/**
 * @file AudioPlayerTests.cpp
 * @author Antonius Torode
 * @date 09/18/2026
 * @brief Implements the features in the AudioPlayerTests header file.
 */

#include <iostream>
#include <string>

// Include the associated header file.
#include "AudioPlayerTests.hpp"
// Used for the AudioPlayer class being tested.
#include "AudioPlayer.hpp"
// Used for system return codes.
#include "Constants.hpp"
// Used for path detection and file finding.
#include "Paths.hpp"
// Used for timing capabilities.
#include "Timing.hpp"


namespace
{
    /**
     * @brief Constructs the full path to one of the test audio files.
     *
     * @param testFileName The name of the test audio file in this folder.
     * @return The full path to the test audio file.
     */
    static std::string getTestAudioFilePath(const std::string& testFileName)
    {
        // Constructs the test file location.
        std::string thisFolder = paths::getCppFileDirAtCompileTime(__FILE__);
        std::string fullFilePath = thisFolder + "/" + testFileName;

        /*
         * On a Cygwin system, the file paths are of the form '/cygdrive/s/...',
         * however, windows expects a windows specific filesystem form of 'S:\...'.
         * This will convert it before calling the files.
         */
    #if defined(__CYGWIN__)
        fullFilePath = paths::cygwinPathToWindowsPath(fullFilePath);
    #endif

        return fullFilePath;
    }
    
    /**
     * @brief Sleeps for a specified number of seconds, optionally displaying a countdown.
     *
     * @param player Reference to the audio player in order to periodically check for failure.
     * @param seconds The number of seconds to sleep.
     * @param verboseMode Whether to display the countdown.
     */
    static void sleepWithCountdown(const audio::AudioPlayer& player, 
                                   int seconds, 
                                   bool verboseMode = false)
    {
        if (verboseMode)
            std::cout << "Sleeping for " << seconds << " seconds..." << std::endl;

        for (int i=seconds; i>0; i--)
        {
            if (player.hasFailed())
                player.rethrowExceptionIfAny();
                
            if (verboseMode)
                std::cout << i << "..." << std::endl;

            timing::sleepSeconds(1);
        }
    }
    
    /**
     * @brief Starts audio playback and reports failure if unsuccessful.
     *
     * @param player The audio player to start.
     * @param fullFilePath The full file path to the file being played.
     * @param verboseMode Whether to display status messages.
     * @return True if playback started successfully, otherwise false.
     */
    static bool startAudio(audio::AudioPlayer& player,
                           const std::string& fullFilePath,
                           bool verboseMode = false)
    {
        if (verboseMode)
            std::cout << "Playing audio from file: " << fullFilePath << std::endl;

        bool status = player.startAudio();

        // Grace period to ensure the audio thread has started.
        timing::sleepMilliseconds(10);
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();

        if (!status)
        {
            if (verboseMode)
                std::cout << "FAILED starting audio player!" << std::endl;
            return false;
        }
        return true;
    }
    
    /**
     * @brief Same as startAudio() except without the first verbose message
     * so that a custom message can be used before this instead.
     */
    static bool startAudioNoMsg(audio::AudioPlayer& player,
                                bool verboseMode = false)
    {
        bool status = player.startAudio();

        // Grace period to ensure the audio thread has started.
        timing::sleepMilliseconds(10);
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();

        if (!status)
        {
            if (verboseMode)
                std::cout << "FAILED starting audio player!" << std::endl;
            return false;
        }
        return true;
    }
    
    /**
     * @brief Stops audio playback and reports failure if unsuccessful.
     *
     * @param player The audio player to stop.
     * @param fadeOutMs The fade-out duration in milliseconds.
     * @param verboseMode Whether to display status messages.
     * @return True if playback stopped successfully, otherwise false.
     */
    static bool stopAudio(audio::AudioPlayer& player,
                          uint32_t fadeOutMs = 0,
                          bool verboseMode = false)
    {
        bool status = player.stopAudio(fadeOutMs);        
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();        
        if (!status)
        {
            if (verboseMode)
                std::cout << "FAILED stopping audio player!" << std::endl;
            return false;
        }
        return true;
    }
    
    /**
     * @brief Verifies that the audio player is no longer playing.
     *
     * @param player The audio player to check.
     * @param verboseMode Whether to display failure messages.
     * @return True if audio is not playing, otherwise false.
     */
    static bool verifyAudioStopped(audio::AudioPlayer& player,
                                   bool verboseMode = false)
    {
        if (player.isAudioPlaying())
        {
            if (verboseMode)
                std::cout << "FAILED audio player still reports playing!" << std::endl;
            return false;
        }
        return true;
    }
} // namespace


int testAudioPlayerBasicPlayback(bool verboseMode)
{
    std::string fullFilePath = getTestAudioFilePath("test_sound_short.mp3");

    // Constructs the player from the test file.
    audio::AudioPlayer player(fullFilePath);

    // Starts playing the audio file.
    if (!startAudio(player, fullFilePath, verboseMode))
        return constants::FAILURE;

    // Waits for the audio to finish playing on its own.
    while (player.isAudioPlaying())
        timing::sleepMilliseconds(50);

    // The audio should no longer be playing.
    if (player.isAudioPlaying())
    {
        if (verboseMode)
            std::cout << "FAILED audio player still reports playing!" << std::endl;
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerWithStop(bool verboseMode)
{
    std::string fullFilePath = getTestAudioFilePath("test_sound_long.mp3");

    // Constructs the player from the test file.
    audio::AudioPlayer player(fullFilePath);

    // Starts playing the audio file.
    if (!startAudio(player, fullFilePath, verboseMode))
        return constants::FAILURE;

    // Wait 5 seconds then stop the audio.
    sleepWithCountdown(player, 5, verboseMode);

    // Stop the audio.
    if (!stopAudio(player, 0, verboseMode))
        return constants::FAILURE;

    // The audio should no longer be playing.
    if (!verifyAudioStopped(player, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerWithFade(bool verboseMode)
{
    std::string fullFilePath = getTestAudioFilePath("test_sound_long.mp3");

    // Constructs the player from the test file.
    audio::AudioPlayer player(fullFilePath);

    // Starts playing the audio file.
    if (!startAudio(player, fullFilePath, verboseMode))
        return constants::FAILURE;

    // Wait 5 seconds then fade out over 5 seconds.
    sleepWithCountdown(player, 5, verboseMode);

    // Stop the audio with a fade.
    if (!stopAudio(player, 5000, verboseMode)) // 5000 ms fade out.
        return constants::FAILURE;

    // The audio should no longer be playing.
    if (!verifyAudioStopped(player, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerWithRepeat(bool verboseMode)
{
    // The short test audio file is onle a couple seconds long.
    std::string fullFilePath = getTestAudioFilePath("test_sound_short.mp3");

    // Constructs the player from the test file and enables track repeating.
    audio::AudioPlayer player(fullFilePath);
    player.setRepeatAudioTrack(true);
    if (!player.getRepeatAudioTrack())
    {
        if (verboseMode)
            std::cout << "FAILED setting repeat audio track!" << std::endl;
        return constants::FAILURE;
    }

    // Starts playing the audio file.
    if (!startAudio(player, fullFilePath, verboseMode))
        return constants::FAILURE;

    // Lets the short file repeat a couple of times before stopping.
    sleepWithCountdown(player, 10, verboseMode);

    // Stop the audio.
    if (!stopAudio(player, 0, verboseMode))
        return constants::FAILURE;

    // The audio should no longer be playing.
    if (!verifyAudioStopped(player, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerRestart(bool verboseMode)
{
    std::string fullFilePath = getTestAudioFilePath("test_sound_long.mp3");

    // Constructs the player from the test file.
    audio::AudioPlayer player(fullFilePath);

    // Starts playing the audio file.
    if (!startAudio(player, fullFilePath, verboseMode))
        return constants::FAILURE;

    // Wait 5 seconds then restart the audio.
    sleepWithCountdown(player, 5, verboseMode);

    if (verboseMode)
        std::cout << "Restarting audio!" << std::endl;
    if (!player.restartAudio())
    {
        if (verboseMode)
            std::cout << "FAILED restarting audio player!" << std::endl;
        return constants::FAILURE;
    }

    // The audio should still be playing after the restart.
    if (verifyAudioStopped(player, false)) // Silence the error since it's expected here.
    {
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }
    
    // Wait 5 seconds to audibly make sure the audio restarted.
    sleepWithCountdown(player, 5, verboseMode);

    // Stop the audio.
    if (!stopAudio(player, 0, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerChangeFileWhilePlaying(bool verboseMode)
{
    std::string shortFilePath = getTestAudioFilePath("test_sound_short.mp3");
    std::string longFilePath = getTestAudioFilePath("test_sound_long.mp3");

    // Constructs the player from the short test file.
    audio::AudioPlayer player(longFilePath);

    // Starts playing the audio file.
    if (!startAudio(player, longFilePath, verboseMode))
        return constants::FAILURE;

    // Waits a bit for the audio to start, then swaps in the short file mid-play.
    sleepWithCountdown(player, 5, verboseMode);
    if (verboseMode)
        std::cout << "Setting new audio file! Audio should continue..." << std::endl;
    if (!player.setAudioFile(shortFilePath))
    {
        if (verboseMode)
            std::cout << "FAILED setting a new audio file mid-play!" << std::endl;
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }

    // The current playback should be unaffected, so it should still report playing.
    if (verifyAudioStopped(player, false)) // Silence the error since it's expected here.
    {
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }
    
    // Sleep for a couple seconds before cleaning up.
    sleepWithCountdown(player, 3, verboseMode);

    // Stop the audio.
    if (!stopAudio(player, 0, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerPlaylistSequential(bool verboseMode)
{
    std::string shortFilePath = getTestAudioFilePath("test_sound_short.mp3");
    std::string shortFilePath2 = getTestAudioFilePath("test_sound_short2.mp3");
    std::string longFilePath = getTestAudioFilePath("test_sound_long.mp3");

    // Constructs the player with a playlist of both test files.
    audio::AudioPlayer player;
    if (!player.setPlaylist({shortFilePath, shortFilePath2, longFilePath}))
    {
        if (verboseMode)
            std::cout << "FAILED setting the playlist!" << std::endl;
        return constants::FAILURE;
    }

    // Starts playing the playlist.
    if (verboseMode)
        std::cout << "Playing playlist: short file then long file." << std::endl;
    if (!startAudioNoMsg(player, verboseMode))
        return constants::FAILURE;
    
    // Print a status timer.
    if (verboseMode)
        std::cout << "The playlist is about 34 seconds long..." << std::endl;
    sleepWithCountdown(player, 34, verboseMode);

    // Waits for the playlist to finish playing on its own.
    while (player.isAudioPlaying())
        timing::sleepMilliseconds(50);

    // The player should have finished after the last track.
    if (player.isAudioPlaying())
    {
        if (verboseMode)
            std::cout << "FAILED audio player still reports playing!" << std::endl;
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerPlaylistLoop(bool verboseMode)
{
    std::string shortFilePath = getTestAudioFilePath("test_sound_short.mp3");
    std::string shortFilePath2 = getTestAudioFilePath("test_sound_short2.mp3");

    // Constructs the player with a looping playlist of short files.
    audio::AudioPlayer player;
    if (!player.setPlaylist({shortFilePath, shortFilePath2}))
    {
        if (verboseMode)
            std::cout << "FAILED setting the playlist!" << std::endl;
        return constants::FAILURE;
    }
    player.setPlaylistLoop(true);
    if (!player.getPlaylistLoop())
    {
        if (verboseMode)
            std::cout << "FAILED setting playlist loop!" << std::endl;
        return constants::FAILURE;
    }

    // Starts playing the playlist.
    if (verboseMode)
        std::cout << "Playing looping playlist from files: " << std::endl
                  << "  " << shortFilePath << std::endl
                  << "  " << shortFilePath2 << std::endl;
    if (!startAudioNoMsg(player, verboseMode))
        return constants::FAILURE;

    // Lets the playlist wrap around past the end before stopping.
    sleepWithCountdown(player, 12, verboseMode);

    // Stop the audio.
    if (!stopAudio(player, 0, verboseMode))
        return constants::FAILURE;

    // The audio should no longer be playing.
    if (!verifyAudioStopped(player, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerPlaylistShuffle(bool verboseMode)
{
    std::string shortFilePath = getTestAudioFilePath("test_sound_short.mp3");
    std::string shortFilePath2 = getTestAudioFilePath("test_sound_short2.mp3");
    std::string longFilePath = getTestAudioFilePath("test_sound_long.mp3");

    // Constructs the player with a shuffled playlist of both test files.
    audio::AudioPlayer player;
    if (!player.setPlaylist({shortFilePath, shortFilePath2, longFilePath}))
    {
        if (verboseMode)
            std::cout << "FAILED setting the playlist!" << std::endl;
        return constants::FAILURE;
    }
    player.setPlaylistShuffle(true);
    if (!player.getPlaylistShuffle())
    {
        if (verboseMode)
            std::cout << "FAILED setting playlist shuffle!" << std::endl;
        return constants::FAILURE;
    }

    // Starts playing the playlist.
    if (verboseMode)
        std::cout << "Playing shuffled playlist of three test files." << std::endl;
    if (!startAudioNoMsg(player, verboseMode))
        return constants::FAILURE;

    // Lets the shuffle pick a few tracks before stopping.
    sleepWithCountdown(player, 20, verboseMode);

    // Stop the audio.
    if (!stopAudio(player, 0, verboseMode))
        return constants::FAILURE;

    // The audio should no longer be playing.
    if (!verifyAudioStopped(player, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}


int testAudioPlayerPlaylistSkip(bool verboseMode)
{
    std::string longFilePath = getTestAudioFilePath("test_sound_long.mp3");
    std::string longFilePath2 = getTestAudioFilePath("test_sound_long2.mp3");

    // Constructs the player with a playlist of both test files.
    audio::AudioPlayer player;
    if (!player.setPlaylist({longFilePath, longFilePath2}))
    {
        if (verboseMode)
            std::cout << "FAILED setting the playlist!" << std::endl;
        return constants::FAILURE;
    }

    // Starts playing the playlist.
    if (verboseMode)
        std::cout << "Playing playlist with forward and backward skips." << std::endl;
    if (!startAudioNoMsg(player, verboseMode))
        return constants::FAILURE;

    // Waits a bit, then skips forward to the next track.
    sleepWithCountdown(player, 5, verboseMode);
    if (verboseMode)
        std::cout << "Skipping to the next audio track." << std::endl;
    if (!player.nextAudio())
    {
        if (verboseMode)
            std::cout << "FAILED skipping to the next track!" << std::endl;
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }

    // Waits a short time (short audio is very short), then skips back to the previous track.
    sleepWithCountdown(player, 5, verboseMode);
    if (verboseMode)
        std::cout << "Returning to the previous audio track." << std::endl;
    if (!player.previousAudio())
    {
        if (verboseMode)
            std::cout << "FAILED skipping to the previous track!" << std::endl;
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }

    // The player should still be playing after the skips.
    if (verifyAudioStopped(player, false)) // Silence the error since it's expected here.
    {
        player.stopAudio();
        // Rethrow any exceptions if they occured in the audio thread.
        player.rethrowExceptionIfAny();
        return constants::FAILURE;
    }
    
    // Waits a few before stopping.
    sleepWithCountdown(player, 5, verboseMode);

    // Stop the audio.
    if (!stopAudio(player, 0, verboseMode))
        return constants::FAILURE;

    // If it made it this far, it's a success.
    return constants::SUCCESS;
}
