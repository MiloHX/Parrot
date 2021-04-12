copy premake5_GLFW.lua Parrot\vendor\GLFW\premake5.lua
copy premake5_Glad.lua Parrot\vendor\Glad\premake5.lua
copy premake5_imgui.lua Parrot\vendor\imgui\premake5.lua
copy premake5_ImGuizmo.lua Parrot\vendor\ImGuizmo\premake5.lua
copy premake5_yaml_cpp.lua Parrot\vendor\yaml_cpp\premake5.lua
call vendor\premake\premake5.exe vs2019
del Parrot\vendor\yaml_cpp\premake5.lua
del Parrot\vendor\ImGuizmo\premake5.lua
del Parrot\vendor\imgui\premake5.lua
del Parrot\vendor\Glad\premake5.lua
del Parrot\vendor\GLFW\premake5.lua
PAUSE