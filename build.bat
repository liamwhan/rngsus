@echo off
setlocal
REM if "%WAV_VCVARS_LOCATION%"=="" set WAV_VCVARS_LOCATION="C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvarsall.bat" x64
set path=%cd%\misc;%path%

set CommonCompilerFlags= -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Oi -WX -W4 -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC

REM -MTd: Defines _DEBUG and _MT
REM -MT: Define _MT (use multithreaded static version of the runtime library)
REM -0d: Disable optimizations
REM -Ox: Enable max optimizations
REM -Zo: Enahnce optimized debugging
REM -Z7: Produce object files with full symbolic information
if NOT "%1"=="Release" (set CommonCompilerFlags=%CommonCompilerFlags% -MTd -Od -Zo -Z7) else (set CommonCompilerFlags=%CommonCompilerFlags% -MT -Ox)

set Defines=-DRNG_WIN32=1
if NOT "%1"=="Release" set Defines=-DRNG_SLOW=1 -DRNG_INTERNAL=1
if "%2"=="Test" set Defines=%Defines% -DRNG_INTERNAL=1

set CommonLinkerFlags= -incremental:no -opt:ref -LIBPATH:"%DXSDK_DIR%/Lib/x86" OLE32.LIB d3d11.lib d3dcompiler.lib Shell32.lib Kernel32.lib Pathcch.lib
set Includes=-I..\code -I..\code\imgui -I..\code\imgui\backends -I "%WindowsSdkDir%Include\um" -I "%WindowsSdkDir%Include\shared" -I "%DXSDK_DIR%Include"
set Sources=..\code\win32_rng.cpp ..\code\imgui\backends\imgui_impl_dx11.cpp ..\code\imgui\backends\imgui_impl_win32.cpp ..\code\imgui\imgui*.cpp

set OutputFile=
if "%1"=="Release" set OutputFile=-Fe..\publish\win32_rng.exe

REM Output directory setup
IF NOT EXIST build mkdir build

REM Cleanup build directory
call clean.bat
pushd build

REM Kill any debug instances if they are open (so the build doesn't fail)
taskkill /f /t /im win32_rng.exe > NUL 2> NUL

cl %OutputFile% %CommonCompilerFlags% %Defines% %Includes% -Fmapp.map %Sources% /link %CommonLinkerFlags%
endlocal
popd
