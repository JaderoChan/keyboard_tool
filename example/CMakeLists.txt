cmake_minimum_required(VERSION 3.25)

project(kbdt_examples LANGUAGES C)
message(STATUS "${KBDT_MSG_PREFIX} Example will be builded.")

option(KBDT_EXAMPLE_BUILD_ECHO "Build echo example" ON)
option(KBDT_EXAMPLE_BUILD_QUICK_INPUT_PI "Build quick input pi example" ON)

if(KBDT_EXAMPLE_BUILD_ECHO)
    add_subdirectory(echo)
endif()

if(KBDT_EXAMPLE_BUILD_QUICK_INPUT_PI)
    add_subdirectory(quick_input_pi)
endif()
