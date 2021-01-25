copy premake5_GLFW.lua Parrot\vendor\GLFW\premake5.lua
call vendor\premake\premake5.exe vs2019
del Parrot\vendor\GLFW\premake5.lua
PAUSE