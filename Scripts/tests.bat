@ECHO OFF

:: 
:: Runs engine tests
:: NOTE: Must build tests using Visual Studio or MSBuild BEFORE running this script
:: 
:: Launch VS > Right-click 'Tests' > Select 'Build'
::

CALL :resolvepath "%~dp0.."
GOTO :eof

:resolvepath

:: Optional: Build solution from scratch before running tests
:: CALL msbuild %~f1/sad.sln -interactive:False -p:Configuration=Debug

CMD /c %~f1/Build/Bin/Tests/Tests.exe 
PAUSE
GOTO :eof
