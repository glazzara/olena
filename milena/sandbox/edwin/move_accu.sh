#!/bin/bash

SVN=svn #git ?
MLN_DIR="$MLN_DIR"
FIXME="// ACCU_FIXME do not commit"

#FOR DEBUG 'sed -n' to view modifications, 'sed -i' to replace
#SED="sed -n"
#SED_OPT='gp'

#FOR REAL
SED="sed -i"
SED_OPT='g'



function usage()
{
    echo "Usage: $0 accu_list to_dir"
    exit 1
}


if [ $# -ne 2 ]; then
    usage
fi

if [ -z "$MLN_DIR" ]; then
    echo 'Global variable "$MLN_DIR" undeclared.'
    exit 2
fi


FILE_LIST="`cat $1`"
sub="$2"
SUB="$(echo $sub | tr '[:lower:]' '[:upper:]')"

cd $MLN_DIR
if [ ! \( -d "mln/accu/$sub" -a -d "tests/accu/$sub" \) ]; then
    echo "mln/accu/$sub or tests/accu/$sub not found."
    exit 2
fi

for accu in `cat $1`; do
    if [ ! \( -f "mln/accu/$accu.hh" -a  -f "tests/accu/$accu.cc" \) ]; then
	echo "Header or test file not found."
	echo "Continue ? y / n"
	read c
	if [ "$c" != "y" ]; then
	    exit 2
	fi
    fi

    $SVN mv "mln/accu/$accu.hh" "mln/accu/$sub"
    $SVN mv "tests/accu/$accu.cc" "tests/accu/$sub"

    echo $FIXME >> "mln/accu/$sub/$accu.hh"
    echo $FIXME >> "tests/accu/$sub/$accu.cc"

    # Replacement.
    files="$(find . -name '*.cc' -o -name '*.hh' -o -name '*.hxx')"
    ACCU="$(echo $accu | tr '[:lower:]' '[:upper:]')"
    $SED "s/MLN_ACCU_${ACCU}_HH/MLN_ACCU_${SUB}_${ACCU}_HH/$SED_OPT" "mln/accu/$sub/$accu.hh"
    $SED "s|mln/accu/$accu.hh|mln/accu/$sub/$accu.hh|$SED_OPT" $files
    $SED "s/accu::$accu</accu::$sub::$accu</$SED_OPT" $files
    $SED "s/accu::meta::$accu\([^a-zA-Z_]\)/accu::meta::$sub::$accu\1/$SED_OPT" $files

    # All.hh & essential.hh update.
    $SED "\|# include <mln/accu/$accu.hh>|d" mln/accu/all.hh
    echo "# include <mln/accu/$sub/$accu.hh>" >> "mln/accu/$sub/all.hh"

    if [ "$(grep "# include <mln/accu/$accu.hh>" mln/accu/essential.hh)" ]; then
	if [ ! -f "mln/accu/$sub/essential.hh" ]; then
	    echo "Error: file \"mln/accu/$sub/essential.hh\" not found"
	    exit 2
	fi
	$SED "\|# include <mln/accu/$accu.hh>|d" mln/accu/essential.hh
	echo "# include <mln/accu/$sub/$accu.hh>" >> "mln/accu/$sub/essential.hh"
    fi
done

#All & essential to up.
echo $FIXME >> "mln/accu/$sub/all.hh"
if [ -f "mln/accu/$sub/essential.hh" ]; then
    echo $FIXME >> "mln/accu/$sub/essential.hh"
fi

#Makefile to up.
echo '#' $FIXME >> "tests/accu/Makefile.am"
echo '#' $FIXME >> "tests/accu/$sub/Makefile.am"
