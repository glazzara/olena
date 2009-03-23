CXX = llvm-g++
DCM_SRC=/Users/HiSoKa/Downloads/gdcm-2.0.10
DCM_BIN=/Users/HiSoKa/Downloads/gdcmbin
DICOM_INC = -I${DCM_SRC}/Source/Common/ \
	    -I${DCM_BIN}/Source/Common/ \
	    -I${DCM_SRC}/Source/DataDictionary/ \
	    -I${DCM_SRC}/Source/MediaStorageAndFileFormat/ \
	    -I${DCM_SRC}/Source/DataStructureAndEncodingDefinition/
DICOM_LIBS = -L${DCM_BIN}/bin \
	     -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib \
	     -framework CoreFoundation
CXXFLAGS = -DNDEBUG -O1

all: 2d 3d wsd2d wsd3d grad

2d: seg_vol_irm.hh seg2d.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} seg2d.cc -o seg2d

3d: seg_vol_irm.hh seg3d.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} seg3d.cc -o seg3d

wsd2d: watershed.hh watershed2d.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} $^ -o wsd2d

wsd3d: watershed.hh watershed3d.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} $^ -o wsd3d

wsd3dg: watershed.hh watershed3d.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} -g $^ -o wsd3dg

nbasins: nbasins_finder.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} $^ -o nbasins_finder
	
grad: grad.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} $^ -o grad
	
clo_vol: clo_vol.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o clo_vol

wst: wst.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o wst
	
graph: graph.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} $^ -o graph
	
med: med.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} $^ -o med
	
thres: thres.cc
	${CXX} -I../../../ ${CXXFLAGS} $^ -o thres
	
matlab: matlab.cc
	${CXX} -I../../../ ${DICOM_INC} ${DICOM_LIBS} ${CXXFLAGS} -lm $^ -o matlab
	
clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv *.dSYM
