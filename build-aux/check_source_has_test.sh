#! /bin/sh

failed_tests=0
all_tests=0

check_directory ()
{
  echo "Current directories: $1 $2"

  for file in `ls $1`; do

    if [ -f ${1}/$file ]; then
      ext=${file##*.}
      base=${file%.*}

      if [ "$ext" = "hh" -a \
	   "${base##*.}" != "spe" ]; then
	test_file=${file%.hh}.cc
	all_tests=$(($all_tests + 1))

	if [ -f ${2}/$test_file ]; then
	  echo "\t\e[0;32mOK\e[m $file <-> $test_file" > /dev/null
	else
	  if [ "$file" != "all.hh"  -a \
	    "$file" != "essential.hh" ]; then
	    echo "\tFAIL Test \"$test_file\" does not exist."
	    failed_tests=$(($failed_tests + 1))
	  fi
	fi

      fi

    fi

    if [ -d ${1}/$file ]; then
      if [ -d ${2}/$file ]; then
	echo ""
	check_directory ${1}/$file ${2}/$file
      else
	if [ "$file" != "internal" ]; then
	  echo "FAIL Test directory \"${2}/$file\" does not exist."
	fi
      fi
    fi

  done
}

if [ $# -ne 2 ]; then
  echo "Usage: ./check_test_hierarchy mln/ tests/"
  exit 1
fi

echo "Source directory: $1"
echo "Test directory: $2"
echo "---"

if [ -d ${1} ]; then
  if [ -d ${2} ]; then
    check_directory ${1} ${2}
  else
    echo "\e[0;31mFAIL\e[m Test directory $2 does not exist."
  fi
else
  echo "\e[0;31mFAIL\e[m Source directory $1 does not exist."
fi

echo "\n====="
echo "Missing tests: $failed_tests"

