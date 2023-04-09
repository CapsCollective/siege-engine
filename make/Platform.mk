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
	COPY = $(scriptsDir)/copy.bat $1 $2 $3
	COPY_DIR = $(scriptsDir)/copy.bat --copy-directory $1 $2
	VALIDATION_LAYERS_INSTALL_DIR := explicit_layer.d
	BUILD_FLAGS_SCRIPT = $(scriptsDir)/buildFlags.bat
	COMBINE_LIBS = $(scriptsDir)/Combine-Libs.ps1 -Src_Libs $1 -Src_Objs $2 -Extraction_Dir $3 -Output_Name $4
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := linux
		CXX ?= g++
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macos
		CXX ?= clang++
	endif

	# Set UNIX macros
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp -r $1$(PATHSEP)$3 $2
	COPY_DIR = $(call COPY,$1,$2,$3)
	VALIDATION_LAYERS_INSTALL_DIR := lib/explicit_layer.d
	BUILD_FLAGS_SCRIPT = $(scriptsDir)/buildFlags.sh
endif