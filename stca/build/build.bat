pushd %~dp0

set PROJECT_DIR=%1
rem echo %PROJECT_DIR%
set DIST_DIR=%2
rem echo %DIST_DIR%
set BUILD_DIR=%PROJECT_DIR%\release\stca
rem echo %BUILD_DIR%

rem Prepare build directory
rd /s /q %BUILD_DIR%
md %BUILD_DIR%

cd %BUILD_DIR%

rem Start building 'stca' project
qmake %PROJECT_DIR%/stcaSolution.pro CONFIG+=release
mingw32-make

rem Copy built application files
copy /D /Y "%PROJECT_DIR%\stca\bin\stca.exe" "%DIST_DIR%\bin"
copy /D /Y "%PROJECT_DIR%\rewarn\bin\rewarn.dll" "%DIST_DIR%\bin"
copy /D /Y "%PROJECT_DIR%\..\common\protocols\asterix\bin\asterix.dll" "%DIST_DIR%\bin"
copy /D /Y "%PROJECT_DIR%\..\common\coordsys\bin\CoordSys.dll" "%DIST_DIR%\bin"
xcopy /Y /Q /S /I "%PROJECT_DIR%\stca\share\translations\*.qm" "%DIST_DIR%\share\translations"
copy /Y "%PROJECT_DIR%\stca\etc\config.xml" "%DIST_DIR%\etc\stca.xml"

popd