/*
 * knnrecognition.h
 * Created on: 2015/12/7
 *      Author: Wang
 */

#ifndef KNNRECOGNITION_H_
#define KNNRECOGNITION_H_
#include "ap_int.h"
#include "training_data.h"
#include <hls_stream.h>

#define K_CONST 3
#define TRAINING_SIZE 1600
#define TRAINING_SIZE_O 105

void dut( hls::stream< ap_uint<32> > &strm_in, hls::stream< ap_uint<32> > &strm_out );

ap_uint<4> digitrec( ap_uint<196> input );

void update_knn( ap_uint<196> test_data, ap_uint<196> train_set, ap_uint<8> min_distances[K_CONST] );

ap_uint<4> knn_vote( ap_uint<8> min_distances[13][K_CONST] );

#endif /* KNNRECOGNITION_H_ */
