# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.24

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2022.3.2\bin\cmake\win\x64\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2022.3.2\bin\cmake\win\x64\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\uriel\CLionProjects\Assembler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Maman14.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Maman14.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Maman14.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Maman14.dir/flags.make

CMakeFiles/Maman14.dir/error_print.c.obj: CMakeFiles/Maman14.dir/flags.make
CMakeFiles/Maman14.dir/error_print.c.obj: C:/Users/uriel/CLionProjects/Assembler/error_print.c
CMakeFiles/Maman14.dir/error_print.c.obj: CMakeFiles/Maman14.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Maman14.dir/error_print.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Maman14.dir/error_print.c.obj -MF CMakeFiles\Maman14.dir\error_print.c.obj.d -o CMakeFiles\Maman14.dir\error_print.c.obj -c C:\Users\uriel\CLionProjects\Assembler\error_print.c

CMakeFiles/Maman14.dir/error_print.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Maman14.dir/error_print.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\uriel\CLionProjects\Assembler\error_print.c > CMakeFiles\Maman14.dir\error_print.c.i

CMakeFiles/Maman14.dir/error_print.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Maman14.dir/error_print.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\uriel\CLionProjects\Assembler\error_print.c -o CMakeFiles\Maman14.dir\error_print.c.s

CMakeFiles/Maman14.dir/pre_assembler.c.obj: CMakeFiles/Maman14.dir/flags.make
CMakeFiles/Maman14.dir/pre_assembler.c.obj: C:/Users/uriel/CLionProjects/Assembler/pre_assembler.c
CMakeFiles/Maman14.dir/pre_assembler.c.obj: CMakeFiles/Maman14.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Maman14.dir/pre_assembler.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Maman14.dir/pre_assembler.c.obj -MF CMakeFiles\Maman14.dir\pre_assembler.c.obj.d -o CMakeFiles\Maman14.dir\pre_assembler.c.obj -c C:\Users\uriel\CLionProjects\Assembler\pre_assembler.c

CMakeFiles/Maman14.dir/pre_assembler.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Maman14.dir/pre_assembler.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\uriel\CLionProjects\Assembler\pre_assembler.c > CMakeFiles\Maman14.dir\pre_assembler.c.i

CMakeFiles/Maman14.dir/pre_assembler.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Maman14.dir/pre_assembler.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\uriel\CLionProjects\Assembler\pre_assembler.c -o CMakeFiles\Maman14.dir\pre_assembler.c.s

CMakeFiles/Maman14.dir/hash_table.c.obj: CMakeFiles/Maman14.dir/flags.make
CMakeFiles/Maman14.dir/hash_table.c.obj: C:/Users/uriel/CLionProjects/Assembler/hash_table.c
CMakeFiles/Maman14.dir/hash_table.c.obj: CMakeFiles/Maman14.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Maman14.dir/hash_table.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Maman14.dir/hash_table.c.obj -MF CMakeFiles\Maman14.dir\hash_table.c.obj.d -o CMakeFiles\Maman14.dir\hash_table.c.obj -c C:\Users\uriel\CLionProjects\Assembler\hash_table.c

CMakeFiles/Maman14.dir/hash_table.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Maman14.dir/hash_table.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\uriel\CLionProjects\Assembler\hash_table.c > CMakeFiles\Maman14.dir\hash_table.c.i

CMakeFiles/Maman14.dir/hash_table.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Maman14.dir/hash_table.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\uriel\CLionProjects\Assembler\hash_table.c -o CMakeFiles\Maman14.dir\hash_table.c.s

CMakeFiles/Maman14.dir/input_validation.c.obj: CMakeFiles/Maman14.dir/flags.make
CMakeFiles/Maman14.dir/input_validation.c.obj: C:/Users/uriel/CLionProjects/Assembler/input_validation.c
CMakeFiles/Maman14.dir/input_validation.c.obj: CMakeFiles/Maman14.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/Maman14.dir/input_validation.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Maman14.dir/input_validation.c.obj -MF CMakeFiles\Maman14.dir\input_validation.c.obj.d -o CMakeFiles\Maman14.dir\input_validation.c.obj -c C:\Users\uriel\CLionProjects\Assembler\input_validation.c

CMakeFiles/Maman14.dir/input_validation.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Maman14.dir/input_validation.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\uriel\CLionProjects\Assembler\input_validation.c > CMakeFiles\Maman14.dir\input_validation.c.i

CMakeFiles/Maman14.dir/input_validation.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Maman14.dir/input_validation.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\uriel\CLionProjects\Assembler\input_validation.c -o CMakeFiles\Maman14.dir\input_validation.c.s

CMakeFiles/Maman14.dir/functions.c.obj: CMakeFiles/Maman14.dir/flags.make
CMakeFiles/Maman14.dir/functions.c.obj: C:/Users/uriel/CLionProjects/Assembler/functions.c
CMakeFiles/Maman14.dir/functions.c.obj: CMakeFiles/Maman14.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building C object CMakeFiles/Maman14.dir/functions.c.obj"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/Maman14.dir/functions.c.obj -MF CMakeFiles\Maman14.dir\functions.c.obj.d -o CMakeFiles\Maman14.dir\functions.c.obj -c C:\Users\uriel\CLionProjects\Assembler\functions.c

CMakeFiles/Maman14.dir/functions.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Maman14.dir/functions.c.i"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\uriel\CLionProjects\Assembler\functions.c > CMakeFiles\Maman14.dir\functions.c.i

CMakeFiles/Maman14.dir/functions.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Maman14.dir/functions.c.s"
	C:\PROGRA~1\JETBRA~1\CLION2~1.2\bin\mingw\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\uriel\CLionProjects\Assembler\functions.c -o CMakeFiles\Maman14.dir\functions.c.s

# Object files for target Maman14
Maman14_OBJECTS = \
"CMakeFiles/Maman14.dir/error_print.c.obj" \
"CMakeFiles/Maman14.dir/pre_assembler.c.obj" \
"CMakeFiles/Maman14.dir/hash_table.c.obj" \
"CMakeFiles/Maman14.dir/input_validation.c.obj" \
"CMakeFiles/Maman14.dir/functions.c.obj"

# External object files for target Maman14
Maman14_EXTERNAL_OBJECTS =

Maman14.exe: CMakeFiles/Maman14.dir/error_print.c.obj
Maman14.exe: CMakeFiles/Maman14.dir/pre_assembler.c.obj
Maman14.exe: CMakeFiles/Maman14.dir/hash_table.c.obj
Maman14.exe: CMakeFiles/Maman14.dir/input_validation.c.obj
Maman14.exe: CMakeFiles/Maman14.dir/functions.c.obj
Maman14.exe: CMakeFiles/Maman14.dir/build.make
Maman14.exe: CMakeFiles/Maman14.dir/linklibs.rsp
Maman14.exe: CMakeFiles/Maman14.dir/objects1.rsp
Maman14.exe: CMakeFiles/Maman14.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking C executable Maman14.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\Maman14.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Maman14.dir/build: Maman14.exe
.PHONY : CMakeFiles/Maman14.dir/build

CMakeFiles/Maman14.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Maman14.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Maman14.dir/clean

CMakeFiles/Maman14.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\uriel\CLionProjects\Assembler C:\Users\uriel\CLionProjects\Assembler C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug C:\Users\uriel\CLionProjects\Assembler\cmake-build-debug\CMakeFiles\Maman14.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Maman14.dir/depend
