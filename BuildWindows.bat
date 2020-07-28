@echo off
echo ===============================Build Script================================
echo ===========================================================================
echo ===========================================================================
echo Enter the name of your desired project folder, it will prefixed with "Build"

:try_again
echo You must fill out the folder name

set /p projpath="Build..."

if not defined projpath goto :try_again
echo ===========================================================================
echo.
echo.
echo ===========================================================================
echo Enter root path of VCPKG, or leave blank to use the default path
echo default path = C://dev/vcpkg
set /p root="Enter Custom Path: "

if not defined root (set toolchain=C://dev/vcpkg/scripts/buildsystems/vcpkg.cmake) else (set toolchain=%root%/scripts/buildsystems/vcpkg.cmake)
echo Using tool chain %toolchain%
echo ===========================================================================
echo.
echo.
echo Pick your IDE, either put the full cmake name or use any of the following shorthand names:
echo ===========================================================================
echo vs2019          = Visual Studio 16 2019
echo vs2017          = Visual Studio 15 2017
echo vs2015          = Visual Studio 14 2015
echo mingw-make      = MinGW Makefiles
echo sublime-mingw   = Sublime Text 2 - MinGW Makefiles (Sublime 3 works too!)
echo cb              = CodeBlocks - MinGW Makefiles
echo eclipse         = Eclipse CDT4 - MinGW Makefiles
echo ===========================================================================
echo Note that if you choose a MinGW build, this project might use C++ 17 features at some point
echo You can get an updated MinGW if need be, here: https://nuwen.net/mingw.html
echo If you do not enter an IDE, the default will just be whatever your cmake default is
echo See "cmake --help" for a list of possible IDEs to generate project files for
set /p ide="Enter IDE choice: "

if not defined ide (

	echo Generating project files at location /Build%projpath%

	cmake -S . -B Build%projpath% -DCMAKE_TOOLCHAIN_FILE=%toolchain%
	goto :end
)

if %ide%==vs2019 (set buildsys="Visual Studio 16 2019")
if %ide%==vs2017 (set buildsys="Visual Studio 15 2017")
if %ide%==vs2015 (set buildsys="Visual Studio 14 2015")
if %ide%==mingw-make (set buildsys="MinGW Makefiles")
if %ide%==sublime-mingw (set buildsys="Sublime Text 2 - MinGW Makefiles")
if %ide%==cb (set buildsys="CodeBlocks - MinGW Makefiles")
if %ide%==eclipse (set buildsys="Eclipse CDT4 - MinGW Makefiles")

if not defined buildsys (
		echo Non listed IDEs currently not supported by this script
		goto :end
	)

echo Generating project files for %buildsys% at location /Build%projpath%

cmake -S . -B Build%projpath% -G %buildsys% -DCMAKE_TOOLCHAIN_FILE=%toolchain%

:end

set root=
set projpath=
set toolchain=
set ide=
set buildsys=
pause