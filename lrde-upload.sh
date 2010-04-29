#! /bin/sh

# This file is run by the LRDE autobuilder after a successful compilation.
# It is not meant to be distributed with Olena.

set -ex

DEST=/lrde/dload/olena/snapshots
DEST_DOC=$DEST/doc/milena

# Retrieve the package version
VERSION=`autoconf --trace='AC_INIT:$2'`
CURRENT_DATE=`date +'%d_%m_%y'`
REV=$VERSION-snapshot-$CURRENT_DATE

# Always do "cp then mv" when uploading the file, so that nobody
# can start a download while the destination file is incomplete.

mkdir -p $DEST

# Delete tarballs older than 2 days.
find $DEST -maxdepth 1 -type f -mtime +1 -name 'olena-*-snapshot-*' \
  -exec rm -f {} \;

# Upload the `.tar.gz' tarball.
cp -f olena-$VERSION.tar.gz $DEST/olena-$REV.tar.gz.tmp
mv -f $DEST/olena-$REV.tar.gz.tmp $DEST/olena-$REV.tar.gz
chmod -R a+r $DEST/olena-$REV.tar.gz

# Upload the `.tar.bz2' tarball.
cp -f olena-$VERSION.tar.bz2 $DEST/olena-$REV.tar.bz2.tmp
mv -f $DEST/olena-$REV.tar.bz2.tmp $DEST/olena-$REV.tar.bz2
chmod -R a+r $DEST/olena-$REV.tar.bz2


# Upload a copy of the reference manual and other documentation.
mkdir -p $DEST_DOC

# BuildBots' buildslaves set umask to 077 in their default
# configuration.  Set read permissions for all on uploaded files.
cp milena/doc/ref-guide.pdf   $DEST_DOC && chmod a+r $DEST_DOC/ref-guide.pdf
cp milena/doc/tutorial.pdf    $DEST_DOC && chmod a+r $DEST_DOC/tutorial.pdf
cp milena/doc/user-refman.pdf $DEST_DOC && chmod a+r $DEST_DOC/user-refman.pdf
cp milena/doc/white-paper.pdf $DEST_DOC && chmod a+r $DEST_DOC/white-paper.pdf

rm -rf $DEST_DOC/user-refman.tmp
rm -rf $DEST_DOC/white-paper.tmp

cp -pR milena/doc/user-refman $DEST_DOC/user-refman.tmp
cp -pR milena/doc/white-paper $DEST_DOC/white-paper.tmp

# Likewise, set permissions for all on uploaded directories.
chmod -R a+rX $DEST_DOC/user-refman.tmp
chmod -R a+rX $DEST_DOC/white-paper.tmp

if test -e $DEST_DOC/user-refman; then
  rm -rf $DEST_DOC/user-refman.old
  mv -f $DEST_DOC/user-refman $DEST_DOC/user-refman.old
fi
if test -e $DEST_DOC/white-paper; then
  rm -rf $DEST_DOC/white-paper.old
  mv -f $DEST_DOC/white-paper $DEST_DOC/white-paper.old
fi

mv -f $DEST_DOC/user-refman.tmp $DEST_DOC/user-refman
mv -f $DEST_DOC/white-paper.tmp $DEST_DOC/white-paper

rm -rf $DEST_DOC/user-refman.old
rm -rf $DEST_DOC/white-paper.old

# We want to be able to modify these files with both the `build' and
# `doc' accounts.
chmod -R g+w $DEST_DOC/user-refman
chmod -R g+w $DEST_DOC/white-paper
