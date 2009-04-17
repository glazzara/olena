include Makefile.rules

seg2d: seg_vol_irm.hh seg2d.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} seg2d.cc -o seg2d

seg3d: seg_vol_irm.hh seg3d.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} seg3d.cc -o seg3d

wsd2d: watershed.hh watershed2d.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o wsd2d

wsd3d: watershed.hh watershed3d.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o wsd3d

nbasins: nbasins_finder.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o nbasins_finder
	
grad: grad.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o grad
	
clo_vol: clo_vol.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o clo_vol

wst: wst.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o wst
	
graph: graph.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o graph
	
med: med.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o med
	
thres: thres.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o thres
	
matlab: matlab.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} -lm $^ -o matlab
	
time_max: time_max.cc
	${CXX} -I../../../ ${DICOM} ${MAGICK} ${CXXFLAGS} -lm $^ -o time_max
	
first_slice_dicom: first_slice_dicom.cc
	${CXX} -I../../../ ${DICOM} ${MAGICK} ${CXXFLAGS} -lm $^ -o first_slice_dicom

fun_labels: fun_labels.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o fun_labels

mean_slices: mean_slices.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o mean_slices

crop: crop.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o crop

label2gif: label2gif.cc
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o label2gif

norm: norm.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o norm

clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv *.dSYM
	rm seg2d seg3d wsd2d wsd3d nbasins_finder grad clo_vol wst graph med thres matlab time_max first_slice_dicom
