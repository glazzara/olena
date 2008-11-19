#! /bin/sh

if [ $# -ne 1 ] ; then
    echo "usage : init_tar.sh \"tarball_root_dir\""
    return 1
fi
echo "rm -Rf $1"
rm -Rf "$1"
echo "mkdir $1 2> /dev/null"
mkdir "$1" 2> /dev/null
for file in `ls | grep -v $1` ; do
    echo "ln -fs ../$file $1/$file"
    ln -fs "../$file" "$1/$file"
done
