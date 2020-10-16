#!/bin/bash

SCRIPT_PATH=`dirname $0`
SCRIPT_PATH=`dirname $SCRIPT_PATH`
OUTPUT_PATH=$1
BUILDDIR=release

cd $SCRIPT_PATH

# Building 'Environment Editor' solution package

if [ -d $BUILDDIR ];
then
    rm -rf $BUILDDIR
fi

mkdir $BUILDDIR

echo "Start building ..."

cd $BUILDDIR
qmake $SCRIPT_PATH/environmenteditor.pro CONFIG+=release
make
cd ..

echo "Build finished ..."

# Copy files for distribution ...

echo "Copy arincmerge binaries"
cp -P $SCRIPT_PATH/bin/environmenteditor $OUTPUT_PATH/bin/
cp -P $SCRIPT_PATH/share/translations/*.qm $OUTPUT_PATH/share/translations/
cp -P $SCRIPT_PATH/etc/environmenteditor.xml $OUTPUT_PATH/etc/

cd ..	# cd $SCRIPT_PATH

