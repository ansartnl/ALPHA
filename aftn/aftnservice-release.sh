mkdir aftnservice-release
cd aftnservice-release
cmake ../aftnservice -DCMAKE_BUILD_TYPE=Release
make -j4
cd ..
