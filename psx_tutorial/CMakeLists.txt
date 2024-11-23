cmake_minimum_required(VERSION 3.21)

project(
	psx_tutorial
	LANGUAGES    C ASM
	VERSION      1.0.0
	DESCRIPTION  "Taylor's PSn00bSDK test"
	HOMEPAGE_URL "http://lameguy64.net/?page=psn00bsdk"
)

file(GLOB _sources *.c)
psn00bsdk_add_executable(psx_tutorial GPREL ${_sources})

psn00bsdk_target_incbin(psx_tutorial PRIVATE garden64_1_tim garden64_1.tim)

install(FILES ${PROJECT_BINARY_DIR}/psx_tutorial.exe TYPE BIN)

# psn00bsdk_add_cd_image(psx_tutorial_iso psx_tutorial iso.xml DEPENDS psx_tutorial system.cnf)