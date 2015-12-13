#==========================================================================
# Makefile
#==========================================================================
# @brief: A makefile the compiles and synthesizes the digitrec program
#
# @desc: 1. "make" runs csim by default
#        2. "make csim" compiles & executes the fixed-point implementation
#        3. "make clean" cleans up the directory


# Extract Vivado HLS include path
VHLS_PATH := $(dir $(shell which vivado_hls))/..
VHLS_INC ?= ${VHLS_PATH}/include

CFLAGS = -g -I${VHLS_INC} 

all: csim

csim: knn_recognition.cpp recognition_test.cpp calculation.cpp img_processing.cpp 
	@echo "Compiling & simulating digitrec on amdpool ..."
	g++ ${CFLAGS} $^ -o recognition_test
	

bitstream: 
	@echo "Synthesizing digitrec and creating bitstream on amdpool ..."
	vivado_hls -f run_pipeline.tcl
	./run_bitstream.sh

clean:
	rm -rf digitrec *.dat *.prj *.log
	rm -rf zedboard_project* xillydemo.bit
