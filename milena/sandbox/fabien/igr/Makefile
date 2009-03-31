CXX = llvm-g++
CXXFLAGS = -DNDEBUG -O4

DICOM_INC = -I/usr/local/include/gdcm-2.0
DICOM_LIBS = -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib \
	     -framework CoreFoundation
DICOM = ${DICOM_INC} ${DICOM_LIBS}

MAGICK = `Magick++-config --cppflags --ldflags --libs`

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
	${CXX} -I../../../ ${DICOM} ${CXXFLAGS} $^ -o grad
	
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

clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv *.dSYM
	rm seg2d seg3d wsd2d wsd3d nbasins_finder grad clo_vol wst graph med thres matlab time_max first_slice_dicom
