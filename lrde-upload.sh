#!/bin/sh

# This file is run by the LRDE autobuilder after a successful compilation.
# It is not meant to be distributed with Olena.

set -e
set -x


DEST=/lrde/dload/olena/snapshots
DEST_DOC=$DEST/doc/milena

# Retrieve the package version
VERSION=`autoconf --trace='AC_INIT:$2'`
CURRENT_DATE=`date +'%d_%m_%y'`
REV=$VERSION-snapshot-$CURRENT_DATE

# Always do "cp then mv" when uploading the file, so that someone
# cannot start a download why the destination file is incomplete.

# Upload the tarball
mkdir -p $DEST

# Delete tarballs older than 2 days.
find $DEST -maxdepth 1 -type f -mtime +2 -name 'olena-*-snapshot-*' -exec rm -f {} \;

# tar.gz
cp -f olena-$VERSION.tar.gz $DEST/olena-$REV.tar.gz.tmp
mv -f $DEST/olena-$REV.tar.gz.tmp $DEST/olena-$REV.tar.gz
chmod -R a+r $DEST/olena-$REV.tar.gz

# tar.bz2
cp -f olena-$VERSION.tar.bz2 $DEST/olena-$REV.tar.bz2.tmp
mv -f $DEST/olena-$REV.tar.bz2.tmp $DEST/olena-$REV.tar.bz2
chmod -R a+r $DEST/olena-$REV.tar.bz2


# Upload a copy of the reference manual and other documentation.
mkdir -p $DEST_DOC

rm -rf $DEST_DOC/user_doc.tmp
rm -rf $DEST_DOC/white_paper_html.tmp


# Dist check set umask to 077. Restoring permissions for all
chmod -R a+rx milena/doc/user
chmod -R a+rx milena/doc/white_paper

cp -pR milena/doc/user/html $DEST_DOC/user_doc.tmp
cp -pR milena/doc/white_paper/white_paper_html $DEST_DOC/white_paper_html.tmp

cp milena/doc/ref_guide/ref_guide.pdf $DEST_DOC/
cp milena/doc/tutorial/tutorial.pdf $DEST_DOC/
cp milena/doc/user/latex/refman.pdf $DEST_DOC/user_doc.pdf
cp milena/doc/white_paper/white_paper.pdf $DEST_DOC/

mv -f $DEST_DOC/user_doc $DEST_DOC/user_doc.old || true
mv -f $DEST_DOC/white_paper_html $DEST_DOC/white_paper_html.old || true

mv -f $DEST_DOC/user_doc.tmp $DEST_DOC/user_doc
mv -f $DEST_DOC/white_paper_html.tmp $DEST_DOC/white_paper_html

rm -rf $DEST_DOC/user_doc.old
rm -rf $DEST_DOC/white_paper_html.old
