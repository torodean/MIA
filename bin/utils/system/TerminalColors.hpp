/**
 * File: TerminalColors.hpp
 * Author: Antonius Torode
 * Creation Date: 06/18/2025
 * Description: A lightweight C++ header-only utility for printing colored text to the 
 *     terminal using ANSI escape codes. Supports foreground and background colors, as 
 *     well as resetting formatting. Compatible with Unix-like systems and Windows 10+.
 */
#pragma once

#include <iostream>

namespace terminal_colors
{
    /**
     * @enum ColorCode
     * @brief ANSI escape code values for terminal text formatting.
     *
     * Defines foreground and background color codes, as well as a reset code,
     * for use with ANSI-compatible terminal output.
     *
     * Values:
     *   FG_*       - Foreground (text) colors
     *   BG_*       - Background colors
     *   RESET      - Resets all formatting to default
     */
    enum ColorCode 
    {
        FG_RED      = 31,
        FG_GREEN    = 32,
        FG_YELLOW   = 33,
        FG_BLUE     = 34,
        FG_MAGENTA  = 35,
        FG_CYAN     = 36,
        FG_WHITE    = 37,
        FG_DEFAULT  = 39,
        BG_RED      = 41,
        BG_GREEN    = 42,
        BG_BLUE     = 44,
        BG_DEFAULT  = 49,
        RESET       = 0
    };

    /**
     * @brief These methods insert the ANSI escape sequence for the colors into the stream.
     * @param os[std::ostream&] - The output stream.
     * @return [std::ostream&] - The modified output stream.
     *
     * Example usage:
     *     std::cout << terminal_colors::red   << "This text is red" 
     *               << terminal_colors::reset << "This is normal text";
     */
    inline std::ostream& red(std::ostream& os)    { return os << "\033[" << ColorCode::FG_RED     << "m"; }
    inline std::ostream& bgred(std::ostream& os)  { return os << "\033[" << ColorCode::BG_RED     << "m"; }
    inline std::ostream& green(std::ostream& os)  { return os << "\033[" << ColorCode::FG_GREEN   << "m"; }
    inline std::ostream& bggreen(std::ostream& os){ return os << "\033[" << ColorCode::BG_GREEN   << "m"; }
    inline std::ostream& yellow(std::ostream& os) { return os << "\033[" << ColorCode::FG_YELLOW  << "m"; }
    inline std::ostream& blue(std::ostream& os)   { return os << "\033[" << ColorCode::FG_BLUE    << "m"; }
    inline std::ostream& bgblue(std::ostream& os) { return os << "\033[" << ColorCode::BG_BLUE    << "m"; }
    inline std::ostream& magenta(std::ostream& os){ return os << "\033[" << ColorCode::FG_MAGENTA << "m"; }
    inline std::ostream& cyan(std::ostream& os)   { return os << "\033[" << ColorCode::FG_CYAN    << "m"; }
    inline std::ostream& white(std::ostream& os)  { return os << "\033[" << ColorCode::FG_WHITE   << "m"; }
    inline std::ostream& def(std::ostream& os)    { return os << "\033[" << ColorCode::FG_DEFAULT << "m"; }
    inline std::ostream& bgdef(std::ostream& os)  { return os << "\033[" << ColorCode::BG_DEFAULT << "m"; }
    inline std::ostream& reset(std::ostream& os)  { return os << "\033[" << ColorCode::RESET      << "m"; }
    
    /**
     * @class ColorChanger
     * @brief Utility class for applying ANSI color codes to terminal output streams.
     *
     * Constructs an object representing an ANSI escape sequence, which can be
     * inserted into an output stream (e.g., std::cout) to change text formatting
     * such as foreground/background color.
     *
     * Example usage:
     *     ColorChanger red(FG_RED);
     *     std::cout << red << "This text is red";
     */
    class ColorChanger 
    {
    public:
        /**
         * @brief Constructs a ColorChanger with the given ANSI color code.
         * @param cCode[ColorCode] - The ANSI color code to apply.
         */
        ColorChanger(ColorCode cCode) : code(cCode)
        { }
        
        /**
         * @brief Overloaded stream insertion operator.
         *
         * Injects the ANSI escape sequence corresponding to the color code
         * into the output stream.
         *
         * @param os[std::ostream&] - The output stream to modify.
         * @param mod[const ColorChanger&] - The ColorChanger object containing the color code.
         * @return [std::ostream&] - Reference to the modified output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const ColorChanger& mod) 
        { return os << "\033[" << mod.code << "m"; }
        
    private:    
        ColorCode code; ///< ANSI color code to apply.
    }; // class ColorChanger 
} // namespace terminal_colors
