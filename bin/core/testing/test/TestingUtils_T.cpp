/**
* @file TestingUtils_T.cpp
* @author Antonius Torode
* @date 10/13/2026
* @brief Unit tests for TestingUtils.hpp.
*/

#include <string>
#include <utility>

#include <gtest/gtest.h>

#include "TestingUtils.hpp"

namespace testing
{
    namespace
    {
        /*
         * Trivial object with a dump method for testing allStringsInDump.
         */
        struct Dumpable
        {
            void dump(std::ostream& os) const
            {
                os << "name=Alice score=42";
            }
        };


        /*
         * Object whose dump matches the common default argument form
         * `void dump(std::ostream& os = std::cout) const;`
         */
        struct DefaultedDump
        {
            void dump(std::ostream& os = std::cout) const
            {
                os << "status=ok";
            }
        };
    } // namespace


    /*
     * Tests for the std::string overload of containsAllStrings.
     */
    TEST(ContainsAllStrings, NeedleChecks)
    {
        EXPECT_TRUE(containsAllStrings(std::string("alpha beta gamma"),
                                       std::string("alpha"),
                                       std::string("beta"),
                                       std::string("gamma")))
            << "All needles present should return true.";

        EXPECT_FALSE(containsAllStrings(std::string("alpha beta"),
                                        std::string("alpha"),
                                        std::string("delta")))
            << "One missing needle should return false.";

        EXPECT_TRUE(containsAllStrings(std::string("hello world"),
                                       std::string("lo wo")))
            << "A substring needle counts as contained.";

        EXPECT_TRUE(containsAllStrings(std::string("alpha"), std::string("")))
            << "An empty needle is always contained.";

        EXPECT_FALSE(containsAllStrings(std::string(""), std::string("alpha")))
            << "An empty haystack rejects a non-empty needle.";

        EXPECT_TRUE(containsAllStrings(std::string("anything")))
            << "No needles hits the base case and returns true.";
    }


    /*
     * Tests for the const char* overload of containsAllStrings.
     */
    TEST(ContainsAllStringsLiterals, LiteralChecks)
    {
        EXPECT_TRUE(containsAllStrings("host=example.org port=443",
                                       "host=example.org",
                                       "port=443"))
            << "String literals should forward to the std::string overload.";

        EXPECT_FALSE(containsAllStrings("host=example.org port=443",
                                        "host=example.org",
                                        "port=80"))
            << "A missing literal needle should return false.";

        EXPECT_TRUE(containsAllStrings("anything"))
            << "No needles hits the base case and returns true.";
    }


    /*
     * Tests for allStringsInDump.
     */
    TEST(AllStringsInDump, DumpChecks)
    {
        Dumpable obj;
        EXPECT_TRUE(allStringsInDump(obj, "name=Alice", "score=42"))
            << "All needles present in the dump should return true.";

        EXPECT_FALSE(allStringsInDump(obj, "name=Alice", "score=7"))
            << "A needle missing from the dump should return false.";

        EXPECT_TRUE(allStringsInDump(obj))
            << "No needles hits the base case and returns true.";

        DefaultedDump defaulted;
        EXPECT_TRUE(allStringsInDump(defaulted, "status=ok"))
            << "A dump method with a default cout argument should work.";
    }
} // namespace testing
