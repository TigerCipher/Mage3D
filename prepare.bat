
@echo off

rem Copy modified/new asset files and delete old files that are no longer in source
robocopy .\assets .\BuildDebug\app\assets\ /mir

rem Copy dlls if needed to build location and delete any dlls no longer used
rem I use CLion, which doesn't create build folders such as Debug, Release, etc.
rem For use with Visual Studio, you'd need to make it \Debug\src for example
robocopy .\BuildDebug\src .\BuildDebug\app *.dll /purge

rem robocopy returns non zero error code even when there is no real error
rem the following lets us return "errors" 1, 2, and 4 as 0

set/A errlev="%ERRORLEVEL% & 24"
exit/B %errlev%