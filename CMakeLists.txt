# Required CMake version.
cmake_minimum_required(VERSION 2.8)

# Project name.
project(AaToolkit C CXX)

set(CMAKE_CXX_FLAGS "/EHa")

if(CMAKE_GENERATOR MATCHES ".* Win64")
    add_definitions(-DAA_TOOLKIT_X64_LIBRARY)

    set(ARCHITECTURE_DIR x64)

    set(CMAKE_RELEASE_POSTFIX -x64)
    
    set(CMAKE_DEBUG_POSTFIX -x64d)
else(CMAKE_GENERATOR MATCHES ".* Win64")
    set(ARCHITECTURE_DIR .)

    set(CMAKE_DEBUG_POSTFIX d)
endif(CMAKE_GENERATOR MATCHES ".* Win64")

# Include directoires.
include_directories(
    include
)

# Include files.
file(GLOB HEADER_FILES
    include/Aa*
    include/bits/Aa*
)

# Source files.
file(GLOB SOURCE_FILES
    src/Aa*
)

# Target generation.
add_library(AaToolkit
    SHARED
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# Output paths.
set_target_properties(
    AaToolkit
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/bin/debug/${ARCHITECTURE_DIR}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/bin/release/${ARCHITECTURE_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/lib/debug/${ARCHITECTURE_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/release/${ARCHITECTURE_DIR}"
    COMPILE_DEFINITIONS AA_TOOLKIT_BUILD_SHARED
)

# Target generation.
add_library(AaToolkit-static
    STATIC
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

# Output paths.
set_target_properties(
    AaToolkit-static
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${PROJECT_SOURCE_DIR}/lib/debug/${ARCHITECTURE_DIR}"
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${PROJECT_SOURCE_DIR}/lib/release/${ARCHITECTURE_DIR}"
)
