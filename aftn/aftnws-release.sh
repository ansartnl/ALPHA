mkdir aftnws-release
cd aftnws-release
cmake ../aftnws -G -DCMAKE_BUILD_TYPE=Release
make -j4
cd ..
