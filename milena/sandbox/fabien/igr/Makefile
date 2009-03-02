all: seg_vol_irm.cc
	g++ -I../../../ \
	  -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/Common/ \
	  -I/Users/HiSoKa/Downloads/gdcmbin/Source/Common/ \
	  -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/DataDictionary/ \
	  -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/MediaStorageAndFileFormat/ \
	  -I/Users/HiSoKa/Downloads/gdcm-2.0.10/Source/DataStructureAndEncodingDefinition/ \
	  -L/Users/HiSoKa/Downloads/gdcmbin/bin \
	  -lgdcmCommon -lgdcmDICT -lgdcmDSED -lgdcmIOD -lgdcmMSFF -lgdcmexpat -lgdcmjpeg12 -lgdcmjpeg16 -lgdcmjpeg8 -lgdcmopenjpeg -lgdcmuuid -lgdcmzlib \
	  -framework CoreFoundation \
	  -DNDEBUG -O1 \
	  seg_vol_irm.cc -o seg_vol_irm

clean:
	rm -rf *.dump *.p?m
