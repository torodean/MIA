/**
 * @file TestingUtils.cpp
 * @author Antonius Torode
 * @date 10/13/2026
 * @brief Implements various utilities to streamline and improve testing.
 */

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

// Include associated header file.
#include "TestingUtils.hpp"

namespace testing
{
    std::string createTempTestFile(const std::vector<std::string>& lines)
    {
        static uint32_t tempFileCounter = 0;
        std::string filename = "temp_test_file_"  + std::to_string(tempFileCounter) + ".txt";
        std::ofstream ofs(filename);
        for (const auto& line : lines) 
        {
            ofs << line << std::endl;
        }
        ofs.close();
        return filename;
    }


    void removeFile(const std::string& filename)
    {
        if (std::filesystem::exists(filename))
            std::remove(filename.c_str());
    }
} // namespace testing
