<h3 align="center">😔</h3> 

### Installation
Luckily most of the setup for the repository has been automated using the a custom scripting pipeline. As a result, only [Python3 v3.9+](https://www.python.org/downloads/) is required to properly install the project.

1. Recursively clone the repository and its submodules

```
$ git clone --recursive git@github.com:focus-fire/sad.git

or

$ git clone --recursive https://github.com/focus-fire/sad.git
```

**Note:** If you're using a GUI client for git such as GitHub Desktop or SourceTree it should automatically perform a recursive clone on the repository. Otherwise copy the command above, right click your file explorer, and run the command in git bash. 

If the repository is not cloned recursively **nothing will work**.

2. Execute the `bet.py` script from the terminal and specify the proper build flag for your target platform 

```
# Visual Studio 2022
$ python3 .\Scripts\bet.py --build vs2022

# Visual Studio 2019
$ python3 .\Scripts\bet.py --build vs2019

# OS Independent Makefiles 
$ python3 .\Scripts\bet.py --build gmake2

# XCode (WARNING: Untested and Unrecommended)
$ python3 .\Scripts\bet.py --build xcode4
```

This step will have to be rerun **each** time any premake file (`*.premake5.lua*`) is changed. Please note that personal changes to the individual Visual Studio solution *may* not persist once this command is re-executed.  

3. Open the `sad.sln` solution file in Visual Studio 2022 if `vs2022` or `vs2019` was your desired toolset

4. If your target toolset was `gmake2`, run the makefiles in a terminal

```
$ make
```

The binaries and static libraries should be located in the `Build/Bin` directory of the project.

### Tests
This project is setup to use [Catch2](https://github.com/catchorg/Catch2) for unit and integration testing. Running tests is also tied into the scripting pipeline.

#### Running Tests

1. Build the `Tests` project using Visual Studio or MSBuild (might have to be manually added to your system's environment variables)

```
Launch Visual Studio > Right-click the 'Tests' project > Select 'Build'

or

$ msbuild sad.sln
```

2. Run the `bet.py` script with the `--tests` flag enabled in a terminal

```
$ python3 .\Scripts\bet.py --tests
```

After running the script you should see some output similar to the following...

```
[tests.py] Running Catch2 tests...

===============================================================================
All tests passed (4 assertions in 1 test case)

[tests.py] bet! Successfully ran tests.
```
