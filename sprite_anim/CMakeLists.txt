cmake_minimum_required(VERSION 3.21)

project(
	sprite_anim
	LANGUAGES    C ASM
	VERSION      1.0.0
	DESCRIPTION  "Texturing and rotating a poly"
	HOMEPAGE_URL "https://github.com/taylorplewe"
)

file(GLOB _sources src/*.c)
psn00bsdk_add_executable(sprite_anim GPREL ${_sources})
psn00bsdk_target_incbin(sprite_anim PRIVATE iron_knight_tim iron_knight.tim)

install(FILES ${PROJECT_BINARY_DIR}/sprite_anim.exe TYPE BIN)

# psn00bsdk_add_cd_image(sprite_anim_iso sprite_anim iso.xml DEPENDS sprite_anim system.cnf)