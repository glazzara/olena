#!/bin/sh

SRC_DIR=`dirname $0`;

if [ -z "$1" ]
then
    echo "Please give an output type !"
else
    if [ -z "$2" ]
    then
	echo "Please give input XML file !"
    else

#	HTML
	if [ "$1" = "html" ]
	then
	    which xsltproc > /dev/null;
	    if [ $? -eq 0 ]
	    then
		out="$3";
		if [ -z "$out" ]
		then
		    out="output.html"
		fi
		out=`readlink -f $out`

		echo "HTML output : $out"
		xsltproc "$SRC_DIR/html/main.xsl" "$2" > "$out"

		rm -f $2;
	    fi
#	HTML BASE 64
	elif [ "$1" = "html-base64" ]
	then
	    which xsltproc > /dev/null;
	    if [ $? -eq 0 ]
	    then
		out="$3";
		if [ -z "$out" ]
		then
		    out="output.html"
		fi
		out=`readlink -f $out`

		echo "HTML output : $out"
		xsltproc "$SRC_DIR/html/main_base64.xsl" "$2" > "$out"
	    fi

#	PDF/SVG
	elif [ -n "`echo $1 | grep -E 'pdf(\-base64)?|svg'`" ]
	then
	    which fop > /dev/null;
	    if [ $? -eq 0 ]
	    then
		out="$3";

#		PDF
		if [ "$1" = "pdf" ]
		then
		    if [ -z "$out" ]
		    then
			out="output.pdf"
		    fi
		    out=`readlink -f $out`

		    echo "PDF output : $out"
		    fop -xsl "$SRC_DIR/pdf/main.xsl" -xml "$2" -pdf "$out";

#		SVG
		elif [ "$1" = "svg" ]
		then
		    if [ -z "$out" ]
		    then
			out="output.svg"
		    fi
		    out=`readlink -f $out`

		    echo "SVG output : $out"
		    fop -xsl "$SRC_DIR/svg/main.xsl" -xml "$2" -svg "$out"
		else
		    echo "Wrong output format !";
		fi
	    else
		echo "fop >= 0.95 required !"
	    fi

#	Open Document
	elif [ "$1" = "oo-doc" ]
	then
	    which xsltproc > /dev/null;
	    if [ $? -eq 0 ]
	    then

		out="$3";
		if [ -z "$out" ]
		then
		    out="output.odt"
		fi
		out=`readlink -f $out`

		xsltproc "$SRC_DIR/html/main.xsl" "$2/output.xml" > "$2/out.html"

		echo "Open Document output : $out"
		"$SRC_DIR/ooconvert" "$2/out.html" "$2/out.odt" > /dev/null

		cur_dir=`pwd`;
		tmp_dir="$2/oo_tmp"

 		mkdir $tmp_dir
		unzip $tmp_dir/../out.odt -d $tmp_dir > /dev/null

		cat $tmp_dir/content.xml | sed -re 's!\.\./([a-zA-Z0-9])!Pictures/\1!g' >> $tmp_dir/tmp.xml

		mkdir $tmp_dir/Pictures

		mv $2/*.png $tmp_dir/Pictures/

		mv $tmp_dir/tmp.xml $tmp_dir/content.xml

		cd $tmp_dir
		zip zip.odt -r * > /dev/null
		cp zip.odt "$out"
		cd $cur_dir;

		rm -r $2;
	    else
		echo "xsltproc required !"
	    fi

	else
	    echo "Wrong output format !";
	fi
    fi
fi