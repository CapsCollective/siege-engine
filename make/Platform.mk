# Check for Windows
ifeq ($(OS), Windows_NT)
	# Set Windows macros
	platform := Windows
	CXX ?= g++
	THEN := &&
	PATHSEP := \$(BLANK)
	MKDIR := -mkdir -p
	RM := -del /q
	COPY = -robocopy "$(call platformpth,$1)" "$(call platformpth,$2)" $3
else
	# Check for MacOS/Linux
	UNAMEOS := $(shell uname)
	ifeq ($(UNAMEOS), Linux)
		# Set Linux macros
		platform := Linux
		CXX ?= g++
	endif
	ifeq ($(UNAMEOS), Darwin)
		# Set macOS macros
		platform := macOS
		CXX ?= clang++
	endif

	# Set UNIX macros
	THEN := ;
	PATHSEP := /
	MKDIR := mkdir -p
	RM := rm -rf
	COPY = cp $1$(PATHSEP)$3 $2
endif