#!/bin/zsh

process_file ()
{
      ./seg2d $1
      if [ -f result.pbm ]; then
	mv result.pbm results/${2}_06_result.pbm
      fi

      if [ -f bg.pbm ]; then
	mv bg.pbm results/${2}_01_bg.pbm
	mv obj.pbm results/${2}_03_obj.pbm

	mv bg_closed.pbm results/${2}_02_bg_closed.pbm
	mv obj_closed.pbm results/${2}_04_obj_closed.pbm

	mv regions_color.ppm results/${2}_05_regions_color.ppm

	mv bg_histo_norm.plot results/${2}_bg.plot
	mv obj_histo_norm.plot results/${2}_obj.plot
      fi
}

process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0049.dcm" "49"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0055.dcm" "55"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm" "58"
