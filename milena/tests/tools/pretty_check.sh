#!/bin/sh

# Copyright (C) 2009 EPITA Research and Development Laboratory (LRDE)
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


# ##################
# # How to use it? #
# ##################
#
#   -----------------
#   | Manual launch |
#   -----------------
#
#   Launch the script and pass the path to milena/tests as argument.
#
#     ./launch_tests /my/absolute/or/relative/path/to/milena/tests
#
# You may pass the path to a subdirectory of milena/tests if you want
# to run only a sub part of the test suite.
#
#
#   --------------
#   | Using Make |
#   --------------
#
#   In the build directory, go to milena/tests. Just run:
#
#     make pretty_check
#
#   A subdirectoy can also be passed to the script. Do as follow:
#
#     TEST_DIR=accu make pretty_check
#
#   Here, the pretty check will be run only in milena/tests/accu.
#
#
# Results are stored in a newly created directory ('tmp', by default).
# This directory is created in the directory from where you call this
# script.
#
# It contains two files:
#   - check.log,    the raw log of the test suite.
#   - tests.html,   the formated test suite results.
#
#
# Useful Global variables to know:
#   - TEST_DIR	  Run the tests only in this directory.
#   - TEST_CXX	  Use a specific compiler.
#
#
#
# _WARNING_: be aware of the fact this script run a make clean in the
#	     given path before running the tests. PREVIOUS TEST RESULTS
#	     ARE ALSO CLEARED.
#
#
# KNOWN BUGS: this script is supposed to sort compilation warnings and
#	      compilation errors separately. For unknown reasons, it is
#	      still buggy and errors and warnings may be mixed up in the
#	      formated page. Does anyone want to fix that?
#

output_directory=tmp
output_file=tests.html
check_log="$PWD/$output_directory/check.log"
base_url_data="http://www.lrde.epita.fr/~lazzara/uttk"

base_make_path=$1
if [ -z "$base_make_path" ]; then
  if [ -z "$TEST_DIR" ]; then
    base_make_path="."
  else
    base_make_path="$TEST_DIR"
  fi
fi

if [ -z "$TEST_CXX" ]; then
  if [ -z "$CXX" ]; then
    TEST_CXX="g++"
  else
    TEST_CXX="$CXX"
  fi
fi

gcc_wrapper="$PWD/$output_directory/gcc-wrapper.sh"

cleanup_last_results()
{
  make -C $1 clean
  find . -name '*.log' -exec rm -f {} \;
  rm -Rf $output_directory
  mkdir $output_directory
}

cleanup_tmp_files()
{
  rm -f *.tmp
}

begin_output_file()
{
  cat > $output_file << EOF
<html>
  <head>
    <script language="javascript" src="$base_url_data/javascripts/uttk.js"></script>
    <style type="text/css">
      @import url('$base_url_data/stylesheets/uttk.css');
    </style>
    </head>
  <body>
    <div id="uttk_contents">
EOF
}

end_output_file()
{
  cat >> $output_file << EOF
    </div>
  </body>
</html>
EOF
}

begin_tmp_files()
{
  echo "<a name="top">Failed tests:</a><ol>" >> error_entry.html.tmp
  echo "<a name="top">Compilation warnings:</a><ol>" >> warning_entry.html.tmp
}

end_tmp_files()
{
  local ef="error_entry_html.tmp"
  local wf="warning_entry_html.tmp"
  close_dir_entry $ef
  close_dir_entry $wf
  echo "</ol><br>" >> $ef
  echo "</ol><br>" >> $wf
  close_dir error_node.html.tmp
  close_dir warning_node.html.tmp
}

make_output_file()
{
  begin_output_file

  echo "<table><tr><td valign=top>" >> $output_file
  cat error_entry.html.tmp >> $output_file
  echo "</td><td valign=top>" >> $output_file
  cat warning_entry.html.tmp >> $output_file
  echo "</td></tr></table><br><br>" >> $output_file

  cat error_node.html.tmp >> $output_file
  echo "<br><hr><br>" >> $output_file
  cat warning_node.html.tmp >> $output_file

  end_output_file
}

open_dir()
{
 cat >> $2 << EOF
  <div class="node">
    <table class="title" id="FAIL">
      <tr>
	<td width="11px" <img src="$base_url_data/images/triangle.gif"/></td>
	<td>$1</td>
	<td width="20px">$3</td>
      </tr>
    </table>
    <br>
EOF

}

close_dir()
{
  echo "</div><br><br><br>" >> $1
}


filtered_errors()
{
  # Convert '>' and '<' to html characters.
  if [ -s $1 ]; then
    cat $1 | sed -e 's/</\&lt;/g' | sed -e 's/>/\&gt;/g' | sed -e 's/mln:://g'  | sed -e 's/point<grid::square, short int>/point2d/g' | sed -e 's/error/<b>error<\/b>/g' | sed -e 's/warning/<b>warning<\/b>/g' | sed -e 's/Assertion/<b>Assertion<\/b>/g' | sed -e ':a;N;$!ba;s/\n/<br>/g' >> $2
  fi
}

last_dir=""
last_wdir=""
open_dir_if_needed()
{
  local dir=$1
  local current_dir=$last_dir
  if [ "$3" == "ABORT" ]; then
    current_dir=$last_wdir
  fi

  if [ "$current_dir" != "$dir" ]; then
    if ! [ -z "$current_dir" ]; then
      close_dir $2
    fi
    open_dir $dir $2 $3
    if [ "$3" == "FAIL" ] || [ "$3" == "XFAIL" ]; then
      last_dir=$dir
    else
      last_wdir=$dir
    fi
  fi
}


error_node()
{
  local name=$1
  local file=$2
  local command=$3
  local log=$4
  local output=$6_node.html.tmp
  local class_id=$7
  open_dir_if_needed $5 $output $class_id
  cat >> $output << EOF
  <div class="node">
  <table class="title" id="$class_id">
    <tr>
      <td width="11px" <img src="$base_url_data/images/triangle.gif"/></td>
      <td><a name="$name">$name</a></td>
      <td width="20px"><a href="#top">^ TOP</a></td>
    </tr>
  </table>
  <div class="leaf">
    <table class="title" id="">
      <tr>
	<td>command: </td>
	<td><pre class="attribute_alone">$command</pre></td>
      </tr>
      <tr><td colspan=2>&nbsp;</td></tr>
      <tr>
	<td> Compilation warnings/errors:</td>
	<td>
EOF
  filtered_errors $log.comp.log $output

  cat >> $output << EOF
	</td>
      </tr>
      <tr><td colspan=2>&nbsp;</td></tr>
      <tr>
	<td> Error output:</td>
	<td>
EOF

  filtered_errors $log.log $output

  cat >> $output << EOF
	</td>
      </tr>
      </table>
    </div>

  </div>
  <br>
EOF
}

close_dir_entry()
{
  echo "</ol>" >> $1
}

open_dir_entry()
{
  echo "<li><b>$1</b></li><ol>" >> $2
}

last_dir_entry=""
last_wdir_entry=""
open_dir_entry_if_needed()
{
  local dir=$1
  current_dir=$last_dir_entry
  if [ "$3" == "ABORT" ]; then
    current_dir=$last_wdir_entry
  fi
  if [ "$current_dir" != "$dir" ]; then
    if ! [ -z "$current_dir" ]; then
      close_dir_entry $2
    fi
    open_dir_entry $dir $2
    if [ "$3" == "FAIL" ] || [ "$3" == "XFAIL" ]; then
      last_dir_entry=$dir
    else
      last_wdir_entry=$dir
    fi
  fi
}

error_entry()
{
  local name=$1
  local dir=$2
  local output=$3_entry.html.tmp
  open_dir_entry_if_needed $dir $output $4
  echo "<li><a href=\"#$name\">$name</a></li>" >> $output
}


make_gcc_wrapper()
{
  cat >> "$gcc_wrapper" << EOF
#/bin/sh
# Automatically generated. Do not modify.

#############################################################
#							    #
# Wrapper around GCC in order to log the compilation output #
# for each file.					    #
#							    #
#############################################################

next=0
file=""

#Try to catch the test name.
for arg in \$@; do
  if [ \$next -eq 1 ]; then
    file=\$arg
    break
  fi
  if [ "\$arg" == "-o" ]; then
    next=1
  fi
done

name=\`basename \$file .o\`

# Compile and log the compilation output.
$TEST_CXX 2>\$name.comp.log \$@
EOF

  chmod +x "$gcc_wrapper"
}

cleanup_gcc_wrapper()
{
  rm "$gcc_wrapper"
}


main()
{
  cleanup_last_results $base_make_path

  make_gcc_wrapper

  make CXX="$gcc_wrapper" -C $base_make_path check -k 1>$check_log 2>&1
  rvalue=$?

  cd $output_directory
  begin_tmp_files

  for tdir in `find ../$base_make_path -maxdepth 0 -type d | grep -v "$output_directory" | grep -v "\.svn" | grep -vE "^../$"`; do
    for log in `find $tdir -type f -name '*.comp.log'`; do
      echo "processing $log"
      local name=`basename $log .comp.log`
      local dir=`dirname $log`
      local bdir=`echo $dir | sed -e 's/\.\.\///g'`
      local status=`grep ": $name\$" $check_log | cut -d ':' -f 1`

      # Empty compilation logs
      # Did compile without error/warning.
      if ! [ -s $log ]; then

	# Status FAIL or XFAIL found in logs.
	# failed during execution.
	if [ "$status" == "FAIL" ] || [ "$status" == "XFAIL" ]; then

	  grep -B 1 "X*FAIL: $name\$" $check_log | head -n 1 > "$dir/$name.log"
          error_entry $name $bdir error $status
          error_node $name $log "./$name" "$dir/$name" $bdir error $status
	fi

        # Empty compilation and runtime logs.
	# Everything went well. Not reporting anything.


      # Compilation logs are not empty.
      # There were errors or warnings at compile time.
      else
	# No test status found in logs.
	# Did not compile.
	if [ -z "$status" ]; then
	  error_entry $name $bdir error FAIL
          error_node $name $log "./$name" "$dir/$name" $bdir error FAIL

	# Status FAIL or XFAIL found in logs.
	# Did compile with warnings and failed at runtime.
	elif [ "$status" == "FAIL" ] || [ "$status" == "XFAIL" ]; then
	  error_entry $name $bdir error $status
          error_node $name $log "./$name" "$dir/$name" $bdir error $status

	# Status PASS.
	# Compiled with warnings and passed at runtime.
	else
          error_entry $name $bdir warning ABORT
          error_node $name $log "./$name" "$dir/$name" $bdir warning ABORT
	fi
      fi
    done
  done

  # Build pretty output.
  end_tmp_files
  make_output_file

  # Cleanup temporary files.
  cleanup_tmp_files
  cleanup_gcc_wrapper

  exit $rvalue
}

main
