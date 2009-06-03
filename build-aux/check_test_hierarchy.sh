#! /bin/sh

failed_tests=0
all_tests=0

check_directory ()
{
  echo "Current directories: $1 $2"
  for file in `ls $1`; do

    source_file=${file%.cc}.hh
    if [ -f ${1}/$file ]; then
      all_tests=$(($all_tests + 1))
      if [ -f ${2}/$source_file ]; then
	echo "\t\e[0;32mOK\e[m $file <-> $source_file"
      else
# FIXME: Manage exceptions.
	if [ "$file" != "Makefile.am" ]; then
	  echo "\t\e[0;31mFAIL\e[m $source_file source does not exist."
	  failed_tests=$(($failed_tests + 1))
	fi
      fi
    fi

    if [ -d ${1}/$file ]; then
      if [ -d ${2}/$file ]; then
	echo ""
	check_directory ${1}/$file ${2}/$file
      else
# FIXME: Manage exceptions.
	echo "\e[0;31mFAIL\e[m ${1}/$file source directory does not exist."
      fi
    fi

  done
}

if [ $# -ne 2 ]; then
  echo "Usage: ./check_test_hierarchy tests/ mln/"
  exit 1
fi

echo "Test directory: $1"
echo "Source directory: $2"
echo "---"

if [ -d ${1} ]; then
  if [ -d ${2} ]; then
    check_directory ${1} ${2}
  else
    echo "\e[0;31mFAIL\e[m ${1} source directory does not exist."
  fi
fi

echo "\n====="
echo "Uncorrect tests: $failed_tests"

