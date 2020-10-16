mkdir aftnservice-release
cd aftnservice-release
cmake ../aftnservice -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
mingw32-make -j4 cpack
cd ..
pause

