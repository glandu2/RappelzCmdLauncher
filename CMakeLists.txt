project(RappelzCmdLauncher)

add_executable(RappelzCmdLauncher WIN32 main.c)

#SFrame won't inherit the event handle if RappelzCmdLauncher is not started with administrator rights
set_target_properties(RappelzCmdLauncher PROPERTIES LINK_FLAGS "/MANIFESTUAC:\"level='requireAdministrator' uiAccess='false'\"")
