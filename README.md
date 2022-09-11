<h3 align="center">😔</h3> 

### Quick Install
If you're confident you don't need to read the rest of the instructions, recursively clone the repo and run the `bet.bat` script.

### Installation
Luckily most of the setup for the repository has been automated and as such no external dependencies need to be manually installed other than a default [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) installation.

1. Recursively clone the repository and its submodules

```
$ git clone --recursive git@github.com:focus-fire/sad.git

or

$ git clone --recursive https://github.com/focus-fire/sad.git
```

**Note:** If you're using a GUI client for git such as GitHub Desktop or SourceTree look for a 'Recursive Clone' option. Otherwise copy the command above, right click your file explorer, run it in git bash, and then use GitHub Desktop for the rest of the installation. 

If the repository is not cloned recursively **nothing will work**.

2. Double-click the `build.bat` script or run it from a terminal

```
$ .\Scripts\build.bat
```

This step will have to be rerun **each** time any premake file (`*.premake5.lua*`) is changed. Please note that personal changes to the individual Visual Studio solution *may* not persist once this script is re-executed.  

3. Open the `sad.sln` solution file in Visual Studio 2022

### Tests
This project is setup to use [Catch2](https://github.com/catchorg/Catch2) for unit and integration testing. 

#### Running Tests

1. Build the `Tests` project using Visual Studio or MSBuild (might have to be manually added to your system's environment variables)

```
Launch Visual Studio > Right-click the 'Tests' project > Select 'Build'

or

$ msbuild sad.sln
```

2. Double-click the `tests.bat` script or run it from a terminal

```
$ .\Scripts\tests.bat
```

After running the script you should see some output similar to the following...

```
===============================================================================
All tests passed (4 assertions in 1 test case)

Press any key to continue . . .
```
