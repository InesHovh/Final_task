# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/Desktop/Final_task

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/Desktop/Final_task/cmake-build-debug

# Include any dependencies generated for this target.
include Server/CMakeFiles/Server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include Server/CMakeFiles/Server.dir/compiler_depend.make

# Include the progress variables for this target.
include Server/CMakeFiles/Server.dir/progress.make

# Include the compile flags for this target's objects.
include Server/CMakeFiles/Server.dir/flags.make

Server/CMakeFiles/Server.dir/Server.cpp.o: Server/CMakeFiles/Server.dir/flags.make
Server/CMakeFiles/Server.dir/Server.cpp.o: ../Server/Server.cpp
Server/CMakeFiles/Server.dir/Server.cpp.o: Server/CMakeFiles/Server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/user/Desktop/Final_task/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Server/CMakeFiles/Server.dir/Server.cpp.o"
	cd /home/user/Desktop/Final_task/cmake-build-debug/Server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT Server/CMakeFiles/Server.dir/Server.cpp.o -MF CMakeFiles/Server.dir/Server.cpp.o.d -o CMakeFiles/Server.dir/Server.cpp.o -c /home/user/Desktop/Final_task/Server/Server.cpp

Server/CMakeFiles/Server.dir/Server.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Server.dir/Server.cpp.i"
	cd /home/user/Desktop/Final_task/cmake-build-debug/Server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/user/Desktop/Final_task/Server/Server.cpp > CMakeFiles/Server.dir/Server.cpp.i

Server/CMakeFiles/Server.dir/Server.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Server.dir/Server.cpp.s"
	cd /home/user/Desktop/Final_task/cmake-build-debug/Server && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/user/Desktop/Final_task/Server/Server.cpp -o CMakeFiles/Server.dir/Server.cpp.s

# Object files for target Server
Server_OBJECTS = \
"CMakeFiles/Server.dir/Server.cpp.o"

# External object files for target Server
Server_EXTERNAL_OBJECTS =

Server/Server: Server/CMakeFiles/Server.dir/Server.cpp.o
Server/Server: Server/CMakeFiles/Server.dir/build.make
Server/Server: Server/CMakeFiles/Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/user/Desktop/Final_task/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable Server"
	cd /home/user/Desktop/Final_task/cmake-build-debug/Server && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Server/CMakeFiles/Server.dir/build: Server/Server
.PHONY : Server/CMakeFiles/Server.dir/build

Server/CMakeFiles/Server.dir/clean:
	cd /home/user/Desktop/Final_task/cmake-build-debug/Server && $(CMAKE_COMMAND) -P CMakeFiles/Server.dir/cmake_clean.cmake
.PHONY : Server/CMakeFiles/Server.dir/clean

Server/CMakeFiles/Server.dir/depend:
	cd /home/user/Desktop/Final_task/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/user/Desktop/Final_task /home/user/Desktop/Final_task/Server /home/user/Desktop/Final_task/cmake-build-debug /home/user/Desktop/Final_task/cmake-build-debug/Server /home/user/Desktop/Final_task/cmake-build-debug/Server/CMakeFiles/Server.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Server/CMakeFiles/Server.dir/depend

