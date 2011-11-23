#! /bin/sh

# Copyright (C) 2009, 2010, 2011 EPITA Research and Development
# Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.


# This file is run by the LRDE autobuilder after a successful compilation.
# It is not meant to be distributed with Olena.

set -ex

# Layout of /lrde/dload/olena/snapshots:
#
#   /lrde/dload/olena/snapshots
#   |-- master
#   |   |-- doc
#   |   |   `-- milena
#   |   |       `-- ...
#   |   |-- olena-$VERSION-snapshot-master-$date.tar.bz2
#   |   `-- olena-$VERSION-snapshot-master-$date.tar.gz
#   |-- next
#   |   |-- doc
#   |   |   `-- milena
#   |   |       `-- ...
#   |   |-- olena-$VERSION-snapshot-next-$date.tar.bz2
#   |   `-- olena-$VERSION-snapshot-next-$date.tar.gz
#  ...
#

# Buildbot will tell us the name of the branch being compiled using $1.
branch=$1

case "$branch" in
  # Consider these branches only.
  master|next|swilena|mesh-segm-skel|stable/scribo|unstable/scribo) ;;
  # Don't upload other branches.
  *) exit ;;
esac

subdir=$branch
suffix=snapshot-$branch

DEST=/lrde/dload/olena/snapshots/$subdir
DEST_DOC=$DEST/doc/milena

# Retrieve the package version
VERSION=`autoconf --trace='AC_INIT:$2'`

date=`date +'%Y-%m-%d'`
REV=$VERSION-$suffix-$date

# Always do "cp then mv" when uploading the file, so that nobody
# can start a download while the destination file is incomplete.

mkdir -p $DEST

# Delete tarballs older than 2 days.
find $DEST -maxdepth 1 -type f -mtime +1 -name 'olena-*-snapshot-*' \
  -exec rm -f {} \;

# Delete old symbolic links.
rm -f $DEST/olena-$VERSION-$suffix.tar.gz $DEST/olena-$VERSION-$suffix.tar.bz2

# Upload the `.tar.gz' tarball.
cp -f olena-$VERSION.tar.gz $DEST/olena-$REV.tar.gz.tmp
mv -f $DEST/olena-$REV.tar.gz.tmp $DEST/olena-$REV.tar.gz
ln -s $DEST/olena-$REV.tar.gz $DEST/olena-$VERSION-$suffix.tar.gz

# Upload the `.tar.bz2' tarball.
cp -f olena-$VERSION.tar.bz2 $DEST/olena-$REV.tar.bz2.tmp
mv -f $DEST/olena-$REV.tar.bz2.tmp $DEST/olena-$REV.tar.bz2
ln -s $DEST/olena-$REV.tar.bz2 $DEST/olena-$VERSION-$suffix.tar.bz2

# Upload a copy of the reference manual and other documentation.
mkdir -p $DEST_DOC

# BuildBots' buildslaves set umask to 077 in their default
# configuration.
cp milena/doc/ref-guide.pdf   $DEST_DOC
cp milena/doc/tutorial.pdf    $DEST_DOC
cp milena/doc/user-refman.pdf $DEST_DOC
cp milena/doc/white-paper.pdf $DEST_DOC

rm -rf $DEST_DOC/user-refman.tmp
rm -rf $DEST_DOC/white-paper.tmp

# Upload only the HTML version (not the LaTeX sources) of the user
# reference manual.
cp -pR milena/doc/user-refman/html $DEST_DOC/user-refman.tmp
cp -pR milena/doc/white-paper $DEST_DOC/white-paper.tmp

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

# Expose uploaded files.
chmod -R a+rX $DEST

# We want to be able to modify these files with both the `build' and
# `doc' accounts.
chmod -R g+w $DEST_DOC/user-refman
chmod -R g+w $DEST_DOC/white-paper
