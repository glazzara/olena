DICOM_INC = -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/Common/ \
	    -I/Users/HiSoKa/Downloads/gdcmbin/Source/Common/ \
	    -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/DataDictionary/ \
	    -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/MediaStorageAndFileFormat/ \
	    -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/DataStructureAndEncodingDefinition/
DICOM_LIB = -L/Users/HiSoKa/Downloads/gdcmbin/bin \
	    -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib \
	    -framework CoreFoundation
CXXFLAGS = -DNDEBUG -O1

all: 2d 3d wsd2d wsd3d

2d: seg_vol_irm.hh seg2d.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} seg2d.cc -o seg2d

3d: seg_vol_irm.hh seg3d.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} seg3d.cc -o seg3d

wsd2d: watershed.hh watershed2d.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o wsd2d

wsd3d: watershed.hh watershed3d.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o wsd3d

wsd3dg: watershed.hh watershed3d.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} -DNDEBUG -g $^ -o wsd3dg
	
clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv
