@ECHO OFF 

::
:: cry.bat
:: Shortcut script that executes commands to clean, build, and run the project
:: Using the 'vs2022' toolchain
::
:: Note: Should be run from the root of the repository
::

CALL :clean "%~dp0"
CALL :build "%~dp0"

:: Optional: Run the unit tests (requires ms-build)
:: CALL :tests "%~dp0"

GOTO :eof

:clean
CALL python3 %~f1\Scripts\bet.py --clean
GOTO :eof

:build
CALL python3 %~f1\Scripts\bet.py --build
GOTO :eof

:tests
CALL python3 %~f1\Scripts\bet.py --compile-tests msbuild
GOTO :eof
