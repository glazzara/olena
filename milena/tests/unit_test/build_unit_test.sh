#!/bin/sh

if [ $# -ne 1 ]; then
  echo "Usage: $0 <mln_path>"
fi

HEADERS=`find $1 -name "*.hh" | grep -vE "*.spe.hh" | grep -v "mln/core/doc" | sed -e 's/.*\/mln\/\(.*\)/mln\/\1/g' | sed s/"\.\.\/\.\.\/"//g`

rm -f Makefile.am
rm -f *.hh *.cc

#build Makefile.am
echo "## Process this file through Automake to create Makefile.in -*- Makefile -*-" >> Makefile.am
echo ""                                             >> Makefile.am
echo "include \$(top_srcdir)/milena/tests/tests.mk" >> Makefile.am
echo ""                                             >> Makefile.am
echo -n "check_PROGRAMS = "                         >> Makefile.am

for i in $HEADERS; do
    FILE_CC=`echo $i | sed s/"\(\/\|\.\)"/_/g | sed s/_hh/\.cc/g`
#Build .cc
    echo "// Unit test for $i."              >> $FILE_CC
    echo "// Generated file, do not modify." >> $FILE_CC
    echo "#include <$i>"                     >> $FILE_CC
    echo ""                                  >> $FILE_CC
    echo "int main()"                        >> $FILE_CC
    echo "{"                                 >> $FILE_CC
    echo "  // Nothing."                     >> $FILE_CC
    echo "}"                                 >> $FILE_CC

#build Makefile.am
    echo " \\" >> Makefile.am
    echo -n "${FILE_CC}" | sed s/"\.cc"// >> Makefile.am
done

#build Makefile.am
echo "" >> Makefile.am
echo "" >> Makefile.am
for i in $HEADERS; do
    FILE_CC=`echo $i | sed s/"\(\/\|\.\)"/_/g | sed s/_hh/\.cc/g`
    NAME=`echo $FILE_CC | sed s/"\.cc"//`
    echo "${NAME}_SOURCES = $FILE_CC" >> Makefile.am
done
echo "" >> Makefile.am
echo "TESTS = \$(check_PROGRAMS)" >> Makefile.am
