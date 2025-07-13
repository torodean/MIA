/**
 * @file AppFramework.hpp
 * @author Antonius Torode
 * @date 05/22/2025
 * Description: 
 *   Provides a simple templated application framework to enforce
 *   a common app lifecycle: initialize, loadConfig, and run.
 * 
 *   This header defines a concept to constrain apps with required
 *   methods, a template function to launch apps in a uniform way,
 *   and a helper macro to define main() for any app class.
 */
#pragma once

#include <concepts>

/**
 * @concept AppInterface
 * @brief Concept that enforces the presence of initialize and run methods on App.
 * 
 * Requirements:
 * - void initialize(int argc, char** argv)
 * - int run()
 */
template<typename App>
concept AppInterface = requires(App app, int argc, char** argv) 
{
    { app.initialize(argc, argv) } -> std::same_as<void>;
    { app.run() } -> std::same_as<int>;
};

/**
 * @brief Template function to launch an application.
 * 
 * Calls in order:
 *   1) initialize(argc, argv)
 *   3) run()
 * 
 * @tparam App Type of application implementing the AppInterface concept.
 * @param argc Argument count from main.
 * @param argv Argument values from main.
 * @return int Return code from app run().
 */
template<AppInterface App>
int runApp(int argc, char** argv)
{
    App app;
    app.initialize(argc, argv);
    return app.run();
}

/**
 * @def MIA_MAIN(AppClass)
 * @brief Macro to define the main() entry point using the templated launcher.
 * 
 * Usage:
 *   MIA_MAIN(MyAppClass);
 */
#define MIA_MAIN(AppClass)                   \
int main(int argc, char** argv)              \
{                                            \
    return runApp<AppClass>(argc, argv);     \
}
