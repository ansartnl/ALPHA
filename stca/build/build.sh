#!/bin/bash

SCRIPT_PATH=`dirname $0`
SCRIPT_PATH=`dirname $SCRIPT_PATH`
OUTPUT_PATH=$1
BUILDDIR=release

cd $SCRIPT_PATH

# Building 'stca' solution package

if [ -d $BUILDDIR ];
then
    rm -rf $BUILDDIR
fi

mkdir $BUILDDIR

echo "Start building ..."

cd $BUILDDIR
qmake $SCRIPT_PATH/stcaSolution.pro CONFIG+=release
make
cd ..

echo "Build finished ..."

# Copy files for distribution ...

echo "Copy stca binaries"
cp -P $SCRIPT_PATH/stca/bin/stca $OUTPUT_PATH/bin/
cp -P $SCRIPT_PATH/stca/etc/config.xml $OUTPUT_PATH/etc/stca.xml
cp -P $SCRIPT_PATH/rewarn/bin/librewarn.so* $OUTPUT_PATH/lib/

cd ..	# cd $SCRIPT_PATH

