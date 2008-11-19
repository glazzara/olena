#! /bin/sh

IFS="
"
KEYWORD="TODO
FIXME"

print ()
{
    for k in $KEYWORD ; do
	for line in `find src -type f | fgrep -v ".svn" | xargs fgrep -nH $k` ; do
	    info=`echo $line | sed 's/\([^:]*\):\([^:]*\):[	 ]*\(.*\)/in \1 at line \2:/'`
	    text=`echo $line | sed 's/\([^:]*\):\([^:]*\):[	 ]*\(.*\)/\3/'`
	    size=`echo $info | wc -c`

	    void=""
	    for i in `seq $size 80` ; do
		void="$void "
	    done

	    echo "$info$void$text"
	done
    done
}

print | sort
