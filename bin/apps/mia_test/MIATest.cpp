/**
 * @file MIATest.cpp
 * @author Antonius Torode
 * @date 09/03/2026
 * @brief Implements any test features.
 */

#include <iostream>
#include <string>
#include <vector>

// The associated header file.
#include "MIATest.hpp"
// Used for the Python module wrapper being tested.
#include "PythonModule.hpp"
// Used for the python module test methods.
#include "PythonModuleTests.hpp"
// Used for the python plotting test methods.
#include "PythonPlotterTests.hpp"
// Used for the python UI test methods.
#include "PythonUITests.hpp"
// Used for the keybind listener test methods.
#include "KeybindTests.hpp"
// Used to test some system sound features.
#include "SystemSoundTests.hpp"
// Used to test the AudioPlayer features.
#include "AudioPlayerTests.hpp"


MIATest::MIATest() :
    testHelpOpt("-H", "--testHelp", "Prints the various test cases available.",
                CommandOption::commandOptionType::BOOL_OPTION),
    testIndexOpt("-i", "--index", "The index of the test to run.",
                 CommandOption::commandOptionType::INT_OPTION)
{ };


/*
 * This method currently does what the base class does... It is only here
 * so that I do not need to re-add it when adding test options that need parsed.
 */
void MIATest::initialize(int argc, char* argv[])
{
    try
    {    
        MIAApplication::initialize(argc, argv);
       
        bool testHelp = false;
        testHelpOpt.getOptionVal<bool>(argc, argv, testHelp);
        if (testHelp)
        {
            printTestHelp();
            std::exit(constants::SUCCESS);
        }
        testIndexOpt.getOptionVal<int>(argc, argv, testIndexToRun);
    }
    catch (const error::MIAException& ex)
    {
        std::cerr << "Error during MIATest::initialize: " << ex.what() << std::endl;
    }
}


void MIATest::printTestHelp()
{
    std::cout << "Valid test options are:" << std::endl
              << "  0: Test PythonModule." << std::endl
              << "  1: Test PythonPlotter basic plotting." << std::endl
              << "  2: Test PythonPlotter multi-line plotting." << std::endl
              << "  3: Test PythonPlotter per-line x-axis values." << std::endl
              << "  4: Test PythonPlotter input validation." << std::endl
              << "  5: Test basic PythonUI interactions." << std::endl
              << "  6: Test the python UI library listener." << std::endl
              << "  7: Test the keybind listener." << std::endl
              << "  8: Test the keybind listener with global capture." << std::endl
              << "  9: Test playing system sounds." << std::endl
              << " 10: Test playing an mp3 sound from a file." << std::endl
              << " 11: Test playing a wav sound from a file for 5s then stopping." << std::endl
              << " 12: Test playing a wav sound from a file for 5s then fading out over 5s." << std::endl
              << " 13: Test basic AudioPlayer playback to completion." << std::endl
              << " 14: Test the AudioPlayer stopped after 5s." << std::endl
              << " 15: Test the AudioPlayer faded out over 5s." << std::endl
              << " 16: Test the AudioPlayer repeating a short file for 20s." << std::endl
              << " 17: Test the AudioPlayer restartAudio() feature." << std::endl
              << " 18: Test the AudioPlayer changing the audio file mid-play." << std::endl
              << " 19: Test the AudioPlayer playing a playlist to the end." << std::endl
              << " 20: Test the AudioPlayer looping a playlist for 20s." << std::endl
              << " 21: Test the AudioPlayer shuffling a playlist for 20s." << std::endl
              << " 22: Test the AudioPlayer skipping forward and back in a playlist." << std::endl
              << " 23: Test the AudioPlayer changing the volume during playback." << std::endl
              << " 24: Test the AudioPlayer playing a changed file after the current track." << std::endl;
}


void MIATest::printHelp() const
{
    MIAApplication::printHelp();
   
    // This is a dump of the help messages used by the various command options.
    std::cout << "MIATest specific options:" << std::endl
              << testHelpOpt.getHelp() << std::endl
              << testIndexOpt.getHelp() << std::endl
              << std::endl;
}



int MIATest::run()
{
    bool verboseMode = getVerboseMode();
    switch (testIndexToRun)
    {
        case 0:  return testPythonModule();
        case 1:  return testPythonPlotter(verboseMode);
        case 2:  return testPythonPlotterMultiLine(verboseMode);
        case 3:  return testPythonPlotterPerLineX(verboseMode);
        case 4:  return testPythonPlotterValidation();
        case 5:  return testPythonUI();
        case 6:  return testPythonUILibrary();
        case 7:  return testKeybindListener(verboseMode);
        case 8:  return testKeybindListenerGlobalCapture(verboseMode);
        case 9:  return testSystemBeeps(verboseMode);
        case 10: return testPlayingSoundFromFile(verboseMode);
        case 11: return testPlayingSoundFromFileWithStop(verboseMode);
        case 12: return testPlayingSoundFromFileWithFade(verboseMode);
        case 13: return testAudioPlayerBasicPlayback(verboseMode);
        case 14: return testAudioPlayerWithStop(verboseMode);
        case 15: return testAudioPlayerWithFade(verboseMode);
        case 16: return testAudioPlayerWithRepeat(verboseMode);
        case 17: return testAudioPlayerRestart(verboseMode);
        case 18: return testAudioPlayerChangeFileWhilePlaying(verboseMode);
        case 19: return testAudioPlayerPlaylistSequential(verboseMode);
        case 20: return testAudioPlayerPlaylistLoop(verboseMode);
        case 21: return testAudioPlayerPlaylistShuffle(verboseMode);
        case 22: return testAudioPlayerPlaylistSkip(verboseMode);
        case 23: return testAudioPlayerVolumeChange(verboseMode);
        case 24: return testAudioPlayerChangeFilePlaysNext(verboseMode);
        default:
            std::cerr << "Invalid test index: " << testIndexToRun << std::endl;
            printTestHelp();
            return constants::FAILURE;
    }
}
