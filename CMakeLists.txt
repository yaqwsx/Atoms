CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(AtomsExamples CXX)

# SET result to a list of subdirectories in curdir
# taken from: https://stackoverflow.com/questions/7787823/cmake-how-to-get-the-name-of-all-subdirectories-of-a-directory
MACRO(SUBDIRLIST result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH(child ${children})
        IF(IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist "${child}")
        ENDIF()
    ENDFOREACH()
    SET(${result} ${dirlist})
ENDMACRO()

# configure compiler to C++14 and (nearly) all warnings
SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
# To be compatible with Visual Studio (https://stackoverflow.com/questions/2368811/how-to-set-warning-level-in-cmake)
IF(MSVC)
  IF(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    STRING(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  ELSE()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
ENDIF()

# Future ToDo: Build atoms shared library here

# Prepare include path
SET(ATOMS_INCLUDE "${PROJECT_SOURCE_DIR}/include")

# Add all examples
SUBDIRLIST(examples_list "${PROJECT_SOURCE_DIR}/examples")
FOREACH(example ${examples_list})
    MESSAGE(STATUS "Configuring example: ${example}")
    ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/examples/${example}")
ENDFOREACH()

# Build Atoms tests
ADD_SUBDIRECTORY("${PROJECT_SOURCE_DIR}/tests")
