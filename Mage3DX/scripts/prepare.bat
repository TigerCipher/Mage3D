
@echo off

rem Copy modified/new asset files and delete old files that are no longer in source
robocopy .\assets %1\assets /MIR

xcopy ..\settings.ini %1 /Y

rem robocopy returns non zero error code even when there is no real error
rem the following lets us return "errors" 1, 2, and 4 as 0

set/A errlev="%ERRORLEVEL% & 24"
exit/B %errlev%