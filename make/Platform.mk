# Copyright (c) 2022 Jonathan Moallem (@J-Mo63) & Aryeh Zinn (@Raelr)
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
	PATHSEP := \$(BLANK)
	MKDIR = $(scriptsDir)/mkdir.bat
	RM := rm -r -f
	EXE_NAME := .exe
	VOLK_OS := WIN32_KHR
	COPY = $(scriptsDir)/copy.bat $1 $2 $3
	COPY_DIR = $(scriptsDir)/copy.bat --copy-directory $1 $2
	VALIDATION_LAYERS_INSTALL_DIR := explicit_layer.d
	BUILD_FLAGS_SCRIPT = $(scriptsDir)/buildflags.bat
	COMBINE_LIBS = powershell $(scriptsDir)/combinelibs.ps1 --src_libs $1 --src_objs $2 --output_dir $3 --output_name $4
else
	COMBINE_LIBS = $(scriptsDir)/combinelibs.sh --src_libs $1 --src_objs $2 --output_dir $3 --output_name $4
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
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp -r $1$(PATHSEP)$3 $2
	COPY_DIR = $(call COPY,$1,$2,$3)
	VALIDATION_LAYERS_INSTALL_DIR := lib/explicit_layer.d
	BUILD_FLAGS_SCRIPT = $(scriptsDir)/buildflags.sh
endif