![macOS Status](../../workflows/macOS/badge.svg)
![Linux Status](../../workflows/Ubuntu/badge.svg)
![Windows Status](../../workflows/Windows/badge.svg)
![Tests Status](../../workflows/Tests/badge.svg)
# Siege Engine

A lightweight, cross-platform game engine

## Contents

- [Siege Engine](#siege-engine)
    - [Contents](#contents)
    - [Getting Started](#getting-started)
        - [Dependencies](#dependencies)
        - [Environment Setup](#environment-setup)
            - [Using the Vulkan SDK](#using-the-vulkan-sdk)
            - [Setup without the Vulkan SDK](#setup-without-the-vulkan-sdk)
        - [Building the Project](#building-the-project)
    - [Project Structure](#project-structure)
    - [Contributing](#contributing)
        - [How do I contribute?](#how-do-i-contribute)
        - [Contributors](#contributors)
    - [Licence](#licence)

## Getting Started

### Dependencies

The following are dependencies for building and running SNEK:

- [Cmake](https://cmake.org/) (all platforms)
- [GNU Make](https://www.gnu.org/software/make/) (linux & macos)
- [MingW32-make](https://www.mingw-w64.org/) (windows)
- [Python](https://www.python.org/) (all platforms)
- [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/) (windows)

**Linux:** For linux environments the following packages are required for building the dependencies.

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

These can be installed using `apt-get` or an equivalent package manager for your OS distribution:

```
// Only required if not building with the Vulkan SDK
$ sudo apt-get update
$ sudo apt-get install git build-essential libx11-xcb-dev libxkbcommon-dev libwayland-dev libxrandr-dev

// Required for building GLFW
$ sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
```

### Setup

1. Navigate to the project root.
2. Run the install script for your platform:

   ```bash
   // Linux & macos
   $ ./scripts/setup.sh

   // Windows
   > ./scripts/setup.bat
   ```

3. This should install all required dependencies. Once completed a `.env` file will be generated with all required variables. If the build is completed with no issue then you can proceed to build the project.

**Building validation layers**

If you want to build validation layers then the install scripts should be run by passing the `--include-validation-layers` flag to the `install` script:

```
// Linux & macos
$ ./scripts/install.sh --include-validation-layers

// Windows
> ./scripts/setup.bat --include-validation-layers
```

**NOTE**: Building with this option can take some time to complete. Please be patient while the project builds the required validation layers.

### Building the Project

Once all dependencies have been set up, the project can be build and run using the following command:

```
// linux and macos
$ make

// linux and macos (with validation layers)
$ make CXXFLAGS="-DENABLE_VALIDATION_LAYERS=1"

// windows
> mingw32-make

// windows - DEBUG
> mingw32-make CXXFLAGS="-DENABLE_VALIDATION_LAYERS=1"
```

These commands should build the project and immediately run executable.

Once these are done the project should be built and ready to go. Enjoy!

## Project Structure

```
[root]
     ├─[engine]
     │        │
     │        ├─[core]
     │        │      │
     │        │      ├─[entity]
     │        │      │
     │        │      ├─[input]
     │        │      │
     │        │      ├─[physics]
     │        │      │
     │        │      ├─[render]
     │        │      │
     │        │      ├─[scene]
     │        │
     │        ├─[render]
     │        │        │  
     │        │        ├─[assets]
     │        │        │        ├─[shaders]
     │        │        │
     │        │        ├─[components]
     │        │        │
     │        │        ├─[input]
     │        │        │       
     │        │        ├─[renderer]
     │        │        │          │
     │        │        │          ├─[buffer]
     │        │        │          │
     │        │        │          ├─[camera]
     │        │        │          │
     │        │        │          ├─[descriptor]
     │        │        │          │
     │        │        │          ├─[device]
     │        │        │          │        │
     │        │        │          │        ├─[utils]
     │        │        │          │
     │        │        │          ├─[image]
     │        │        │          │       │
     │        │        │          │       ├─[utils]
     │        │        │          │
     │        │        │          ├─[lights]
     │        │        │          │
     │        │        │          ├─[material]
     │        │        │          │
     │        │        │          ├─[mesh]
     │        │        │          │
     │        │        │          ├─[model]
     │        │        │          │
     │        │        │          ├─[pass]
     │        │        │          │      │
     │        │        │          │      ├─[utils]
     │        │        │          │
     │        │        │          ├─[pipeline]
     │        │        │          │
     │        │        │          ├─[renderer]
     │        │        │          │
     │        │        │          ├─[shader]
     │        │        │          │
     │        │        │          ├─[swapchain]
     │        │        │          │
     │        │        │          ├─[utils]
     │        │        ├─[utils]
     │        │        │
     │        │        ├─[window]
     │        │
     │        ├─[utils]
     │
     ├─[examples]
     │          │
     │          ├─[game]
     │          │      │
     │          │      ├─[assets]
     │          │      │
     │          │      ├─[src]
     │          │      
     │          ├─[render]
     │                   │
     │                   ├─[assets]
     │                   │
     │                   ├─[src]
     ├─[make]
     │
     ├─[scripts]
     │
     ├─[tests]
     │       │
     │       ├─[core]
     │       │
     │       ├─[data]
     │       │
     │       ├─[utils]
     │
     ├─[vendor]
```

## Contributing

### How do I contribute?

It's pretty simple actually:

1. Fork it from [here](https://github.com/CapsCollective/a-dark-discomfort/fork)
2. Create your feature branch (git checkout -b cool-new-feature)
3. Commit your changes (git commit -m "Added some feature")
4. Push to the branch (git push origin cool-new-feature)
5. Create a new pull request for it!

### Contributors

- [J-Mo63](https://github.com/J-Mo63) Jonathan Moallem - co-creator, maintainer
- [Raelr](https://github.com/Raelr) Aryeh Zinn - co-creator, maintainer

## Licence

This project is licenced under an unmodified zlib/libpng licence, which is an OSI-certified, BSD-like licence that allows static linking with closed source software. Check [`LICENCE`](LICENSE) for further details.