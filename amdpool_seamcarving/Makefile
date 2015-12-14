# Extract Vivado HLS include path
VHLS_PATH := $(dir $(shell which vivado_hls))/..
VHLS_INC ?= ${VHLS_PATH}/include

ALL_FILES := calculation.h knn_recognition.h img_processing.h calculation.cpp knn_recognition.cpp img_processing.cpp SeamCarving.h SeamCarving.cpp

all: ${ALL_FILES} recognition_test.cpp
	g++ -I${VHLS_INC} ${ALL_FILES} recognition_test.cpp -o recognition_test

accuracy: ${ALL_FILES} accuracy.cpp
	g++ -I${VHLS_INC} ${ALL_FILES} accuracy.cpp -o accuracy


clean:
	rm -f recognition_test vivado_hls.log
