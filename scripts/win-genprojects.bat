@echo off
pushd ..\
xcopy premakes\GLFW\premake5.lua Engine\vendor\GLFW
xcopy premakes\imgui\premake5.lua Engine\vendor\imgui
xcopy premakes\yaml-cpp\premake5.lua Engine\vendor\yaml-cpp
call scripts\premake5.exe vs2022
del Engine\vendor\GLFW\premake5.lua
del Engine\vendor\imgui\premake5.lua
del Engine\vendor\yaml-cpp\premake5.lua
popd
PAUSE