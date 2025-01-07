# Copyright (c) 2020-present Caps Collective & contributors
# Originally authored by Jonathan Moallem (@jonjondev) & Aryeh Zinn (@Raelr)
#
# This code is released under an unmodified zlib license.
# For conditions of distribution and use, please see:
#     https://opensource.org/licenses/Zlib

# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := windows
	CXX ?= g++
	THEN := &&
	WCARD := *.*
	PATHSEP := \$(BLANK)
	MKDIR = powershell -executionpolicy bypass $(scriptsDir)/mkdir.ps1
	RM = powershell -executionpolicy bypass $(scriptsDir)/rm.ps1
	EXE_NAME := .exe
	VOLK_OS := WIN32_KHR
	COPY = powershell -executionpolicy bypass $(scriptsDir)/copy.ps1 -Source $1 -Destination $2 -Filter $3
	BASENAME = $(basename $1)
	VALIDATION_LAYERS_INSTALL_DIR := explicit_layer.d
	BUILD_FLAGS_SCRIPT = powershell -executionpolicy bypass $(scriptsDir)/buildflags.ps1 --Target $1 --CXXFlags $2 --Dirs $3
	BUILD_LIB = powershell -executionpolicy bypass $(scriptsDir)/combinelibs.ps1 --src_objs $1 --output_dir $2 --output_name $3
	COMBINE_LIBS = powershell -executionpolicy bypass $(scriptsDir)/combinelibs.ps1 --src_libs $1 --src_objs $2 --output_dir $3 --output_name $4
	PACKAGE_SCRIPT = powershell -executionpolicy bypass $(scriptsDir)/package.ps1 --pkg-name '$1' --exe-name $2 --output-dir $3 --build-dir $4 $5
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := linux
		CXX ?= g++
		VOLK_OS := XLIB_KHR
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macos
		CXX ?= clang++
		VOLK_OS := MACOS_MVK
	endif

	# Set UNIX macros
	WCARD := **
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp -r $1$(PATHSEP)$3 $2
	BASENAME = $(shell basename $1)
	COPY_DIR = $(call COPY,$1,$2,$3)
	VALIDATION_LAYERS_INSTALL_DIR := lib/explicit_layer.d
	BUILD_FLAGS_SCRIPT = $(scriptsDir)/buildflags.sh $1 "$2" "$3"
	BUILD_LIB = $(scriptsDir)/combinelibs.sh --src_objs $1 --output_dir $2 --output_name $3
	COMBINE_LIBS = $(scriptsDir)/combinelibs.sh --src_libs $1 --src_objs $2 --output_dir $3 --output_name $4
	PACKAGE_SCRIPT = $(scriptsDir)/package.sh $1 $2 $3 $4 $5
endif
