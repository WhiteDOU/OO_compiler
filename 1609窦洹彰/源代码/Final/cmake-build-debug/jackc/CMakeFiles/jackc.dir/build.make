# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/white/Downloads/Final

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/white/Downloads/Final/cmake-build-debug

# Include any dependencies generated for this target.
include jackc/CMakeFiles/jackc.dir/depend.make

# Include the progress variables for this target.
include jackc/CMakeFiles/jackc.dir/progress.make

# Include the compile flags for this target's objects.
include jackc/CMakeFiles/jackc.dir/flags.make

jackc/CMakeFiles/jackc.dir/driver/jackc.cpp.o: jackc/CMakeFiles/jackc.dir/flags.make
jackc/CMakeFiles/jackc.dir/driver/jackc.cpp.o: ../jackc/driver/jackc.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/white/Downloads/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object jackc/CMakeFiles/jackc.dir/driver/jackc.cpp.o"
	cd /Users/white/Downloads/Final/cmake-build-debug/jackc && /Library/Developer/CommandLineTools/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jackc.dir/driver/jackc.cpp.o -c /Users/white/Downloads/Final/jackc/driver/jackc.cpp

jackc/CMakeFiles/jackc.dir/driver/jackc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jackc.dir/driver/jackc.cpp.i"
	cd /Users/white/Downloads/Final/cmake-build-debug/jackc && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/white/Downloads/Final/jackc/driver/jackc.cpp > CMakeFiles/jackc.dir/driver/jackc.cpp.i

jackc/CMakeFiles/jackc.dir/driver/jackc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jackc.dir/driver/jackc.cpp.s"
	cd /Users/white/Downloads/Final/cmake-build-debug/jackc && /Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/white/Downloads/Final/jackc/driver/jackc.cpp -o CMakeFiles/jackc.dir/driver/jackc.cpp.s

# Object files for target jackc
jackc_OBJECTS = \
"CMakeFiles/jackc.dir/driver/jackc.cpp.o"

# External object files for target jackc
jackc_EXTERNAL_OBJECTS =

jackc/jackc: jackc/CMakeFiles/jackc.dir/driver/jackc.cpp.o
jackc/jackc: jackc/CMakeFiles/jackc.dir/build.make
jackc/jackc: jackc/libjackclib.a
jackc/jackc: jackc/CMakeFiles/jackc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/white/Downloads/Final/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable jackc"
	cd /Users/white/Downloads/Final/cmake-build-debug/jackc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jackc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jackc/CMakeFiles/jackc.dir/build: jackc/jackc

.PHONY : jackc/CMakeFiles/jackc.dir/build

jackc/CMakeFiles/jackc.dir/clean:
	cd /Users/white/Downloads/Final/cmake-build-debug/jackc && $(CMAKE_COMMAND) -P CMakeFiles/jackc.dir/cmake_clean.cmake
.PHONY : jackc/CMakeFiles/jackc.dir/clean

jackc/CMakeFiles/jackc.dir/depend:
	cd /Users/white/Downloads/Final/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/white/Downloads/Final /Users/white/Downloads/Final/jackc /Users/white/Downloads/Final/cmake-build-debug /Users/white/Downloads/Final/cmake-build-debug/jackc /Users/white/Downloads/Final/cmake-build-debug/jackc/CMakeFiles/jackc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jackc/CMakeFiles/jackc.dir/depend

