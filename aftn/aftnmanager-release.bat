mkdir aftnmanager-release
cd aftnmanager-release
cmake ../aftnmanager -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
mingw32-make -j4 cpack
cd ..
