#! /bin/sh

# Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE).
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


# This file is run by the LRDE autobuilder before compilation.
# It is not meant to be distributed with Olena.

set -ex

# Buildbot will tell us the name of the branch being compiled using $1.
branch=$1

# We might want to filter stats for specific branches.
# Currently gitstats does not allow that.
case "$branch" in
#  # Consider these branches only.
  master|next|swilena|mesh-segm-skel|stable/scribo|unstable/scribo) ;;
#  # Don't upload other branches.
  *) exit ;;
esac

DEST=/lrde/dload/olena/stats/

# option '-c commit_end=$branch' is not needed since buildbot and git
# only checkout the current branch as 'master'.
gitstats . $DEST/$branch


