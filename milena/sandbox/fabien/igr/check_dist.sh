#!/bin/sh


process ()
{
  ./dist_edges $1 -1 $2 dist${2}.dump
  mv dist.pgm dist${2}.pgm
}

batch_process ()
{
  process $1 1
  process $1 2
  process $1 3
  process $1 4
  process $1 5
}

batch_process $1 $2
