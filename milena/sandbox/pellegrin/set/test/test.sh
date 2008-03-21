#! /bin/sh

lol="line2d.hh p_bgraph.hh p_line_graph.hh p_priority_queue_fast_with_array.hh
p_queue_fast.hh p_run.hh p_set.hh p_array.hh p_graph.hh p_priority_queue_fast.hh
p_priority_queue.hh p_queue.hh p_runs.hh pset_if.hh"
mln_path="../../../../mln"

if [ -f $mln_path/core/stamp ]; then
  exit 1;
fi

for i in $lol;
do
  mv -v $mln_path/core/$i $mln_path/core/$i.bak;
  cp -v ../core/$i $mln_path/core/;
done

touch $mln_path/core/stamp
