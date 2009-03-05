#!/bin/zsh

process_file ()
{
      ./seg2d $1
      if [ -f result_double.pbm ]; then
	mv result_double.pbm results/${2}_06_result_double.pbm
	mv result_deviation.pbm results/${2}_07_result_deviation.pbm
      fi

      if [ -f bg.pbm ]; then
	mv bg.pbm results/${2}_01_bg.pbm
	mv obj.pbm results/${2}_03_obj.pbm

	mv bg_closed.pbm results/${2}_02_bg_closed.pbm
	mv obj_closed.pbm results/${2}_04_obj_closed.pbm

	if [ -f regions_color.ppm ]; then
	  mv regions_color.ppm results/${2}_05_regions_color.ppm
	fi

	mv histo.plot results/${2}.plot
	mv bg_histo.plot results/${2}_bg.plot
	mv obj_histo.plot results/${2}_obj.plot
	mv bg_histo_norm.plot results/${2}_p_bg.plot
	mv obj_histo_norm.plot results/${2}_p_obj.plot
      fi
}

process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0049.dcm" "49"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0055.dcm" "55"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0058.dcm" "58"
