cmake_minimum_required(VERSION 3.21)

project(
	bg_scrolling
	LANGUAGES C
	VERSION 1.0.0
	DESCRIPTION "Scrolling a tileset background"
)

file(GLOB _sources src/*.c)

psn00bsdk_add_executable(bg_scrolling GPREL ${_sources})

install(FILES ${PROJECT_BINARY_DIR}/bg_scrolling.exe TYPE BIN)