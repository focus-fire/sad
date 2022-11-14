@ECHO OFF

::
:: cry.bat
:: Shortcut script that executes commands to clean, build, and run the project (vs2022)
:: Note: Should be run from the root of the repository
::

:: Check for submodule updates (this can be commented if it's slowing things down)
CALL :update "%~dp0"

:: Clean previous premake artifacts
CALL :clean "%~dp0"

:: Build the project with premake
CALL :build "%~dp0"

:: Optional: Compile using msbuild
:: CALL :make "%~dp0"

:: Optional: Run the unit tests
:: CALL :tests "%~dp0"

:: Optional: Run the project
:: CALL :execute "%~dp0"

GOTO :eof

:update
CALL python3 %~f1\Scripts\bet.py --update
GOTO :eof

:clean
CALL python3 %~f1\Scripts\bet.py --clean
GOTO :eof

:build
CALL python3 %~f1\Scripts\bet.py --build
GOTO :eof

:make
CALL msbuild sad.sln -property:Configuration=Debug
GOTO :eof

:tests
CALL python3 %~f1\Scripts\bet.py --tests
GOTO :eof

:execute
CALL %~f1\Build\Bin\Game\Game.exe
GOTO :eof
