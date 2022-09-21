<p align="center">
    <img src="./.github/assets/pensive-cowboy.png" alt="sad" height="100">
</p>

## Installation
Luckily most of the setup for the repository has been automated using the `bet` (**b**efore **e**ven **t**rying) scripting pipeline. As a result, only [Python v3.10+](https://www.python.org/downloads/) is required to properly install the project on either Windows or Mac.

To start, recursively clone the repository and follow the instructions below for your OS.

```
$ git clone --recursive git@github.com:focus-fire/sad.git

or

$ git clone --recursive https://github.com/focus-fire/sad.git
```

**Note:** If you're using a GUI client for git such as GitHub Desktop or SourceTree it should automatically perform a recursive clone on the repository. Otherwise copy the command above, right click your file explorer, and run the command in git bash. 

If the repository is not cloned recursively **nothing will work**.

### Quick Setup
If you're not inspired to run a lot of the commands listed below, the `cry` script in the root of the project can be run to clean, build, and (optionally) test the project at once.

On Windows, double-click the `cry.bat` script. Or, on either Windows or Mac enter the following command in a Terminal. This command can be used as an alternative to using separate commands to clean and build the project when required.

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
