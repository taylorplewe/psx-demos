cmake_minimum_required(VERSION 3.21)

project(
	textured_poly
	LANGUAGES    C ASM
	VERSION      1.0.0
	DESCRIPTION  "Texturing and rotating a poly"
	HOMEPAGE_URL "https://github.com/taylorplewe"
)

file(GLOB _sources src/*.c)
psn00bsdk_add_executable(textured_poly GPREL ${_sources})
psn00bsdk_target_incbin(textured_poly PRIVATE cooldog_tim cooldog.tim)

install(FILES ${PROJECT_BINARY_DIR}/textured_poly.exe TYPE BIN)

# psn00bsdk_add_cd_image(textured_poly_iso textured_poly iso.xml DEPENDS textured_poly system.cnf)