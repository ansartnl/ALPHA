pushd %~dp0

set PROJECT_DIR=%1
rem echo %PROJECT_DIR%
set DIST_DIR=%2
rem echo %DIST_DIR%
set BUILD_DIR=%PROJECT_DIR%\release
rem echo %BUILD_DIR%

rem Prepare build directory
rd /s /q %BUILD_DIR%
md %BUILD_DIR%

cd %BUILD_DIR%

rem Start building aftnws project
cmake ../ -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
mingw32-make

rem Copy built application files
copy /D /Y "%PROJECT_DIR%\bin\aftnws.exe" "%DIST_DIR%\bin"
xcopy /Y /Q /S /I "%PROJECT_DIR%\share\translations\*.qm" "%DIST_DIR%\share\translations"
copy /Y "%PROJECT_DIR%\etc\default-config.xml" "%DIST_DIR%\etc\aftnws.xml"

popd