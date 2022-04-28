# SNEK

SNEK is an experimental rendering library that utilises Vulkan and GLFW to enable cross-platform 2D and 3D rendering.

The intention of SNEK is to serve as a learning platform for Vulkan and high performance rendering concepts.

## Contents

- [SNEK](#snek)
  - [Contents](#contents)
  - [Getting Started](#getting-started)
    - [Dependencies](#dependencies)
    - [Environment Setup](#environment-setup)
      - [Using the Vulkan SDK](#using-the-vulkan-sdk)
      - [Setup without the Vulkan SDK](#setup-without-the-vulkan-sdk)
      - [Setup without the Vulkan SDK with validation layers](#setup-without-the-vulkan-sdk-with-validation-layers)
    - [Building the Project](#building-the-project)
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

### Environment Setup

There are three ways to build the project:

1. Using the Vulkan SDK.
2. Building without the Vulkan SDK.
3. Building without the Vulkan SDK with validation layers.

#### Using the Vulkan SDK

Building and running SNEK with the Vulkan SDK is the quickest way to get started. Doing so involves the following steps:

1. Download the Vulkan SDK from the [LunarG website](https://vulkan.lunarg.com/).
2. Install the SDK to your machine.
3. Export an environmental variable called `VULKAN_SDK` and point it to the install location of the Vulkan SDK.
4. Run the install script for your platform:

   ```
   // Linux & macos
   $ ./scripts/<PLATFORM>/install.sh

   // Windows
   > ./scripts<PLATFORM>/install.bat
   ```

5. Duplicate the `.env.template` file and rename it to `.env`.
6. Uncomment and fill in the following variables. Each variable should point to a directory or :

   1. `VK_LAYER_PATH` - if you're using validation layers, set this to the directory where they're located.
   2. `VK_ICD_FILENAMES` - if using MacOS, set this to the path that the MoltenVK ICD is located. This usually points to a file called `MoltenVK_icd.json`.
   3. `VULKAN_INCLUDE_DIR` - point this to the directory where the vulkan headers are located.

7. Save the .env file. This should be enough to build the project.

#### Setup without the Vulkan SDK

1. Navigate to the project root.
2. Run the install script for your platform:

   ```
   // Linux & macos
   $ ./scripts/<PLATFORM>/install.sh

   // Windows
   > ./scripts<PLATFORM>/install.bat
   ```

3. This should install all required dependencies. Once completed a `.env` file will be generated with all required variables. If the build is completed with no issue then you can proceed to build the project.

#### Setup without the Vulkan SDK with validation layers

1. Navigate to the project root.
2. Run the install script for your platform with the DEBUG flag:

   ```
   // Linux & macos
   $ ./scripts/<PLATFORM>/install.sh DEBUG

   // Windows
   > ./scripts<PLATFORM>/install.bat DEBUG
   ```

3. This should install all required dependencies plus the validation layers. Once completed a `.env` file will be generated with all required variables. If the build is completed with no issue then you can proceed to build the project.

**NOTE**: Building the DEBUG build can take some time to complete. Please be patient while the project builds the required validation layers.

### Building the Project

Once all dependencies have been set up, the project can be build and run using the following command:

```
// linux and macos
make bin/app; make execute

// linux and macos - DEBUG
make CXXFLAGS="-DDEBUG" bin/app; make execute

// windows
mingw32-make bin/app && mingw32-make execute

// windows - DEBUG
mingw32-make CXXFLAGS="-DDEBUG" bin/app && mingw32-make execute
```

These commands should build the project and immediately run executable.

To build the project separately, you can call the `bin/app` target separately. The same can be done for the `execute` target.

Once these are done the project should be built and ready to go. Enjoy!

## Contributing

### How do I contribute?

It's pretty simple actually:

1. Fork it from [here](https://github.com/CapsCollective/raylib-cpp-starter/fork)
2. Create your feature branch (git checkout -b cool-new-feature)
3. Commit your changes (git commit -m "Added some feature")
4. Push to the branch (git push origin cool-new-feature)
5. Create a new pull request for it!

### Contributors

- [J-Mo63](https://github.com/J-Mo63) Jonathan Moallem - co-creator, maintainer
- [Raelr](https://github.com/Raelr) Aryeh Zinn - co-creator, maintainer

## Licence

This project is licenced under an unmodified zlib/libpng licence, which is an OSI-certified, BSD-like licence that allows static linking with closed source software. Check [`LICENCE`](LICENSE) for further details.
