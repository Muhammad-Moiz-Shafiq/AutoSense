cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

# Project Name
project(QtProject LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the /Zc:__cplusplus flag for MSVC
if (MSVC)
    add_compile_options(/Zc:__cplusplus)
endif()

# Set Qt installation path (update this to your Qt installation directory)
set(CMAKE_PREFIX_PATH "C:/Qt/6.8.1/mingw_64/lib/cmake")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Find the necessary Qt components
find_package(Qt6 REQUIRED COMPONENTS Widgets)
# Find CURL package with specific path
set(CURL_ROOT "C:/vcpkg/installed/x64-windows")
set(CURL_INCLUDE_DIR "${CURL_ROOT}/include")
set(CURL_LIBRARY "${CURL_ROOT}/lib/libcurl.lib")
find_package(CURL REQUIRED)

# Add the executable
add_executable(QtProject mainWindow.cpp main.cpp fileHandling.cpp trie.cpp splashScreen.cpp apiHandler.cpp analysisWindow.cpp)

# Link against Qt libraries
target_link_libraries(QtProject PRIVATE Qt6::Widgets)

target_link_libraries(${PROJECT_NAME} PRIVATE CURL::libcurl)
# Include directories
target_include_directories(QtProject PRIVATE 
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CURL_INCLUDE_DIR})