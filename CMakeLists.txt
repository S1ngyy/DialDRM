## This file is the CMakeList.txt for DialDRM
cmake_minimum_required(VERSION 2.8)

## This includes the Vita toolchain, must go before project definition
# It is a convenience so you do not have to type
# -DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake for cmake. It is
# highly recommended that you include this block for all projects.
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

## Define project parameters here
# Name of the project
project(DialDRM)
# This line adds Vita helper macros, must go after project definition in order
# to build Vita specific artifacts (self/vpk).
include("${VITASDK}/share/vita.cmake" REQUIRED)

## Configuration options for this app
# Display name (under bubble in LiveArea)
set(VITA_APP_NAME "DialDRM")
# Unique ID must be exactly 9 characters. Recommended: XXXXYYYYY where X =
# unique string of developer and Y = a unique number for this app
set(VITA_TITLEID  "SNGY00001")
# Optional version string to show in LiveArea's more info screen
set(VITA_VERSION  "01.00")
# Set if built as a system app for multitasking purposes (currently not functional, keep on false)
set(SYSTEM_APP false)

## Flags and includes for building
# Note that we make sure not to overwrite previous flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -mfpu=neon -fno-optimize-sibling-calls -ftree-vectorize -fsingle-precision-constant -march=armv7-a -mtune=cortex-a9 -mfloat-abi=hard -ffast-math")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -mfpu=neon -fno-optimize-sibling-calls -ftree-vectorize -fsingle-precision-constant -march=armv7-a -mtune=cortex-a9 -mfloat-abi=hard -ffast-math -fpermissive")


# Add any additional library paths here
# ${CMAKE_CURRENT_BINARY_DIR} lets you use any library currently being built
link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

## Build and link
# Add all the files needed to compile here
add_executable(${PROJECT_NAME}.elf
  main.cpp
  stb_image.h
)


# Library to link to (drop the -l prefix). This will mostly be stubs.
  target_link_libraries(${PROJECT_NAME}.elf
		SceDisplay_stub
		SceGxm_stub
		SceCtrl_stub
		SceTouch_stub
		SceCommonDialog_stub
		vitaGL
		mathneon
		imgui
		-Wl,--whole-archive pthread -Wl,--no-whole-archive
)

## Create Vita files
#vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME}.elf)
# The FILE directive lets you add additional files to the VPK, the syntax is
# FILE src_path dst_path_in_vpk. In this case, we add the LiveArea paths.
#vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
#  VERSION ${VITA_VERSION}
# NAME ${VITA_APP_NAME}
if (SYSTEM_APP)
add_custom_target(${PROJECT_NAME}.vpk ALL
	COMMAND vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
	COMMAND vita-make-fself -m 0x00001400 -at 0x08 ${PROJECT_NAME}.velf eboot.bin
	#COMMAND vita-make-fself ${PROJECT_NAME}.velf eboot.bin
	COMMAND vita-mksfoex -s TITLE_ID=SNGY00001 -d ATTRIBUTE=32768 "${PROJECT_NAME}" param.sfo
	COMMAND vita-pack-vpk -s param.sfo -b eboot.bin
	-a ../games/somi/somiA.png=games/somi/somiA.png
	-a ../games/somi/somiB.png=games/somi/somiB.png
	-a ../games/somi/somi.png=games/somi/somi.png
	-a ../sce_sys/icon0.png=sce_sys/icon0.png
	-a ../sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png
	-a ../sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png
	-a ../sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml
	${PROJECT_NAME}.vpk
)
else()
add_custom_target(${PROJECT_NAME}.vpk ALL
	COMMAND vita-elf-create ${PROJECT_NAME}.elf ${PROJECT_NAME}.velf
	#COMMAND vita-make-fself -m 0x00001400 -at 0x08 ${PROJECT_NAME}.velf eboot.bin
	COMMAND vita-make-fself ${PROJECT_NAME}.velf eboot.bin
	COMMAND vita-mksfoex -s TITLE_ID=SNGY00001 -d ATTRIBUTE=32768 "${PROJECT_NAME}" param.sfo
	COMMAND vita-pack-vpk -s param.sfo -b eboot.bin
	-a ../games/somi/somiA.png=games/somi/somiA.png
	-a ../games/somi/somiB.png=games/somi/somiB.png
	-a ../games/somi/somi.png=games/somi/somi.png
	-a ../sce_sys/icon0.png=sce_sys/icon0.png
	-a ../sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png
	-a ../sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png
	-a ../sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml
	${PROJECT_NAME}.vpk
)
endif()
add_dependencies(${PROJECT_NAME}.vpk ${PROJECT_NAME}.elf)