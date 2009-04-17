#!/bin/bash

# configure DMG
DMG_NAME='xVST_2_0_0a-intel.dmg'
DMG_TITLE='xVideoServiceThief'
DMG_SIZE='30m'

# files to copy
FILES=( 'xVideoServiceThief.app' 'GPL.txt' )

# configure dirs
APP_DIR='../bin'
TMP_DIR='../bin/TMP'

# change to script directory
cd `dirname "$0"`

# remove temporal dir function
function remove_temporal_dir {
	if [ -d $TMP_DIR ]; then
		echo "Removing temporal directory: $PLUGINS_PRE"
		rm -rv $TMP_DIR
	fi
}

# remove temporal dir
remove_temporal_dir

# create temporal dir
echo "Creating directory: $TMP_DIR"
mkdir $TMP_DIR

# copy DMG files to temporal dir
echo "Copying files to $TMP_DIR"
for FILE in ${FILES[@]}; do
	cp -vR $APP_DIR/$FILE $TMP_DIR/$FILE
done

# build DMG
echo "Creating DMG file"
hdiutil create -size $DMG_SIZE "$DMG_NAME" -srcfolder "$TMP_DIR" -volname "$DMG_TITLE" -format UDZO -imagekey zlib-level=9

# remove temporal dir
remove_temporal_dir