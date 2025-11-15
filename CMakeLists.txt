cmake_minimum_required(VERSION 3.25)

project(kbdt VERSION 1.0.0 LANGUAGES C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(KBDT_MSG_PREFIX "[KBDT]")

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR "${KBDT_MSG_PREFIX} In-source builds are not allowed. Please make a new directory (e.g. build) and run CMake from there.")
endif()

if(NOT DEFINED KBDT_MASTER_PROJECT)
    if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
        set(KBDT_MASTER_PROJECT ON)
    else()
        set(KBDT_MASTER_PROJECT OFF)
    endif()
endif()

option(KBDT_BUILD_SHARED "Build shared library" OFF)
if(KBDT_BUILD_SHARED)
    message(STATUS "${KBDT_MSG_PREFIX} Shared library will be built.")
else()
    message(STATUS "${KBDT_MSG_PREFIX} Static library will be built.")
endif()

option(KBDT_BUILD_WITH_UTILITY "Build with utility" ON)
option(KBDT_BUILD_EXAMPLE "Build example" ${KBDT_MASTER_PROJECT})

set(SRC)
if(WIN32)
    file(GLOB_RECURSE SRC ${SRC} src/win/*.c)
elseif(APPLE)
    file(GLOB_RECURSE SRC ${SRC} src/mac/*.c)
elseif(LINUX)
    file(GLOB_RECURSE SRC ${SRC} src/linux/*.c)
endif()

file(GLOB SRC ${SRC} src/*.c)
if(KBDT_BUILD_WITH_UTILITY)
    file(GLOB SRC ${SRC} src/utility/utility.c)
    if(WIN32)
        file(GLOB SRC ${SRC} src/utility/utility_win.c)
    elseif(APPLE)
        file(GLOB SRC ${SRC} src/utility/utility_mac.c)
    elseif(LINUX)
        file(GLOB SRC ${SRC} src/utility/utility_linux.c)
    endif()
endif()

if(KBDT_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED ${SRC})
    target_compile_definitions(${PROJECT_NAME} PUBLIC KBDT_BUILD_SHARED)
else()
    add_library(${PROJECT_NAME} STATIC ${SRC})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${PROJECT_NAME}/include>
)

if(WIN32)
    message(STATUS "${KBDT_MSG_PREFIX} Windows platform detected.")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
endif()

if(APPLE)
    message(STATUS "${KBDT_MSG_PREFIX} Apple platform detected.")

    find_library(COREFOUNDATION_LIBRARY CoreFoundation)
    if(COREFOUNDATION_LIBRARY)
        message(STATUS "${KBDT_MSG_PREFIX} Found CoreFoundation: ${COREFOUNDATION_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${COREFOUNDATION_LIBRARY})
    else()
        message(FATAL_ERROR "${KBDT_MSG_PREFIX} Not found CoreFoundation.")
    endif()

    find_library(CARBON_LIBRARY Carbon)
    if(CARBON_LIBRARY)
        message(STATUS "{KBDT_MSG_PREFIX} Found Carbon: ${CARBON_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${CARBON_LIBRARY})
    else()
        message(FATAL_ERROR "{KBDT_MSG_PREFIX} Not found Carbon.")
    endif()

    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    if(COREGRAPHICS_LIBRARY)
        message(STATUS "${KBDT_MSG_PREFIX} Found CoreGraphics: ${COREGRAPHICS_LIBRARY}.")
        target_link_libraries(${PROJECT_NAME} PRIVATE ${COREGRAPHICS_LIBRARY})
    else()
        message(FATAL_ERROR "${KBDT_MSG_PREFIX} Not found CoreGraphics.")
    endif()
endif()

if(LINUX)
    message(STATUS "${KBDT_MSG_PREFIX} Linux platform detected.")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX d)

find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}
    ARCHIVE DESTINATION ${PROJECT_NAME}/lib
    LIBRARY DESTINATION ${PROJECT_NAME}/bin
    RUNTIME DESTINATION ${PROJECT_NAME}/bin
)

install(DIRECTORY include doc DESTINATION ${PROJECT_NAME})
install(FILES LICENSE DESTINATION ${PROJECT_NAME})

install(
    EXPORT ${PROJECT_NAME}
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${PROJECT_NAME}/lib/cmake
)

set(INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/include)
set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/${PROJECT_NAME}/lib)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    ${PROJECT_NAME}Config.cmake.in
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    PATH_VARS INCLUDE_DIRS LIB_DIR
    INSTALL_DESTINATION ${PROJECT_NAME}/lib/cmake
)

install(
    FILES
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${PROJECT_NAME}/lib/cmake
)

if(KBDT_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
