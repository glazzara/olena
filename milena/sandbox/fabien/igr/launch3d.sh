#!/bin/sh

process_file ()
{
    ./seg3d $1
    ../bin/dump2pbm result_double.dump results/${2}_06_result_double.pbm
    ../bin/dump2pbm result_deviation.dump results/${2}_07_result_deviation.pbm
    
    ../bin/dump2pbm bg.dump results/${2}_01_bg.pbm
    ../bin/dump2pbm obj.dump results/${2}_03_obj.pbm

    ../bin/dump2pbm bg_closed.dump results/${2}_02_bg_closed.pbm
    ../bin/dump2pbm obj_closed.dump results/${2}_04_obj_closed.pbm

    ../bin/dump2ppm regions_color.dump results/${2}_05_regions_colors.ppm
    
    rm *.dump

    mv histo.plot results/${2}.plot
    mv bg_histo.plot results/${2}_bg.plot
    mv obj_histo.plot results/${2}_obj.plot
    mv bg_histo_norm.plot results/${2}_p_bg.plot
    mv obj_histo_norm.plot results/${2}_p_obj.plot
}

process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0052.dcm" "52"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0061.dcm" "61"
process_file "/Users/HiSoKa/Work/IGR/souris18/irm/IM_0064.dcm" "64"
