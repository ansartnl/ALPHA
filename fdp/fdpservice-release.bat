mkdir fdpservice-release
cd fdpservice-release
cmake ../fdpservice -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
mingw32-make -j4
mingw32-make cpack -j4 
cd ..
pause