@ECHO OFF

:: 
:: Uses premake to build the project solution for vs2022
::

CALL :resolvepath "%~dp0.."
GOTO :eof

:resolvepath
CALL %~f1/Scripts/premake5/premake5.exe vs2022 --file="%~f1\premake5.lua" --verbose
GOTO :eof