/**
 * File: WoWUtils.hpp
 * Author: Antonius Torode
 * Created on: 05/22/2025
 * Description: This file contains an old WoW features from the original MIA code. It originally
 *     lived in the VirtualKeyCodes file(s) but has since been decoupled. 
 */
#pragma once



/**
 * This class contains small helper functions relating to World of Warcraft.
 */
class WoWUtils
{
public:
    /**
     * The main constructor of the WoWUtils class.
     */
    WoWUtils();
    
    /// Default destructor.
    ~WoWUtils();
    
    #if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined _WIN32 || defined _WIN64 || defined __CYGWIN__
    
    /**
     * Used for duplicating a letter in WoW. Useful for creating RP events.
     * @param copies[int] - The number of copies of the letter to make.
     * @param recipient[std::string] - The name of the recipient to send the letters to.
     */
    void duplicateLetter(int copies, std::string recipient);
    
    /**
     * Used for unloading letters from the mailbox in WoW. Useful for creating RP events.
     * Coordinates may need adapted based on screen resolutions, UI scales, etc..
     * @param copies[int] - The number of copies of the letter to make.
     */
    void unloadLetters(int copies);
    
    #endif // Windows specifier
}
