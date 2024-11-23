cmake_minimum_required(VERSION 3.21)

project(
	asteroids_ship
	LANGUAGES    C ASM
	VERSION      1.0.0
	DESCRIPTION  "Just the ship (triangle) part of the psn00bsdk tutorial"
	HOMEPAGE_URL "https://github.com/taylorplewe"
)

file(GLOB _sources src/*.c)
psn00bsdk_add_executable(asteroids_ship GPREL ${_sources})

install(FILES ${PROJECT_BINARY_DIR}/asteroids_ship.exe TYPE BIN)

# psn00bsdk_add_cd_image(asteroids_ship_iso asteroids_ship iso.xml DEPENDS asteroids_ship system.cnf)
