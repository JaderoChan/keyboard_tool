cmake_minimum_required(VERSION 3.25)

project(KBDTool-Example LANGUAGES C)
message(STATUS "${KBDT_MSG_PREFIX} Example will be builded.")

if(NOT TARGET kbdt)
    find_package(kbdt REQUIRED)
endif()
link_libraries(kbdt::kbdt)

if(NOT KBDT_BUILD_WITH_UTILITY)
    message(FATAL_ERROR "${KBDT_MSG_PREFIX} Can't build example with no utility.")
endif()

option(KBDT_EXAMPLE_BUILD_ECHO "Build echo example" ON)
option(KBDT_EXAMPLE_BUILD_QUICK_INPUT_PI "Build quick input pi example" ON)

if(KBDT_EXAMPLE_BUILD_ECHO)
    add_subdirectory(echo)
endif()
if(KBDT_EXAMPLE_BUILD_QUICK_INPUT_PI)
    add_subdirectory(quick_input_pi)
endif()
