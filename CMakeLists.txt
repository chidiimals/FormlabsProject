cmake_minimum_required(VERSION 3.10)

project(FormLabsProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(nlohmann_json REQUIRED)

# Find the libcurl package
find_package(CURL REQUIRED)

# Add the executable for your project

# Main executable
add_executable(FormLabsProject main.cpp downloader_tests.cpp)
target_include_directories(FormLabsProject PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(FormLabsProject PRIVATE nlohmann_json::nlohmann_json CURL::libcurl)

# Google Test setup with FetchContent
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Add test executable
add_executable(FormLabsTests test.cpp CSVReadertests.cpp layer_tests.cpp)
target_include_directories(FormLabsTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link against GoogleTest and other dependencies
target_link_libraries(FormLabsTests PRIVATE gtest_main nlohmann_json::nlohmann_json)

# Include GoogleTest for test discovery
include(GoogleTest)
gtest_discover_tests(FormLabsTests)
