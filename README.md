# Siege Engine

[![macOS Status](../../workflows/macOS/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/macOS.yml)
[![Linux Status](../../workflows/Ubuntu/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/ubuntu.yml)
[![Windows Status](../../workflows/Windows/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/windows.yml)
[![Tests Status](../../workflows/Tests/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/tests.yml)
[![Formatting Status](../../workflows/Formatting/badge.svg)](https://github.com/CapsCollective/siege-engine/actions/workflows/formatting.yml)
[![License: Zlib](https://img.shields.io/badge/License-Zlib-lightgrey.svg)](https://opensource.org/licenses/Zlib)
[![Contributor Covenant](https://img.shields.io/badge/Contributor%20Covenant-2.1-4baaaa.svg)](CODE_OF_CONDUCT.md)

A lightweight, cross-platform game engine

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
    > ./scripts/setup.bat
    ```

3. This should install all required dependencies. Once completed a `.env` file will be generated with all required variables. If the build is completed with no issue then you can proceed to build the project.

#### Building With Validation Layers

If you want to build with validation layers then the setup scripts should be run by passing the `--include-validation-layers` flag:

```console
// Linux & macOS
$ ./scripts/install.sh --include-validation-layers
```
```console
// Windows
> ./scripts/setup.bat --include-validation-layers
```

**NOTE**: Building with this option can take some time to complete. Please be patient while the project builds the required validation layers.

### Building the Project

Assuming all dependencies have been satisfied, the project can be build and run using the following command:

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

These commands should build the project and immediately run executable.

Once these are done the project should be built and ready to go. Enjoy!

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
