cmake_minimum_required(VERSION 3.12)

project(MSVCPthread LANGUAGES C)
message(STATUS "${KBDT_MSG_PREFIX} MSVC Pthread will be builded.")

add_library(pthread STATIC pthread.c)
set_target_properties(pthread PROPERTIES DEBUG_POSTFIX d)
add_library(pthread::pthread ALIAS pthread)
