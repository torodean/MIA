/**
 * @file PythonModule_T.cpp
 * @author Antonius Torode
 * @date 09/08/2026
 * @brief Tests for the PythonModule wrapper and its call() overloads.
 *
 * The tests run against PythonModuleTests.py, which must sit next to the test
 * binary since the wrapper imports from the working directory. One interpreter
 * and module are shared by the whole suite through SetUpTestSuite, since
 * repeatedly finalizing and restarting the embedded interpreter between tests
 * is both slow and fragile.
 */

#include <memory>
#include <string>
#include <stdexcept>

#include <gtest/gtest.h>

// Associated header to test.
#include "PythonModule.hpp"
// Used for the thrown module load failure.
#include "MIAException.hpp"
// Used for the error codes of the thrown exceptions.
#include "Error.hpp"


/**
 * Test fixture for the PythonModule tests.
 * The fixture owns one module and interpreter for the entire suite so each
 * test does not pay interpreter startup and shutdown costs, and so the tests
 * never exercise interpreter re-initialization.
 */
class PythonModule_T : public ::testing::Test
{
protected:

    /**
     * Creates the shared module once for the whole suite.
     */
    static void SetUpTestSuite()
    {
        module = std::make_unique<PythonModule>("PythonModuleTests", __FILE__);
    }

    /**
     * Releases the shared module, which shuts the interpreter down.
     */
    static void TearDownTestSuite()
    {
        module.reset();
    }

    /// The module shared by all tests in this suite.
    static std::unique_ptr<PythonModule> module;
};

std::unique_ptr<PythonModule> PythonModule_T::module = nullptr;


/**
 * @brief Verifies a missing module throws the Python_Module_Load_Failure code.
 */
TEST_F(PythonModule_T, MissingModuleThrows)
{
    EXPECT_THROW(PythonModule module("NoSuchModuleAnywhere", __FILE__), error::MIAException)
        << "Constructing a module which cannot be imported should throw.";
}

/**
 * @brief Verifies the thrown exception carries the expected error code.
 */
TEST_F(PythonModule_T, MissingModuleErrorCode)
{
    try
    {
        PythonModule module("NoSuchModuleAnywhere", __FILE__);
        FAIL() << "Constructing a missing module should not succeed.";
    }
    catch (const error::MIAException& e)
    {
        EXPECT_EQ(e.getCode(), error::ErrorCode::Python_Module_Load_Failure)
            << "The thrown exception should carry Python_Module_Load_Failure.";
    }
}

/**
 * @brief Verifies hasMethod finds methods which exist and misses those which do not.
 */
TEST_F(PythonModule_T, PresentAndMissingMethod)
{
    EXPECT_TRUE(module->hasMethod("add"))
        << "hasMethod should find a method which exists.";
    EXPECT_FALSE(module->hasMethod("missingMethod"))
        << "hasMethod should not find a method which does not exist.";
}

/**
 * @brief Verifies calling a method with no arguments reaches Python and returns void.
 */
TEST_F(PythonModule_T, NoArgumentVoidReturn)
{
    PythonResult result = module->call("doNothing");

    EXPECT_TRUE(result.isValid())
        << "A call to an existing method should be valid.";
    EXPECT_EQ(result.getType(), PythonResult::Type::Void)
        << "A method returning None should produce a Void result.";
}

/**
 * @brief Verifies an integer return value comes back through asInt.
 */
TEST_F(PythonModule_T, IntegerReturn)
{
    PythonResult result = module->call("add", 2, 3);

    EXPECT_TRUE(result.isValid())
        << "The add call should succeed.";
    EXPECT_EQ(result.getType(), PythonResult::Type::Integer)
        << "An int return should produce an Integer result.";
    EXPECT_EQ(result.asInt(), 5)
        << "add(2, 3) should return 5.";
}

/**
 * @brief Verifies a double return value comes back through asDouble.
 */
TEST_F(PythonModule_T, DoubleReturn)
{
    PythonResult result = module->call("multiply", 2.5, 4.0);

    EXPECT_TRUE(result.isValid())
        << "The multiply call should succeed.";
    EXPECT_EQ(result.getType(), PythonResult::Type::Double)
        << "A float return should produce a Double result.";
    EXPECT_DOUBLE_EQ(result.asDouble(), 10.0)
        << "multiply(2.5, 4.0) should be 10.0.";
}

/**
 * @brief Verifies a string return value comes back through asString.
 */
TEST_F(PythonModule_T, StringReturn)
{
    PythonResult result = module->call("greet", "Antonius");

    EXPECT_TRUE(result.isValid())
        << "The greet call should succeed.";
    EXPECT_EQ(result.getType(), PythonResult::Type::String)
        << "A str return should produce a String result.";
    EXPECT_EQ(result.asString(), "Hello, Antonius!");
}

/**
 * @brief Verifies a mixed string and integer argument list works with no dedicated overload.
 */
TEST_F(PythonModule_T, MixedArgumentTypes)
{
    PythonResult result = module->call("describe", "Antonius", 30);

    EXPECT_TRUE(result.isValid())
        << "The describe call should succeed.";
    EXPECT_EQ(result.asString(), "Antonius is 30 years old.")
        << "describe should mix the string and integer arguments.";
}

/**
 * @brief Verifies calling a missing method returns an error result instead of throwing.
 */
TEST_F(PythonModule_T, MissingMethodIsErrorResult)
{
    PythonResult result = module->call("missingMethod");

    EXPECT_FALSE(result.isValid())
        << "A missing method should produce an invalid result.";
    EXPECT_NE(result.getError().find("not found"), std::string::npos)
        << "The error message should mention the method was not found.";
}

/**
 * @brief Verifies a Python-side exception surfaces as an error result with the message.
 */
TEST_F(PythonModule_T, PythonTypeErrorIsErrorResult)
{
    // Python's + concatenates two strings, so mixing a string and an integer
    // is what actually raises the TypeError inside add(a, b).
    PythonResult result = module->call("add", "x", 1);

    EXPECT_FALSE(result.isValid())
        << "A Python TypeError should produce an invalid result.";
    EXPECT_NE(result.getError().find("can only concatenate str"), std::string::npos)
        << "The error message should carry the Python exception text.";
}

/**
 * @brief Verifies two modules can be alive and called at the same time.
 */
TEST_F(PythonModule_T, TwoModulesAlive)
{
    PythonModule second("PythonModuleTests", __FILE__);

    PythonResult first = module->call("add", 1, 1);
    PythonResult fromSecond = second.call("add", 2, 2);

    EXPECT_TRUE(first.isValid() && fromSecond.isValid())
        << "Both modules should be callable while alive.";
    EXPECT_EQ(first.asInt(), 2)
        << "The first module should still work.";
    EXPECT_EQ(fromSecond.asInt(), 4)
        << "The second module should be independently usable.";
}
