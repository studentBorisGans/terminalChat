# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.30

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
CMAKE_COMMAND = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/Cellar/cmake/3.30.3/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/borisgans/IE/programming/terminalChat

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/borisgans/IE/programming/terminalChat/src

# Include any dependencies generated for this target.
include CMakeFiles/Server.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Server.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Server.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Server.dir/flags.make

CMakeFiles/Server.dir/server.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/server.c.o: server.c
CMakeFiles/Server.dir/server.c.o: CMakeFiles/Server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/borisgans/IE/programming/terminalChat/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Server.dir/server.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Server.dir/server.c.o -MF CMakeFiles/Server.dir/server.c.o.d -o CMakeFiles/Server.dir/server.c.o -c /Users/borisgans/IE/programming/terminalChat/src/server.c

CMakeFiles/Server.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Server.dir/server.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/borisgans/IE/programming/terminalChat/src/server.c > CMakeFiles/Server.dir/server.c.i

CMakeFiles/Server.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Server.dir/server.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/borisgans/IE/programming/terminalChat/src/server.c -o CMakeFiles/Server.dir/server.c.s

CMakeFiles/Server.dir/network_utils.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/network_utils.c.o: network_utils.c
CMakeFiles/Server.dir/network_utils.c.o: CMakeFiles/Server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/borisgans/IE/programming/terminalChat/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Server.dir/network_utils.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Server.dir/network_utils.c.o -MF CMakeFiles/Server.dir/network_utils.c.o.d -o CMakeFiles/Server.dir/network_utils.c.o -c /Users/borisgans/IE/programming/terminalChat/src/network_utils.c

CMakeFiles/Server.dir/network_utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Server.dir/network_utils.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/borisgans/IE/programming/terminalChat/src/network_utils.c > CMakeFiles/Server.dir/network_utils.c.i

CMakeFiles/Server.dir/network_utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Server.dir/network_utils.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/borisgans/IE/programming/terminalChat/src/network_utils.c -o CMakeFiles/Server.dir/network_utils.c.s

CMakeFiles/Server.dir/chat_room.c.o: CMakeFiles/Server.dir/flags.make
CMakeFiles/Server.dir/chat_room.c.o: chat_room.c
CMakeFiles/Server.dir/chat_room.c.o: CMakeFiles/Server.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/borisgans/IE/programming/terminalChat/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Server.dir/chat_room.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Server.dir/chat_room.c.o -MF CMakeFiles/Server.dir/chat_room.c.o.d -o CMakeFiles/Server.dir/chat_room.c.o -c /Users/borisgans/IE/programming/terminalChat/src/chat_room.c

CMakeFiles/Server.dir/chat_room.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/Server.dir/chat_room.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/borisgans/IE/programming/terminalChat/src/chat_room.c > CMakeFiles/Server.dir/chat_room.c.i

CMakeFiles/Server.dir/chat_room.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/Server.dir/chat_room.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/borisgans/IE/programming/terminalChat/src/chat_room.c -o CMakeFiles/Server.dir/chat_room.c.s

# Object files for target Server
Server_OBJECTS = \
"CMakeFiles/Server.dir/server.c.o" \
"CMakeFiles/Server.dir/network_utils.c.o" \
"CMakeFiles/Server.dir/chat_room.c.o"

# External object files for target Server
Server_EXTERNAL_OBJECTS =

bin/Server: CMakeFiles/Server.dir/server.c.o
bin/Server: CMakeFiles/Server.dir/network_utils.c.o
bin/Server: CMakeFiles/Server.dir/chat_room.c.o
bin/Server: CMakeFiles/Server.dir/build.make
bin/Server: CMakeFiles/Server.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/borisgans/IE/programming/terminalChat/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable bin/Server"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Server.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Server.dir/build: bin/Server
.PHONY : CMakeFiles/Server.dir/build

CMakeFiles/Server.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Server.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Server.dir/clean

CMakeFiles/Server.dir/depend:
	cd /Users/borisgans/IE/programming/terminalChat/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/borisgans/IE/programming/terminalChat /Users/borisgans/IE/programming/terminalChat /Users/borisgans/IE/programming/terminalChat/src /Users/borisgans/IE/programming/terminalChat/src /Users/borisgans/IE/programming/terminalChat/src/CMakeFiles/Server.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Server.dir/depend
