DICOM_INC = -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/Common/ \
	    -I/Users/HiSoKa/Downloads/gdcmbin/Source/Common/ \
	    -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/DataDictionary/ \
	    -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/MediaStorageAndFileFormat/ \
	    -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/DataStructureAndEncodingDefinition/
DICOM_LIB = -L/Users/HiSoKa/Downloads/gdcmbin/bin \
	    -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib \
	    -framework CoreFoundation
CXXFLAGS = -DNDEBUG -O1

all: 2d 3d

2d: seg_vol_irm.hh seg2d.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} seg2d.cc -o seg2d

3d: seg_vol_irm.hh seg3d.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} seg3d.cc -o seg3d

grad: grad_clo_and_wshd.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o grad_clo

wst: wst_rag.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o wst_rag

clean:
	rm -rf *.dump *.p?m *.plot *.log *.csv
