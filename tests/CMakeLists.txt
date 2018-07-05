cmake_minimum_required(VERSION 3.0)
project(AtomsTests)
include(ExternalProject)

find_package(Git)

ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
set(CATCH_INCLUDE_DIR ${CMAKE_BINARY_DIR}/src/catch/single_include/catch2 CACHE INTERNAL "Path to include folder for Catch")

# Setup includes
include_directories(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
include_directories(${ATOMS_INCLUDE})

# Find all tests
file(GLOB_RECURSE TESTS "*.cpp")

# Set test binary
add_executable(tests ${TESTS})
add_dependencies(tests catch)

enable_testing(true)