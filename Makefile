# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/c/Users/albyr/Documents/argos-python

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/albyr/Documents/argos-python

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/albyr/Documents/argos-python/CMakeFiles /mnt/c/Users/albyr/Documents/argos-python/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /mnt/c/Users/albyr/Documents/argos-python/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean

.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named py_controller_interface

# Build rule for target.
py_controller_interface: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 py_controller_interface
.PHONY : py_controller_interface

# fast build rule for target.
py_controller_interface/fast:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/build
.PHONY : py_controller_interface/fast

py_actusensor_wrapper_epuck.o: py_actusensor_wrapper_epuck.cpp.o

.PHONY : py_actusensor_wrapper_epuck.o

# target to build an object file
py_actusensor_wrapper_epuck.cpp.o:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_epuck.cpp.o
.PHONY : py_actusensor_wrapper_epuck.cpp.o

py_actusensor_wrapper_epuck.i: py_actusensor_wrapper_epuck.cpp.i

.PHONY : py_actusensor_wrapper_epuck.i

# target to preprocess a source file
py_actusensor_wrapper_epuck.cpp.i:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_epuck.cpp.i
.PHONY : py_actusensor_wrapper_epuck.cpp.i

py_actusensor_wrapper_epuck.s: py_actusensor_wrapper_epuck.cpp.s

.PHONY : py_actusensor_wrapper_epuck.s

# target to generate assembly for a file
py_actusensor_wrapper_epuck.cpp.s:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_epuck.cpp.s
.PHONY : py_actusensor_wrapper_epuck.cpp.s

py_actusensor_wrapper_footbot.o: py_actusensor_wrapper_footbot.cpp.o

.PHONY : py_actusensor_wrapper_footbot.o

# target to build an object file
py_actusensor_wrapper_footbot.cpp.o:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_footbot.cpp.o
.PHONY : py_actusensor_wrapper_footbot.cpp.o

py_actusensor_wrapper_footbot.i: py_actusensor_wrapper_footbot.cpp.i

.PHONY : py_actusensor_wrapper_footbot.i

# target to preprocess a source file
py_actusensor_wrapper_footbot.cpp.i:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_footbot.cpp.i
.PHONY : py_actusensor_wrapper_footbot.cpp.i

py_actusensor_wrapper_footbot.s: py_actusensor_wrapper_footbot.cpp.s

.PHONY : py_actusensor_wrapper_footbot.s

# target to generate assembly for a file
py_actusensor_wrapper_footbot.cpp.s:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_footbot.cpp.s
.PHONY : py_actusensor_wrapper_footbot.cpp.s

py_actusensor_wrapper_generic.o: py_actusensor_wrapper_generic.cpp.o

.PHONY : py_actusensor_wrapper_generic.o

# target to build an object file
py_actusensor_wrapper_generic.cpp.o:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_generic.cpp.o
.PHONY : py_actusensor_wrapper_generic.cpp.o

py_actusensor_wrapper_generic.i: py_actusensor_wrapper_generic.cpp.i

.PHONY : py_actusensor_wrapper_generic.i

# target to preprocess a source file
py_actusensor_wrapper_generic.cpp.i:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_generic.cpp.i
.PHONY : py_actusensor_wrapper_generic.cpp.i

py_actusensor_wrapper_generic.s: py_actusensor_wrapper_generic.cpp.s

.PHONY : py_actusensor_wrapper_generic.s

# target to generate assembly for a file
py_actusensor_wrapper_generic.cpp.s:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_actusensor_wrapper_generic.cpp.s
.PHONY : py_actusensor_wrapper_generic.cpp.s

py_controller.o: py_controller.cpp.o

.PHONY : py_controller.o

# target to build an object file
py_controller.cpp.o:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_controller.cpp.o
.PHONY : py_controller.cpp.o

py_controller.i: py_controller.cpp.i

.PHONY : py_controller.i

# target to preprocess a source file
py_controller.cpp.i:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_controller.cpp.i
.PHONY : py_controller.cpp.i

py_controller.s: py_controller.cpp.s

.PHONY : py_controller.s

# target to generate assembly for a file
py_controller.cpp.s:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_controller.cpp.s
.PHONY : py_controller.cpp.s

py_wrapper.o: py_wrapper.cpp.o

.PHONY : py_wrapper.o

# target to build an object file
py_wrapper.cpp.o:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_wrapper.cpp.o
.PHONY : py_wrapper.cpp.o

py_wrapper.i: py_wrapper.cpp.i

.PHONY : py_wrapper.i

# target to preprocess a source file
py_wrapper.cpp.i:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_wrapper.cpp.i
.PHONY : py_wrapper.cpp.i

py_wrapper.s: py_wrapper.cpp.s

.PHONY : py_wrapper.s

# target to generate assembly for a file
py_wrapper.cpp.s:
	$(MAKE) -f CMakeFiles/py_controller_interface.dir/build.make CMakeFiles/py_controller_interface.dir/py_wrapper.cpp.s
.PHONY : py_wrapper.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... py_controller_interface"
	@echo "... py_actusensor_wrapper_epuck.o"
	@echo "... py_actusensor_wrapper_epuck.i"
	@echo "... py_actusensor_wrapper_epuck.s"
	@echo "... py_actusensor_wrapper_footbot.o"
	@echo "... py_actusensor_wrapper_footbot.i"
	@echo "... py_actusensor_wrapper_footbot.s"
	@echo "... py_actusensor_wrapper_generic.o"
	@echo "... py_actusensor_wrapper_generic.i"
	@echo "... py_actusensor_wrapper_generic.s"
	@echo "... py_controller.o"
	@echo "... py_controller.i"
	@echo "... py_controller.s"
	@echo "... py_wrapper.o"
	@echo "... py_wrapper.i"
	@echo "... py_wrapper.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

