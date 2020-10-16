mkdir aftnws-release
cd aftnws-release
cmake ../aftnws -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
mingw32-make -j4
mingw32-make -j4 cpack
cd ..
pause