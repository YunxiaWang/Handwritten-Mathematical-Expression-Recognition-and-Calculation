/*
 * knnrecognition.h
 * Created on: 2015/12/7
 *      Author: Wang
 */

#ifndef KNNRECOGNITION_H_
#define KNNRECOGNITION_H_
#include "ap_int.h"
#include "training_data.h"

#define K_CONST 3
#define TRAINING_SIZE 2000
#define TRAINING_SIZE_O 105

ap_uint<4> digitrec( ap_uint<196> input );
void update_knn( ap_uint<196> test_data, ap_uint<196> train_set, ap_uint<8> min_distances[K_CONST] );
ap_uint<4> knn_vote( ap_uint<8> min_distances[13][K_CONST] );

#endif /* KNNRECOGNITION_H_ */
