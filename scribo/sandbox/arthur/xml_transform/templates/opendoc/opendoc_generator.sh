#!/bin/sh

if [ -z "$1" ]
then
    echo "opendoc_generator <odt|odf>";
else
    if [ "$1" == "odt" ]
    then
	echo "saving to output.odt";
	xsltproc xsl.xsl output.xml > output.html
	ooconvert output.html output.odt
	rm output.html
    elif [ "$1" == "odf" ]
    then
	echo "saving to output.odf";
	xsltproc xsl.xsl output.xml > output.html
	ooconvert output.html output.odf;
	rm output.html
    else
	echo "opendoc_generator <odt|odf>";
    fi
fi

#rm output.html