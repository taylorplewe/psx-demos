cmake_minimum_required(VERSION 3.21)

project(
	just_the_ship
	LANGUAGES    C ASM
	VERSION      1.0.0
	DESCRIPTION  "Just the ship (triangle) part of the psn00bsdk tutorial"
	HOMEPAGE_URL "https://github.com/taylorplewe"
)

file(GLOB _sources *.c)
psn00bsdk_add_executable(just_the_ship GPREL ${_sources})

install(FILES ${PROJECT_BINARY_DIR}/just_the_ship.exe TYPE BIN)

# psn00bsdk_add_cd_image(just_the_ship_iso just_the_ship iso.xml DEPENDS just_the_ship system.cnf)