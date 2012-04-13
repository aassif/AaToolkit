# Required CMake version.
cmake_minimum_required(VERSION 2.8)

# Project name.
project(AaToolkit C CXX)

set(CMAKE_CXX_FLAGS "/EHa")

# Include directoires.
include_directories(
  include
)

# Include files.
set(PROJECT_HDR
  include/AaException
  include/AaMath
  include/AaText
  include/AaCmdLine
  include/AaRatio
  include/AaTimer
  include/AaVector
  include/AaXml
  include/bits/AaToolkit.h
  include/bits/AaException.h
  include/bits/AaMath.h
  include/bits/AaText.h
  include/bits/AaCmdLine.hh
  include/bits/AaRatio.h
  include/bits/AaTimer.h
  include/bits/AaVector.hh
  include/bits/AaXml.h
)

# Source files.
set(PROJECT_SRC
  src/AaException.cc
  src/AaMath.cc
  src/AaText.cc
  src/AaRatio.cc
  src/AaTimer.cc
  src/AaXml.cc
)

# Target generation.
add_library(AaToolkit
  SHARED
  ${PROJECT_HDR}
  ${PROJECT_SRC}
)

# Output paths.
set_target_properties(
  AaToolkit
  PROPERTIES
  #RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  #LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  #ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
  RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/bin/debug
  RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin/release
  LIBRARY_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/lib/debug
  LIBRARY_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG   ${PROJECT_SOURCE_DIR}/lib/debug
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/lib/release
  DEBUG_POSTFIX d
)

