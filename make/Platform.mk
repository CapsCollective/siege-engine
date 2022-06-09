# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := windows
	CXX ?= g++
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := $(call platformpth,$(CURDIR)/scripts/mkdir.bat)
	RM := rm -r -f
	COPY = $(call platformpth,$(CURDIR)/scripts/copy.bat) $1 $2 $3
	COPY_DIR = $(call platformpth,$(CURDIR)/scripts/copy.bat) --copy-directory $1 $2
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
endif