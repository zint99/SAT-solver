# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/joy/project/SAT-solver

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joy/project/SAT-solver/build

# Include any dependencies generated for this target.
include CMakeFiles/SatSolver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SatSolver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SatSolver.dir/flags.make

CMakeFiles/SatSolver.dir/main.cpp.o: CMakeFiles/SatSolver.dir/flags.make
CMakeFiles/SatSolver.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joy/project/SAT-solver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SatSolver.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SatSolver.dir/main.cpp.o -c /home/joy/project/SAT-solver/main.cpp

CMakeFiles/SatSolver.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SatSolver.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joy/project/SAT-solver/main.cpp > CMakeFiles/SatSolver.dir/main.cpp.i

CMakeFiles/SatSolver.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SatSolver.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joy/project/SAT-solver/main.cpp -o CMakeFiles/SatSolver.dir/main.cpp.s

CMakeFiles/SatSolver.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/SatSolver.dir/main.cpp.o.requires

CMakeFiles/SatSolver.dir/main.cpp.o.provides: CMakeFiles/SatSolver.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/SatSolver.dir/build.make CMakeFiles/SatSolver.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/SatSolver.dir/main.cpp.o.provides

CMakeFiles/SatSolver.dir/main.cpp.o.provides.build: CMakeFiles/SatSolver.dir/main.cpp.o


CMakeFiles/SatSolver.dir/DPLL.cpp.o: CMakeFiles/SatSolver.dir/flags.make
CMakeFiles/SatSolver.dir/DPLL.cpp.o: ../DPLL.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joy/project/SAT-solver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SatSolver.dir/DPLL.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SatSolver.dir/DPLL.cpp.o -c /home/joy/project/SAT-solver/DPLL.cpp

CMakeFiles/SatSolver.dir/DPLL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SatSolver.dir/DPLL.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joy/project/SAT-solver/DPLL.cpp > CMakeFiles/SatSolver.dir/DPLL.cpp.i

CMakeFiles/SatSolver.dir/DPLL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SatSolver.dir/DPLL.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joy/project/SAT-solver/DPLL.cpp -o CMakeFiles/SatSolver.dir/DPLL.cpp.s

CMakeFiles/SatSolver.dir/DPLL.cpp.o.requires:

.PHONY : CMakeFiles/SatSolver.dir/DPLL.cpp.o.requires

CMakeFiles/SatSolver.dir/DPLL.cpp.o.provides: CMakeFiles/SatSolver.dir/DPLL.cpp.o.requires
	$(MAKE) -f CMakeFiles/SatSolver.dir/build.make CMakeFiles/SatSolver.dir/DPLL.cpp.o.provides.build
.PHONY : CMakeFiles/SatSolver.dir/DPLL.cpp.o.provides

CMakeFiles/SatSolver.dir/DPLL.cpp.o.provides.build: CMakeFiles/SatSolver.dir/DPLL.cpp.o


# Object files for target SatSolver
SatSolver_OBJECTS = \
"CMakeFiles/SatSolver.dir/main.cpp.o" \
"CMakeFiles/SatSolver.dir/DPLL.cpp.o"

# External object files for target SatSolver
SatSolver_EXTERNAL_OBJECTS =

SatSolver: CMakeFiles/SatSolver.dir/main.cpp.o
SatSolver: CMakeFiles/SatSolver.dir/DPLL.cpp.o
SatSolver: CMakeFiles/SatSolver.dir/build.make
SatSolver: CMakeFiles/SatSolver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/joy/project/SAT-solver/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable SatSolver"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SatSolver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SatSolver.dir/build: SatSolver

.PHONY : CMakeFiles/SatSolver.dir/build

CMakeFiles/SatSolver.dir/requires: CMakeFiles/SatSolver.dir/main.cpp.o.requires
CMakeFiles/SatSolver.dir/requires: CMakeFiles/SatSolver.dir/DPLL.cpp.o.requires

.PHONY : CMakeFiles/SatSolver.dir/requires

CMakeFiles/SatSolver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SatSolver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SatSolver.dir/clean

CMakeFiles/SatSolver.dir/depend:
	cd /home/joy/project/SAT-solver/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joy/project/SAT-solver /home/joy/project/SAT-solver /home/joy/project/SAT-solver/build /home/joy/project/SAT-solver/build /home/joy/project/SAT-solver/build/CMakeFiles/SatSolver.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SatSolver.dir/depend

