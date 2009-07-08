#!/bin/sh

# This file is run by the LRDE autobuilder after a successful compilation.
# It is not meant to be distributed with Olena.

set -e
set -x


DEST=/lrde/dload/olena/snapshot/
DEST_DOC=$DEST/doc/milena/

# Retrieve the package version
VERSION=`autoconf --trace='AC_INIT' | cut -d ':' -f 5`

# Always do "cp then mv" when uploading the file, so that someone
# cannot start a download why the destination file is incomplete.

# Upload the tarball
mkdir -p $DEST

# tar.gz
cp -f olena-$VERSION.tar.gz $DEST/olena-$rev.tar.gz.tmp
mv -f $DEST/olena-$rev.tar.gz.tmp $DEST/olena-$rev.tar.gz
chmod -R a+r $DEST/olena-$rev.tar.gz

# tar.bz2
cp -f olena-$VERSION.tar.bz2 $DEST/olena-$rev.tar.bz2.tmp
mv -f $DEST/olena-$rev.tar.bz2.tmp $DEST/olena-$rev.tar.bz2
chmod -R a+r $DEST/olena-$rev.tar.bz2


# Upload a copy of the reference manual and other documentation.
mkdir -p $DEST_DOC

rm -rf $DEST_DOC/user_doc.tmp
rm -rf $DEST_DOC/white_paper_html.tmp

cp -pR milena/doc/user/html $DEST_DOC/user_doc.tmp
cp -pR milena/doc/white_paper/white_paper_html $DEST_DOC/white_paper_html.tmp

cp milena/doc/ref_guide/ref_guide.pdf $DEST_DOC/
cp milena/doc/tutorial/tutorial.pdf $DEST_DOC/
cp milena/doc/user/latex/refman.pdf $DEST_DOC/user_doc.pdf
cp milena/doc/white_paper/white_paper.pdf $DEST_DOC/
chmod -R a+rX $DEST_DOC/

mv -f $DEST_DOC/user_doc $DEST_DOC/user_doc.old || true
mv -f $DEST_DOC/white_paper_html $DEST_DOC/white_paper_html.old || true

mv -f $DEST_DOC/user_doc.tmp $DEST_DOC/user_doc
mv -f $DEST_DOC/white_paper_html.tmp $DEST_DOC/white_paper_html

rm -rf $DEST_DOC/user_doc.old
rm -rf $DEST_DOC/white_paper_html.old
