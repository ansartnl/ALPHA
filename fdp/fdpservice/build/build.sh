#!/bin/bash

SCRIPT_PATH=`dirname $0`
SCRIPT_PATH=`dirname $SCRIPT_PATH`
OUTPUT_PATH=$1
BUILDDIR=release

cd $SCRIPT_PATH

# Building 'fdp' solution package

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

echo "Copy fdp binaries"
cp -P $SCRIPT_PATH/bin/fdpservice $OUTPUT_PATH/bin/
cp -P $SCRIPT_PATH/bin/fdpservice.xml $OUTPUT_PATH/etc/fdpservice.xml

cd ..	# cd $SCRIPT_PATH

