/**
 * @file KeybindTests.hpp
 * @author Antonius Torode
 * @date 09/16/2026
 * @brief Declares the keybind listener test methods.
 */
#pragma once


/**
 * Tests the KeybindListener with movement bindings and push bindings.
 *
 * The keybinds come from the MIATest.MIA config: WASD are bound to
 * movement events, GHJ to push events, and alt+t / alt+shift+s to
 * modifier pushes. Each completed binding prints once through the event
 * queue, and a held movement key keeps printing roughly every 50
 * milliseconds through the held-state queries. The test runs until the
 * process is interrupted.
 *
 * @param verboseMode Enables verbose listener output when true.
 * @return SUCCESS when the listener loop completes, FAILURE otherwise.
 */
int testKeybindListener(bool verboseMode);

/**
 * Tests the KeybindListener the same way as testKeybindListener() but with
 * global capture enabled, so the bound keys are grabbed system-wide and
 * swallowed while the test runs.
 *
 * @param verboseMode Enables verbose listener output when true.
 * @return SUCCESS when the listener loop completes, FAILURE otherwise.
 */
int testKeybindListenerGlobalCapture(bool verboseMode);
