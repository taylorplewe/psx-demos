cmake_minimum_required(VERSION 3.21)

project(
	3d_cube
	LANGUAGES C
	VERSION 1.0.0
	DESCRIPTION "Display a rotating 3d cube"
)

file(GLOB _sources src/*.c)

psn00bsdk_add_executable(3d_cube GPREL ${_sources})

install(FILES ${PROJECT_BINARY_DIR}/3d_cube.exe TYPE BIN)