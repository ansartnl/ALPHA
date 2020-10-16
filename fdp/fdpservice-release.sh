mkdir fdpservice-release
cd fdpservice-release
cmake ../fdpservice -DCMAKE_BUILD_TYPE=Release
make -j4
cd ..
