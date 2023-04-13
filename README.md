# Siege Engine

[![macOS Status](../../workflows/macOS/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/macOS.yml)
[![Linux Status](../../workflows/Ubuntu/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/ubuntu.yml)
[![Windows Status](../../workflows/Windows/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/windows.yml)
[![Tests Status](../../workflows/Tests/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/tests.yml)
[![Formatting Status](../../workflows/Formatting/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/formatting.yml)
[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg)](https://opensource.org/licenses/Zlib)
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](CODE_OF_CONDUCT.md)

Sometimes game development can feel like a bit of a slog; an endless battle in which the developer is set against an overwhelming bulwark of required knowledge to create a functional game. On one hand, one must understand _linear algebra, calculus, and the intricacies of the rasterised rendering pipeline_ whilst also expecting a mastery of _memory mangement, build systems, algorithms, and coherent software architecture_.

The reality is that making games is hard, and making games can be even harder when one does not fully understand the underlying foundations below the many layers of abstraction present in many commercial and general game engines. Using them can often feel disempowering as they discourage deeper learning through sheer size and complexity.

The ultimate purpose of this project is to act as the metaphorical siege engine to break down the barriers to game engine development. To this end, Siege aims to be a **light-weight, cross platform, 3D game engine**. In doing so, we hope to create an engine for game developers, by game developers.

### Current Compatability

| OS          | Default Compiler |  Last Manual Build  |                   Compile Status                     |
| ----------- | ---------------- | ------------------- | ---------------------------------------------------- |
| **macOS**   | Clang++          | `Monterey 12.3.1`   | [![macOS Status](../../workflows/macOS/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/macOS.yml)     |
| **Linux**   | G++              | `Ubuntu-20.04.2.0`  | [![Linux Status](../../workflows/Ubuntu/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/ubuntu.yml)    |
| **Windows** | MinGW (G++)      | `Windows 10 19041`  | [![Windows Status](../../workflows/Windows/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/windows.yml)   |

## Contents

- [Getting Started](#getting-started)
    - [Dependencies](#dependencies)
    - [Setup](#setup)
        - [Building With Validation Layers](#building-with-validation-layers)
    - [Building the Project](#building-the-project)
    - [Working With Siege](#working-with-siege)
        - [Compile Options](#compile-options)
- [Project Structure](#project-structure)
- [Contributing](#contributing)
    - [How do I contribute?](#how-do-i-contribute)
    - [Contributors](#contributors)
- [Licence](#licence)

## Getting Started

### Dependencies

The following are dependencies for building and running the project:

- **All platforms**
    - [CMake](https://cmake.org/)
    - [Python](https://www.python.org/)
- **macOS**
    - [GNU Make](https://www.gnu.org/software/make/)
    - [Clang Format](https://clang.llvm.org/docs/ClangFormat.html) (Linux & MacOS)
- **Windows**
    - [MinGW32 Make](https://www.mingw-w64.org/)
    - [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/)
- **Linux**
    - For linux environments the following packages are required for building the dependencies:
        - `libx11-xcb-dev`
        - `libxkbcommon-dev`
        - `libwayland-dev`
        - `libxrandr-dev`
        - `libasound2-dev`
        - `mesa-common-dev`
        - `libx11-dev`
        - `libxrandr-dev`
        - `libxi-dev`
        - `xorg-dev`
        - `libgl1-mesa-dev`
        - `libglu1-mesa-dev`

These can be installed using `apt-get` or an equivalent package manager for your Linux distribution:

```console
// Only required if not building with the Vulkan SDK
$ sudo apt-get update
$ sudo apt-get install git build-essential libx11-xcb-dev libxkbcommon-dev libwayland-dev libxrandr-dev

// Required for building GLFW
$ sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

### Setup

1. Navigate to the project root
2. Run the setup script for your platform:
    
    ```console
    // Linux & macOS
    $ ./scripts/setup.sh
    ```
    ```console
    // Windows
    > ./scripts/Setup.ps1
    ```

3. This should install all required dependencies. Once completed a `.env` file will be generated with all required variables. If the build is completed with no issue then you can proceed to build the project.

#### Building With Validation Layers

If you want to build with validation layers then the setup scripts should be run by passing the `--include-validation-layers` flag:

```console
// Linux & macOS
$ ./scripts/setup.sh --include-validation-layers
```
```console
// Windows
> ./scripts/Setup.ps1 -Include_Validation_Layers
```

**NOTE**: Building with this option can take some time to complete. Please be patient while the project builds the required validation layers.

### Building the Project

Assuming all dependencies have been satisfied, the project can be built using the following command:

```console
// Linux and macOS
$ make

// Linux and macOS (with validation layers)
$ make CXXFLAGS="-DENABLE_VALIDATION_LAYERS=1"
```
```console
// Windows
> mingw32-make

// Windows (with validation layers)
> mingw32-make CXXFLAGS="-DENABLE_VALIDATION_LAYERS=1"
```

Once it completes, all targets should be built and ready to run from the newly generated `output` directory.

### Working With Siege

Now that you have the project setup and compiling on your system, it's time to start programming! We first of all should note that Siege is a lightweight game engine made by and for programmers. This is to say that it doesn't have a nice and comfy GUI to do everything in, or even one way of defining its runtime. The engine is split into a number of static libraries that are linked into a final executable so that you can pick and choose what functionality you need. While it can be treated like a framework, each piece is fundamentally written to work with each other in the context of the engine as a whole. 

You can see examples of this in any of the application targets under `examples`. After running Make, you should find a newly generated `output` directory, populated with packaged executables for all example projects.

#### Compile Options

| Option                         | Default Value    | Description                                                                      |
| ------------------------------ | ---------------- | -------------------------------------------------------------------------------- |
| **DEBUG**                      | `1`              | When set to `1`, will compile in debug mode, and default **CC_LOG_LEVEL** to `2` |
| **CC_LOG_LEVEL**               | `2`              | Sets the logging verbosity to `info` (`2`), `warning` (`1`), and `error` (`0`)   |
| **ENABLE_VALIDATION_LAYERS**   | `0`              | When set to `1`, will enable Vulkan validation layers for the executable         |
| **CXX**                        | `g++`/`clang++`  | Sets the compiler to use during build steps (default differs per-platform)       |
| **CXXFLAGS**                   | N/A              | Allows for the specification of compile directives, such as macro definitions    |

## Project Structure

```
[root]
     ├─[engine]
     │        ├─[core] <- the engine's core library
     │        ├─[render] <- the engine's renderer
     │        ├─[utils] <- the engine's utils library
     │
     ├─[examples]
     │          ├─[game] <- an example app utilising all of the engine's libraries
     │          ├─[render] <- an example app demonstrating the renderer
     │
     ├─[make] <- additional Make file utilities for the build system
     ├─[scripts] <- additional scripts for the build system and user convenience
     ├─[tests] <- a unit test app covering all of the engine's libraries
     ├─[vendor] <- all third-party libraries used by the project
```

## Contributing

### How do I contribute?

It's pretty simple actually:

1. Fork it from [here](https://github.com/CapsCollective/siege-engine/fork)
2. Create your feature branch (`git checkout -b cool-new-feature`)
3. Commit your changes (`git commit -m "Added some feature"`)
4. Push to the branch (`git push origin cool-new-feature`)
5. Create a new pull request for it!

If you want to contribute to the project, be sure to review the [contribution guidelines](CONTRIBUTING.md) for more info on adhering to the project's standards. This project adheres to the [Contributor Covenant code of conduct](CODE_OF_CONDUCT.md), and by contributing, interacting, or otherwise participating in it, you are expected to uphold this code.

We use GitHub issues for tracking requests and bugs where possible, please use the GitHub discussions forum for help, general questions and other discourse.

### Contributors

- [J-Mo63](https://github.com/J-Mo63) Jonathan Moallem - co-creator, maintainer
- [Raelr](https://github.com/Raelr) Aryeh Zinn - co-creator, maintainer

## Licence

This project is licenced under an unmodified zlib/libpng licence, which is an OSI-certified, BSD-like licence that allows static linking with closed source software. Check [`LICENCE`](LICENCE) for further details.
