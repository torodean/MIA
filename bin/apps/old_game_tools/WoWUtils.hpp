/**
 * @file WoWUtils.hpp
 * @author Antonius Torode
 * @date 05/22/2025
 * @brief This file contains an old WoW features from the original MIA code. It originally
 *        lived in the VirtualKeyCodes file(s) but has since been decoupled. 
 */
#pragma once

#include "Constants.hpp"


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
    
#if defined(IS_WINDOWS)
    
    /**
     * Used for duplicating a letter in WoW. Useful for creating RP events.
     * @param copies The number of copies of the letter to make.
     * @param recipient The name of the recipient to send the letters to.
     */
    void duplicateLetter(int copies, std::string recipient);
    
    /**
     * Used for unloading letters from the mailbox in WoW. Useful for creating RP events.
     * Coordinates may need adapted based on screen resolutions, UI scales, etc..
     * @param copies The number of copies of the letter to make.
     */
    void unloadLetters(int copies);
    
#endif // Windows specifier
    
private:
    /**
     * WoW Mailbox variables. These have default values for a specific setup.
     */
    int WoWMailboxSendLetterLocationX{270};
    int WoWMailboxSendLetterLocationY{650};
    int WoWMailboxFirstLetterLocationX{55};
    int WoWMailboxFirstLetterLocationY{265};
    int WoWMailboxLootLetterLocationX{675};
    int WoWMailboxLootLetterLocationY{600};
    int WoWMailboxDeleteLetterLocationX{700};
    int WoWMailboxDeleteLetterLocationY{650};
} // class WoWUtils
