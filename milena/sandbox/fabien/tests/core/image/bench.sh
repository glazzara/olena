#! /bin/sh

make disk_ima
make tiled2d

echo "---"
echo "Disk Image Performance"
time ./disk_ima $1

echo "---"
echo "Tiled Image Performance"
time ./tiled2d $1
