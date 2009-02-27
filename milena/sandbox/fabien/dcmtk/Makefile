all: dicom.cc
	g++-4.2 -DHAVE_CONFIG_H -I../../../ -I/opt/local/include/ -L/opt/local/lib/ -ldcmdata -ldcmdsig -ldcmimage -ldcmimgle -ldcmjpeg -ldcmnet -ldcmpstat -ldcmqrdb -ldcmsr -ldcmtls -ldcmwlm dicom.cc -o dicom
