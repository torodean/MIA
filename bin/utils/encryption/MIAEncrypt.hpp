/**
 * @file MIAEncrypt.hpp
 * @author Antonius Torode
 * @date 03/11/2021
 * Description: This file contains the code pertaining to MIA Encryption.
 */
#pragma once

#include <string>
#include <vector>
#include <cstdint>

using std::string;
using std::vector;

/**
 * This is the main class for the Etaoin character set. This character set has
 * translations for each character where they can be rotated or mirrored to
 * produce other characters. These transformations can be used to modify an
 * Etaoin string which can be used for encryption.
 */
class Etaoin
{
    /**
     * The etaoinChar represents a single character of the Etaoin encoding.
     * This can be translated into any value within an extended ascii table.
     */
    typedef uint8_t etaoinChar;

    /// Main default constructor.
    Etaoin() = default;

    /// This is the main constructor for an Etaoin object from a string input.
    explicit Etaoin(const string& input);

    /// The main default destructor.
    ~Etaoin() = default;

    /**
     * This will vertically mirror an etaoin character and return it's mirrored counterpart.
     * @param input the etaoin character to mirror.
     * @return etaoinChar.
     */
    static etaoinChar mirrorVertically(etaoinChar input);

    /**
     * This will horizontally mirror an etaoin character and return it's mirrored counterpart.
     * @param input the etaoin character to mirror.
     * @return etaoinChar.
     */
    static etaoinChar mirrorHorizontally(etaoinChar input);

    /**
     * This will take any english string and convert it to a modified Etaoin string.
     */
    static string toModifiedEtaoin(const string& input);

    /**
     * This will take any english string and convert it to an Etaoin vector.
     */
    static vector<etaoinChar> toEtaoin(const string& input);

private:

    /**
     * This is the main container for the data contained by the Etaoin content.
     */
    vector<etaoinChar> data;

};

/**
 * This is the main class for MIA encryption.
 */
class MIAEncrypt
{
public:

    /**
     * Main constructor for the MIAEncrypt object.
     */
    MIAEncrypt() = default;

    /**
     * Main destructor of a MIAEncrypt object.
     */
     ~MIAEncrypt() = default;

    /**
     * This method will take a string and perform simple encryption on it.
     * @param input[string&] - A reference to a string to encrypt.
     * @return [string] - returns an encrypted string.
     */
    static string encryptedString(const string& input);

protected:

private:

    /// The main container for the data to be encrypted.
    Etaoin data;

};
