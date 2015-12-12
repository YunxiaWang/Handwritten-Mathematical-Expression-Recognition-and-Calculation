# Extract Vivado HLS include path
VHLS_PATH := $(dir $(shell which vivado_hls))/..
VHLS_INC ?= ${VHLS_PATH}/include

ALL_FILES := training_data.h knn_recognition.h knn_recognition.cpp calculation.h calculation.cpp img_processing.h img_processing.cpp

all: ${ALL_FILES} recognition_test.cpp
	g++ -I${VHLS_INC} ${ALL_FILES} recognition_test.cpp -o recognition_test

accuracy: ${ALL_FILES} accuracy.cpp
	g++ -I${VHLS_INC} ${ALL_FILES} accuracy.cpp -o accuracy


clean:
	rm -f recognition_test vivado_hls.log
