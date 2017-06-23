#
# Use this file as follows:
# $ cmake -DCMAKE_TOOLCHAIN_FILE=TargetEPuck.cmake <other args...>
#

# The name of the target system
# 'Linux' here is fine because the target board has a Linux OS on it
# NOTE: When this variable is set, the variable CMAKE_CROSSCOMPILING
# is also set automatically by CMake
set(CMAKE_SYSTEM_NAME Linux)

# Full path to C compiler
set(CMAKE_C_COMPILER 
/home/ken/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-gcc)

# Full path to C++ compr
set(CMAKE_CXX_COMPILER 
/home/ken/arm-unknown-linux-gnueabi/bin/arm-unknown-linux-gnueabi-g++)

# Set the root directories for the find_* commands
# Configure CMake to search for headers and libraries only in those directories
set(CMAKE_FIND_ROOT_PATH
  /home/ken/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi
  /home/ken/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/include
  /home/ken/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/lib)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# By default, install stuff in the toolchain tree
set(CMAKE_INSTALL_PREFIX 
  /home/ken/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/usr CACHE STRING "Install path prefix, prepended onto 
install directories.")

# Compile with optimizations
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build")

# Configure ARGoS flags
set(ARGOS_BUILD_FOR epuck)
set(ARGOS_DOCUMENTATION OFF)
set(ARGOS_DYNAMIC_LIBRARY_LOADING OFF)
set(ARGOS_THREADSAFE_LOG OFF)
set(ARGOS_USE_DOUBLE OFF)

set(ARGOS_INCLUDE_DIRS /home/ken/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/usr/include)
#set(ARGOS_INCLUDE_DIRS /home/ken/depots/argos3/src /home/ken/depots/argos3/cross_build)
#set(ARGOS_LIBRARY_DIRS /home/ken/depots/argos3/cross_build/core)
set(ARGOS_LIBRARY_DIRS /home/ken/arm-unknown-linux-gnueabi/arm-unknown-linux-gnueabi/sysroot/usr/lib/argos3)
