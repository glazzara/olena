# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE).
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

dname="`dirname $0`"
if [ "$dname" = "." ]; then
  export PATH="$PWD:$PATH"
  export MILENA_DEVTOOLS_PATH="$PWD"
elif [ -d "$PWD/$dname" ]; then
  export PATH="$PWD/$MILENA_DEVTOOLS_PATH:$PATH"
  export MILENA_DEVTOOLS_PATH="$PWD/$dname"
else
  echo "Cannot guess Milena's devtools path. Please set MILENA_DEVTOOLS_PATH variable manually." 
fi
