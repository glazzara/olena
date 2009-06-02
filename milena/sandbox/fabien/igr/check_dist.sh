#!/bin/sh


process ()
{
  ./dist_edges $1 -1 $2 dist${2}.dump
  convert dist.pgm dist${2}.png
  rm dist*.pgm
}

batch_process ()
{
  process $1 1
  process $1 2
  process $1 3
  process $1 4
}

batch_process $1 $2
