cmake_minimum_required(VERSION 3.25)

project(quick_input_pi LANGUAGES C)

if(NOT TARGET kbdt)
    find_package(kbdt REQUIRED)
endif()

if(NOT KBDT_WITH_UTILS)
    message(FATAL_ERROR "${KBDT_MSG_PREFIX} Can't build example with no utilitys.")
endif()

add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} PRIVATE kbdt::kbdt)

install(
    TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION example
)
