#!/bin/bash

SCRIPT_PATH=`dirname $0`
SCRIPT_PATH=`dirname $SCRIPT_PATH`
OUTPUT_PATH=$1
BUILDDIR=release

cd $SCRIPT_PATH

# Building 'aftnws' solution package

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

echo "Copy aftnws binaries"
cp -P $SCRIPT_PATH/bin/aftnws* $OUTPUT_PATH/bin/
cp -P $SCRIPT_PATH/share/translations/*.qm $OUTPUT_PATH/share/translations/
cp -P $SCRIPT_PATH/etc/default-config.xml $OUTPUT_PATH/etc/aftnws.xml

cd ..	# cd $SCRIPT_PATH

