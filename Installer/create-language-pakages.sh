#!/bin/bash

#pakunpak bin
PAKUNPAK_BIN='../bin/tools/pakunpak'
LANGUAGES_DIR='../resources/translations'

# change to script directory
cd `dirname "$0"`

# create updates dir
mkdir $LANGUAGES_DIR/updates

# english package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/english_uk_lang.update" "$LANGUAGES_DIR/definitions/english_uk.language"

# spanish package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/espanol_es_lang.update" "$LANGUAGES_DIR/xVST_es.qm" "$LANGUAGES_DIR/definitions/espanol.language"

# catala package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/catala_ca_lang.update" "$LANGUAGES_DIR/xVST_ca.qm" "$LANGUAGES_DIR/definitions/catala.language"