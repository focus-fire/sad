@ECHO OFF

::
:: Cleans premake5 output and vs intermediates
:: Useful if frequently making adjustments to premake files for a sanity check
::

ECHO Cleaning intermediates...
@RD /S /Q ".vs"
@RD /S /Q "Obj"

ECHO Cleaning auto-generated solution files...
DEL /Q "sad.sln"
DEL /Q "Code\*.vcxproj*"
DEL /Q "Vendor\*.vcxproj*"

ECHO Successfully cleaned premake and vs2022 output!