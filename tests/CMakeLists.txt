CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(AtomsTests)
INCLUDE(ExternalProject)

FIND_PACKAGE(Git)

# Get Catch framework: https://github.com/philsquared/Catch/
ExternalProject_Add(
    catch
    PREFIX ${CMAKE_BINARY_DIR}/catch
    GIT_REPOSITORY https://github.com/philsquared/Catch.git
    TIMEOUT 10
    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
   )

# Expose required variable (CATCH_INCLUDE_DIR) to parent scope
ExternalProject_Get_Property(catch source_dir)
SET(CATCH_INCLUDE_DIR ${source_dir}/include CACHE INTERNAL "Path to include folder for Catch")

# Setup includes
INCLUDE_DIRECTORIES(${CATCH_INCLUDE_DIR} ${COMMON_INCLUDES})
INCLUDE_DIRECTORIES(${ATOMS_INCLUDE})

# Find all tests
FILE(GLOB_RECURSE TESTS "*.cpp")

# Set test binary
ADD_EXECUTABLE(tests ${TESTS})
ADD_DEPENDENCIES(tests catch)

enable_testing(true)