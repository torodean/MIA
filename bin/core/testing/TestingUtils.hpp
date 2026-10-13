/**
 * @file TestingUtils.hpp
 * @author Antonius Torode
 * @date 10/13/2026
 * @brief Various utilities to streamline and improve testing.
 */
#pragma once

#include <sstream>
#include <string>
#include <utility>

namespace testing
{
    /**
     * This represents internal methods which are intended to be used only within
     * this file. The methods outside of this namespace are intended to be called
     * by outside tests, and these namespaced methods are then used by those.
     */
    namespace
    {
        /**
         * The base case. This is called during the veriadic template methods when
         * there are no needles left, so that every check passed.
         *
         * containsAllStrings(haystack, "a", "b", "c")
         * containsAllStrings(haystack, "b", "c")
         * containsAllStrings(haystack, "c")
         * containsAllStrings(haystack)       <- base case, returns true.
         *
         * @param Any string (unused parameter).
         * @return true.
         */
        inline bool containsAllStrings(const std::string& /*haystack*/)
        {
            return true;
        }

        /**
         * Checks that every needle string appears in the haystack string.
         *
         * The checks run in order; the first missing needle stops the search
         * and returns false.
         *
         * @param haystack The string to search within.
         * @param needle The string to find in the haystack.
         * @param needles Any number of additional strings to find.
         * @tparam Needles Types of the additional needles; each must convert
         *         to std::string.
         * @return true if every needle appears in the haystack, else false.
         */
        template <typename... Needles>
        bool containsAllStrings(const std::string& haystack,
                                const std::string& needle,
                                const Needles&... needles)
        {
            return haystack.find(needle) != std::string::npos &&
                   containsAllStrings(haystack, needle...);
        }

        /**
         * Overload of containsAllStrings so call sites can pass string literals
         * without wrapping them in std::string.
         *
         * Each argument is converted to std::string and forwarded to the
         * std::string overload above.
         *
         * @param haystack The string to search within.
         * @param needle The string to find in the haystack.
         * @param needles Any number of additional strings to find.
         * @tparam Needles Types of the additional needles; each must convert
         *         to std::string.
         * @return true if every needle appears in the haystack, else false.
         */
        template <typename... Needles>
        bool containsAllStrings(const char* haystack,
                                const char* needle,
                                const Needles&... needles)
        {
            return containsAllStrings(std::string(haystack), 
                                      std::string(needle), needles...);
        }
    } // namespace

    /**
     * @brief Test helper for dump style methods such as:
     * `void dump(std::ostream& os = std::cout) const;`
     *
     * Streams the object into a buffer by calling obj.dump(os), then checks
     * that every needle appears in the resulting text.
     *
     * @param obj The object to dump; must provide dump(std::ostream&).
     * @param needles Any number of strings expected in the dumped output.
     * @tparam Needles Types of the needles; each must convert to std::string.
     * @return true if every needle appears in the dumped text, else false.
     */
    template <typename ObjToDump, typename... Needles>
    bool allStringsInDump(const ObjToDump& obj, const Needles&... needles)
    {
        std::ostringstream os;
        obj.dump(os);
        return containsAllStrings(os.str(), needles...);
    }
} // namespace testing
