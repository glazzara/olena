#!/bin/sh

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


# Set of useful functions to generate HTML reports.


# $1: report file.
close_dir_entry()
{
  echo "</ol>" >> $1
}

# $1: entry name.
# $2: report file.
open_dir_entry()
{
  echo "<li><b>$1</b></li><ol>" >> $2
}



# $1: report file.
# $2: report title.
prepare_report()
{
    cat >> $1 << EOF
<html>
  <head>
    <script language="javascript" src="http://api.uttk.org/javascripts/uttk.js"></script>
    <style type="text/css">
      @import url('http://api.uttk.org/stylesheets/uttk.css');
    </style>
    <title>
     $2
    </title>
    </head>
  <body>
  <h1>$2</h2>
EOF

}


# $1: report file.
end_report()
{
    close_dir_entry $1
    cat >> $1 << EOF
  </body>
</html>
EOF
}