cd ..
mkdir master-release
cd master-release
cmake ../master -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
mingw32-make -j4
cd ..