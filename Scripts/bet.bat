@ECHO OFF

::
:: :peepo-bet:
::

ECHO :peepo-bet:
CALL :resolvepath "%~dp0.."
GOTO :eof

:resolvepath
CALL %~f1\Scripts\clean.bat
CALL %~f1\Scripts\build.bat

:: Optional: Build solution and run tests
:: CALL msbuild %~f1/sad.sln -interactive:False -p:Configuration=Debug
:: CALL %~f1\Scripts\tests.bat

GOTO :eof