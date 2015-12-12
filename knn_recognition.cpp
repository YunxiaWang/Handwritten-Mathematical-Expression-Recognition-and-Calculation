//==========================================================================
//knn_recognition.cpp
//==========================================================================
// @brief: A k-nearest-neighbors implementation for digit recognition

#include "knn_recognition.h"

//----------------------------------------------------------
// Top function
//----------------------------------------------------------
// @param[in] : input - the testing instance
// @return : the recognized digit (0~9)

ap_uint<4> digitrec( ap_uint<196> input )
{
    // This array stores K minimum distances per training set
    ap_uint<8> knn_set[13][K_CONST];
    //#pragma HLS array_partition variable=knn_set block factor=13 dim=1
    //#pragma HLS array_partition variable=training_data block factor=13
    // Initialize the knn set
MY_LP1:
    for ( int i = 0; i < 13; ++i ) {
        //#pragma HLS unroll
        for ( int k = 0; k < K_CONST; ++k ) {
            // Note that the max distance is 196
            knn_set[i][k] = 197;
        }
    }
    
    
MY_LP2:
    for ( int i = 0; i < TRAINING_SIZE; ++i ) {
        //#pragma HLS unroll region
        for ( int j = 0; j < 10; j++ ) {
            // Read a new instance from the training set
            ap_uint<196> training_instance = training_data[j * TRAINING_SIZE + i];
            // Update the KNN set
            update_knn( input, training_instance, knn_set[j] );
        }
    }
    for ( int i = 0; i < TRAINING_SIZE_O; ++i ) {
        for ( int j = 10; j < 13; j++ ) {
            // Read a new instance from the training set
            ap_uint<196> training_instance = training_data_O[(j - 10) * TRAINING_SIZE_O + i];
            // Update the KNN set
            update_knn( input, training_instance, knn_set[j] );
        }
    }
    
    // Compute the final output
    return knn_vote( knn_set );
}


//-----------------------------------------------------------------------
// update_knn function
//-----------------------------------------------------------------------
// Given the test instance and a (new) training instance, this
// function maintains/updates an array of K minimum
// distances per training set.
// @param[in] : test_inst - the testing instance
// @param[in] : train_inst - the training instance
// @param[in/out] : min_distances[K_CONST] - the array that stores the current
//                  K_CONST minimum distance values per training set

void update_knn( ap_uint<196> test_inst, ap_uint<196> train_inst, ap_uint<8> min_distances[K_CONST] )
{
    ap_uint<8> distance = 0;
LOOP1:
    for (ap_uint<8> i = 0; i < 196; i++) {
        if (test_inst[i] != train_inst[i]) {
            distance++;
        }
    }
    ap_uint<4> k;
    ap_uint<8> temp;
LOOP2:
    for (ap_uint<4> i = 0; i < K_CONST-1; i++) {
        k = i;
        for (ap_uint<4> j = i+1; j < K_CONST ; j++) {
            if (min_distances[k] > min_distances[j]) {
                k = j;
            }
        }
        if (i != k) {
            temp = min_distances[i];
            min_distances[i] = min_distances[k];
            min_distances[k] = temp;
        }
    }
    if (distance < min_distances[K_CONST-1]) {
        min_distances[K_CONST-1] = distance;
    }
}


//-----------------------------------------------------------------------
// knn_vote function
//-----------------------------------------------------------------------
// Given 13xK minimum distance values, this function
// finds the actual K nearest neighbors and determines the
// final output based on the most common digit represented by
// these nearest neighbors (i.e., a vote among KNNs).
// @param[in] : knn_set - 13xK_CONST min distance values
// @return : the recognized digit
//

ap_uint<4> knn_vote( ap_uint<8> knn_set[13][K_CONST] )
{
    // put 2-d knn_set[][] array into 1-d knn[] array
    ap_uint<8> knn[13 * K_CONST];
    ap_uint<4> index[13 * K_CONST];
    
LOOP3:
    for (ap_uint<4> i = 0; i < 13; i++) {
        for (ap_uint<4> j = 0; j < K_CONST; j++) {
            knn[i * K_CONST + j] = knn_set[i][j];
            index[i * K_CONST + j] = i;
        }
    }
    // sort 1-d knn[] array with their index
    ap_uint<8> k;
    ap_uint<8> temp1, temp2;
LOOP4:
    for (ap_uint<8> i = 0; i < 13*K_CONST-1; i++) {
        k = i;
        for (ap_uint<8> j = i+1; j < 13*K_CONST ; j++) {
            if (knn[k] > knn[j]) {
                k = j;
            }
        }
        if (i != k) {
            temp1 = knn[i];
            knn[i] = knn[k];
            knn[k] = temp1;
            temp2 = index[i];
            index[i] = index[k];
            index[k] = temp2;
        }
    }
    // take out the first K_CONST minimum digit's index then put them into a 1-d index_actual[] array with
    
    ap_uint<4> index_actual[K_CONST];
    int index_count[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
    
LOOP5:
    for (ap_uint<4> i = 0; i < K_CONST; i++) {
        index_actual[i] = index[i];
    }
    
    // find out the number of each digit 0 ~ 9 from index_actual
LOOP6:
    for (ap_uint<4> i = 0; i < K_CONST; i++) {
        index_count[index_actual[i]]++;
    }
    // sort the index_count with their indices
    
    ap_uint<4> indices[13] = {0,1,2,3,4,5,6,7,8,9,10,11,12};
LOOP7:
    for (ap_uint<4> i = 0; i < 13 - 1; i++) {
        k = i;
        for (ap_uint<4> j = i+1; j < 13 ; j++) {
            if (index_count[k] > index_count[j]) {
                k = j;
            }
        }
        if (i != k) {
            temp1 = index_count[i];
            index_count[i] = index_count[k];
            index_count[k] = temp1;
            temp2 = indices[i];
            indices[i] = indices[k];
            indices[k] = temp2;
        }
    }
    return indices[12];
}
