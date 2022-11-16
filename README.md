<p align="center">
    <img src="./.github/assets/pensive-cowboy.png" alt="sad" height="100">
</p>
<p align="center">
    <img src="https://github.com/focus-fire/sad/actions/workflows/build.yml/badge.svg" alt="build-badge">
    <img src="https://github.com/focus-fire/sad/actions/workflows/test.yml/badge.svg" alt="test-badge">
</p>

# Table of Contents

1. [Installation](##Installation)
2. [Quick Setup](###Quick-Setup)
3. [Windows](###Windows)
4. [Mac](###Mac)
5. [Updates](##Updates)
6. [Tests](##Tests)
7. [Running Tests](###Running-Tests)
8. [Issues](##Issues)

--------------

## Installation
Luckily most of the setup for the repository has been automated using the `bet` (**b**efore **e**ven **t**rying) scripting pipeline. As a result, only [Python v3.10+](https://www.python.org/downloads/) is required to properly install the project on either Windows or Mac.

***Note: For windows users, please ensure you have Python 3 installed from the Windows Store.***

To start, recursively clone the repository and follow the instructions below for your OS.

```
$ git clone --recursive git@github.com:focus-fire/sad.git

or

$ git clone --recursive https://github.com/focus-fire/sad.git
```

**Note:** If you're using a GUI client for git such as GitHub Desktop or SourceTree it should automatically perform a recursive clone on the repository. Otherwise copy the command above, right click your file explorer, and run the command in git bash. 

If the repository is not cloned recursively **nothing will work**.

### Quick Setup
When in doubt, just `cry`! 

On Windows, double-click the `cry.bat` script. Or enter the following command in a Terminal on either Windows or Mac. 

This command can be used as an alternative to using separate commands to clean and build the project when required.

```
$ ./cry
```

### Windows

1. Run the `bet.py` script from the terminal and specify the proper build flag for your target toolset 

```
# Visual Studio 2022 Solution (default)
$ python3 .\Scripts\bet.py --build 

# Visual Studio 2019 Solution
$ python3 .\Scripts\bet.py --build vs2019

# OS Independent Makefiles (must have WSL, cygwin, or mingw installed) 
$ python3 .\Scripts\bet.py --build gmake2
```

This step will have to be rerun **each** time any premake file (`*.premake5.lua*`) is changed. Please note that personal changes to generated files (such as a Visual Studio solution) *may* not persist once this command is re-executed.  

3. Open the `sad.sln` solution file in Visual Studio if `vs2022` or `vs2019` was your desired toolset

4. If your target toolset was `gmake2`, run the Makefiles in a terminal

```
$ make
```

The binaries and static libraries should be located in the `Build/Bin` directory of the project. These can be run in a GUI using something like the Visual Studio Code [Makefile Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.makefile-tools) or can be run in the terminal.

```
$ .\Build\Bin\Game\Game
```

### Mac

1. Execute the `bet.py` script from the terminal to build the project with the `gmake2` or `xcode4` toolsets 

```
# OS Independent Makefiles 
$ python3 ./Scripts/bet.py --build gmake2

# XCode Workspace (WARNING: Untested, unsupported, and unrecommended)
$ python3 ./Scripts/bet.py --build xcode4
```

Similar to the Windows installation, this step will have to be redone **every** time a change is made to a `*premake5.lua*` file.

2. Generate compile commands for the project 

```
$ python3 ./Scripts/bet.py --gen-cc
```

This will install a [tool](https://github.com/tarruda/premake-export-compile-commands) for premake to export compile commands for the current project if it is not yet installed. These will provide intellisense in projects without an IDE or configured compiler (ie: a default Visual Studio Code installation).

3. Install the Microsoft C/C++ tools for Visual Studio Code and edit the project configuration (ex: cmd+shift+p > `C/C++: Edit Configurations`)

4. Under 'Advanced Settings' modify the 'Compile Commands' to `compile_commands/debug.json` (or another `compile_commands/*.json` file for your desired configuration) 

5. Run the target Makefiles in a terminal

```
$ make
```

The binaries and static libraries should be located in the `Build/Bin` directory of the project. These can be run in a GUI using something like the Visual Studio Code [Makefile Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.makefile-tools) or can be run in the terminal.

```
$ ./Build/Bin/Game/Game
```

-------------
## Updates

```
git pull --recurse-submodules
```
Run this command inside of the main folder of the repository and it will pull all changes in the repo, as well as changes in the submodules.

It is ***recommended*** that you run this command often when you start working on the repo after cloning it.

-------------
## Tests
This project is setup to use [Catch2](https://github.com/catchorg/Catch2) for unit and integration testing. Running tests is also tied into the bet scripting pipeline.

### Running Tests

1. Build the `Tests` project using Visual Studio, msbuild (might have to be manually added to your system's environment variables), or make with respect to your desired toolset

```
Launch Visual Studio > Right-click the 'Tests' project > Select 'Build'

or

$ msbuild sad.sln

or

$ make
```

2. Run the `bet.py` script with the `--tests` flag enabled in a terminal

```
$ python3 ./Scripts/bet.py --tests
```

**Note:** Alternatively the `--compile-tests` flag can be used to compile the tests with a particular toolchain and then run the testbed (basically steps 1/2 combined).

```
# Alternatively, the --compile-tests flag can be used to compile with a toolchain before running the testbed
$ python3 ./Scripts/bet.py --compile-tests msbuild
$ python3 ./Scripts/bet.py --compile-tests make
```

After running the script you should see some output similar to the following...

```
[tests.py] Running Catch2 tests...

===============================================================================
All tests passed (4 assertions in 1 test case)

[tests.py] bet! Successfully ran tests.
```

-------------
## Issues

Here are some errors people have run into while working on the project or attempting to build the project.

-------------
You are able to build the solution file, but the build fails, and this is in the output log.

```
Element <LanguageStandard> has an invalid value of "stdcpp20".
```

**Solution: Update your Visual Studio. You are most likely using an outdated version.**

-------------

You run into some sort of GL Error while attempting to build the project.

**Solution: Your graphics card may be outdated, in this case, you may need to lower the GL version locally in order to run the project.**