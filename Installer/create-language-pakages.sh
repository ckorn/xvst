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
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/catala_cat_lang.update" "$LANGUAGES_DIR/xVST_ca.qm" "$LANGUAGES_DIR/definitions/catala.language"

# czech package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/czech_cz_lang.update" "$LANGUAGES_DIR/xVST_cz.qm" "$LANGUAGES_DIR/definitions/czech.language"

# hungarian package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/hungarian_hu_lang.update" "$LANGUAGES_DIR/xVST_hu.qm" "$LANGUAGES_DIR/definitions/hungarian.language"

# swedish package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/swedish_sv_lang.update" "$LANGUAGES_DIR/xVST_sv.qm" "$LANGUAGES_DIR/definitions/swedish.language"

# french package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/french_fr_lang.update" "$LANGUAGES_DIR/xVST_fr.qm" "$LANGUAGES_DIR/definitions/french.language"

# galician package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/galician_gl_lang.update" "$LANGUAGES_DIR/xVST_gl.qm" "$LANGUAGES_DIR/definitions/galician.language"

# korean package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/korean_kr_lang.update" "$LANGUAGES_DIR/xVST_kr.qm" "$LANGUAGES_DIR/definitions/korean.language"

# polish package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/polish_pl_lang.update" "$LANGUAGES_DIR/xVST_pl.qm" "$LANGUAGES_DIR/definitions/polish.language"

# portugese brazil package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/portuguese_br_lang.update" "$LANGUAGES_DIR/xVST_br.qm" "$LANGUAGES_DIR/definitions/portuguese_br.language"

# danish package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/danish_da_lang.update" "$LANGUAGES_DIR/xVST_da.qm" "$LANGUAGES_DIR/definitions/danish.language"

# romanian package
$PAKUNPAK_BIN -i "$LANGUAGES_DIR/updates/romanian_ro_lang.update" "$LANGUAGES_DIR/xVST_ro.qm" "$LANGUAGES_DIR/definitions/romanian.language"