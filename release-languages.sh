#/bin/sh
# Helper for Qt5 libs to generate all Octopkg translations

TRANSLATIONS=./resources/translations/*

for f in $TRANSLATIONS
do
  lrelease-qt5 $f
done
