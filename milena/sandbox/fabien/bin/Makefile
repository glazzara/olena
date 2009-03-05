GDCM_SRC_DIR = /Users/HiSoKa/Downloads/gdcm-2.0.10
GDCM_BIN_DIR = /Users/HiSoKa/Downloads/gdcmbin

DICOM_INC = -I${GDCM_SRC_DIR}/Source/Common/ \
	    -I${GDCM_BIN_DIR}/Source/Common/ \
	    -I${GDCM_SRC_DIR}/Source/DataDictionary/ \
	    -I${GDCM_SRC_DIR}/Source/MediaStorageAndFileFormat/ \
	    -I${GDCM_SRC_DIR}/Source/DataStructureAndEncodingDefinition/

# "-framework CoreFoundation" is a Mac OS X specific flag
DICOM_LIB = -L${GDCM_BIN_DIR}/bin \
	    -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib \
	    -framework CoreFoundation

CXXFLAGS = -DNDEBUG -O1

all: mask dump

mask: dicom_mask.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o dicom_mask

dump: dicom2dump.cc
	g++ -I../../../ ${DICOM_INC} ${DICOM_LIB} ${CXXFLAGS} $^ -o dicom2dump
