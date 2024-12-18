cmake_minimum_required(VERSION 3.16)

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

# Add the executable
add_executable(QtProject mainWindow.cpp main.cpp fileHandling.cpp trie.cpp splashScreen.cpp)

# Link against Qt libraries
target_link_libraries(QtProject PRIVATE Qt6::Widgets)
