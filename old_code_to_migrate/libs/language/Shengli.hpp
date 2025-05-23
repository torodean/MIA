/**
 * File: Shengli.hpp
 * Author: Antonius Torode
 * Date: 04/20/2022
 * Description: Shengli is a language based off of rearranging english letters. This file contains code associated
 *              with Shengli tools and utilities.
 */

#include <string>
#include <vector>

/**
 * This is a class contains code associated with Shengli tools and utilities.
 */
class Shengli
{
public:

    Shengli() = default;

    ~Shengli() = default;

    /**
     * Initializes the Shengli object.
     */
    void initialize();

    /**
     * This will take an English string and convert it to Shengli.
     * @param input the input English string to translate.
     * @return The output shengli string.
     */
    std::string toShengli(const std::string& english);

    /**
     * This will take a Shengli string and convert it to English.
     * @param input the input Shengli string to translate.
     * @return The output English string.
     */
    std::string toEnglish(const std::string& shengli);

private:

    /**
     * This method will attempt to create a shengli word from an english word following a set of rules.
     * @param english
     * @return
     */
    std::string createShengliWord(const std::string& english);

    /**
     * This will write a translation (word) to the dictionary file (english and corresponding shengli).
     * @param english The english word.
     * @param shengli The equivalent shengli word.
     */
    void writeToDictionary(const std::string& english, const std::string& shengli);

    /**
     * This will find the shengli word corresponding to an input english word.
     * @param english
     * @return
     */
    std::string findShengliInDictionary(const std::string& english);

    /**
     * This will load the dictionary file into RAM and store it into the dictionary variable.
     * @return Returns true if successful.
     */
    bool loadDictionary();

    /**
     * This will unload the dictionary file from RAM by clearing the dictionary variable.
     */
    void unloadDictionary();

    std::string dictionaryFileName; ///< This is the filename of the dictionary file.

    std::vector<std::string> dictionary; ///< This is the words contained in the dictionary file.
};
