@ECHO OFF

REM This build script was copied from OnlineVideos and then later changed.
REM All credit for this script goes to OnlineVideos.
REM Thank you!
REM https://code.google.com/p/mp-onlinevideos2/

REM detect if BUILD_TYPE should be release or debug
if not %1!==Debug! goto RELEASE
:DEBUG
set BUILD_TYPE=Debug
goto START
:RELEASE
set BUILD_TYPE=Release
set BUILD_TYPE_16x9=Release_16x9
goto START


:START
REM Select program path based on current machine environment
set progpath=%ProgramFiles%
if not "%ProgramFiles(x86)%".=="". set progpath=%ProgramFiles(x86)%


REM set logfile where the infos are written to, and clear that file
mkdir Logs
set LOG=Logs\Build_%BUILD_TYPE%.log
echo. > %LOG%


echo.
echo Building Atmowin
echo Build mode: %BUILD_TYPE%
echo.

REM : NOT USED FOR ATMOWIN : @"%WINDIR%\Microsoft.NET\Framework\v4.0.30319\MSBUILD.exe" NuGet/RestorePackages.targets

"%WINDIR%\Microsoft.NET\Framework\v4.0.30319\MSBUILD.exe" /target:Rebuild /property:VisualStudioVersion=12.0;Configuration=%BUILD_TYPE% "..\AtmoWinA.sln" >> %LOG%

if %1!==Debug! goto END

echo.
echo Building Atmowin 16x9
echo Build mode: %BUILD_TYPE_16x9%
echo.

REM : NOT USED FOR ATMOWIN : @"%WINDIR%\Microsoft.NET\Framework\v4.0.30319\MSBUILD.exe" NuGet/RestorePackages.targets

"%WINDIR%\Microsoft.NET\Framework\v4.0.30319\MSBUILD.exe" /target:Rebuild /property:VisualStudioVersion=12.0;Configuration=%BUILD_TYPE_16x9% "..\AtmoWinA.sln" >> %LOG%

echo Building MPEI
copy "..\MPEI\AtmoWin.xmp2" "..\MPEI\AtmoWin_COPY.xmp2"
"%progpath%\Team MediaPortal\MediaPortal\MpeMaker.exe" "..\MPEI\AtmoWin_COPY.xmp2" /B >> %LOG%
del "..\MPEI\AtmoWin_COPY.xmp2"

:END