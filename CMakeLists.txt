cmake_minimum_required(VERSION 3.10)

project(SmashShell CXX)

# Use C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    # InternalCommands
    Commands/AliasCommand.cpp
    Commands/BuiltInCommand.cpp
    Commands/CdCommand.cpp
    Commands/ChpromptCommand.cpp
    Commands/Command.cpp
    Commands/ForegroundCommand.cpp
    Commands/JobsCommand.cpp
    Commands/KillCommand.cpp
    Commands/PwdCommand.cpp
    Commands/QuitCommand.cpp
    Commands/ShowpidCommand.cpp
    Commands/UnAliasCommand.cpp
    Commands/UnsetenvCommand.cpp
    Commands/WatchprocCommand.cpp

    # SpecialCommands
    SpecialCommands/DiskUsageCommand.cpp
    SpecialCommands/PipeCommand.cpp
    SpecialCommands/RedirectionCommand.cpp
    SpecialCommands/WhoAmICommand.cpp

    # Core files
    ExternalCommand.cpp
    JobsList.cpp
    Signals.cpp
    SmallShell.cpp
    Smash.cpp        # <-- main()
    Utilities.cpp
)

# Create the executable
add_executable(smash ${SOURCES})

target_link_libraries(smash PRIVATE stdc++fs)

# Add warnings for safety
target_compile_options(smash PRIVATE -Wall -Wextra -Wpedantic)

# Add include directories for headers
target_include_directories(smash PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/Commands
    ${CMAKE_SOURCE_DIR}/SpecialCommands
)
