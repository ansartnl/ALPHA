cd ..
mkdir arincmerge-release
cd arincmerge-release
cmake ../arincmerge -DCMAKE_BUILD_TYPE=Release -G"MinGW Makefiles"
mingw32-make cpack
cd ../arincmerge