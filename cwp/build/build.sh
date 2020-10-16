#!/bin/bash

SCRIPT_PATH=`dirname $0`
SCRIPT_PATH=`dirname $SCRIPT_PATH`
OUTPUT_PATH=$1
BUILDDIR=release

cd $SCRIPT_PATH

# Building 'cwp' solution package

if [ -d $BUILDDIR ];
then
    rm -rf $BUILDDIR
fi

mkdir $BUILDDIR

echo "Start building ..."

cd $BUILDDIR
cmake $SCRIPT_PATH -DCMAKE_BUILD_TYPE=Release
make
cd ..

echo "Build finished ..."

# Copy files for distribution ...

echo "Copy cwp binaries"
cp -P $SCRIPT_PATH/bin/cwp* $OUTPUT_PATH/bin/
cp -P $SCRIPT_PATH/lib/libasterix.so $OUTPUT_PATH/lib/
cp -P $SCRIPT_PATH/lib/libXtProto.so $OUTPUT_PATH/lib/
cp -P $SCRIPT_PATH/share/translations/*.qm $OUTPUT_PATH/share/translations/
cp -P $SCRIPT_PATH/etc/cwp.xml $OUTPUT_PATH/etc/cwp.xml

cd ..	# cd $SCRIPT_PATH

