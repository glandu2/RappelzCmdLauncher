# RappelzCmdLauncher
Usage
---
Launch client without launcher

This tool must be used in a .bat file like this:
`RappelzCmdLauncher.exe SFrame.exe /auth_ip:127.0.0.1 /locale:windows-1252 /country:FR`

RappelzCmdLauncher must be run with administrator rights else SFrame either won't run or will show the error "page 4(0xf0, 6)".

Compilation
---
To compile, either use cmake:
```
mkdir build
cd build
cmake .. -G "NMake Makefiles"
nmake
```

Or create a project to compile `main.c` as a GUI program.

