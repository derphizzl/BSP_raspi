#!/bin/bash

BSPDIR=mybsp
EXECUTABLE=BSP
CONFIG_INI=config.ini
BLACKLIST_INI=blacklist.ini

if [ -d $BSPDIR ];
then
	rm -r $BSPDIR
fi

if [ -e $BSPDIR.tar.gz ];
then
	rm $BSPDIR.tar.gz
fi

mkdir $BSPDIR
cp $EXECUTABLE $BSPDIR
cp $CONFIG_INI $BSPDIR
cp $BLACKLIST_INI $BSPDIR

tar -czf $BSPDIR.tar.gz $BSPDIR
rm -r $BSPDIR
