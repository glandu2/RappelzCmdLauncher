cmake_minimum_required(VERSION 3.13)
project(RappelzCmdLauncher VERSION 1.1)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(CPACK_GENERATOR "ZIP")
set(CPACK_STRIP_FILES ON)
set(CPACK_PACKAGE_NAME "${CMAKE_PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Glandu2")

add_executable(RappelzCmdLauncher WIN32 main.c)

#SFrame won't inherit the event handle if RappelzCmdLauncher is not started with administrator rights
set_target_properties(RappelzCmdLauncher PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")

install(TARGETS RappelzCmdLauncher DESTINATION ./)

include(CPack OPTIONAL)

message("Package name: ${CPACK_PACKAGE_FILE_NAME}")
