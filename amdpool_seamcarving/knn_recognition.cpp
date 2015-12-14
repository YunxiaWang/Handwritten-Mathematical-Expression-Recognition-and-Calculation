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
ML1:
    for ( int i = 0; i < 13; ++i ) {
        //#pragma HLS unroll
        for ( int k = 0; k < K_CONST; ++k ) {
            // Note that the max distance is 196
            knn_set[i][k] = 197;
        }
    }
    
    
L2000:
    for ( int i = 0; i < TRAINING_SIZE; ++i ) {
    L13:        //#pragma HLS unroll region
        for ( int j = 0; j < 13; j++ ) {
            // Read a new instance from the training set
            ap_uint<196> training_instance = training_data[j * TRAINING_SIZE + i];
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
    
    ap_uint<196> diff = test_inst ^ train_inst;
    
    ap_uint<8> distance = 0;
    for (int i = 0; i < 196; i++) {
        distance += diff[i];
    }
    
    ap_uint<8> maxdis=0;
    ap_uint<4> maxindex=0;
    for (int k=0; k<K_CONST; k++){ //find the biggest distance
        if (min_distances[k]>maxdis){
            maxdis=min_distances[k];
            maxindex=k;
        }
    }
    if (distance<min_distances[maxindex]) // sustitute the biggest with small distance
        min_distances[maxindex]=distance;
    
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
    struct distance{ // build struct for distance and its digit
        ap_uint<8> dist;
        ap_uint<4> index;
    };
    struct distance dis[13*K_CONST];
    ap_uint<8> allsort[13*K_CONST];
    for (int i=0; i<13; i++){ // input arrays
        for (int j=0; j<K_CONST; j++){
            allsort[i*K_CONST+j]=knn_set[i][j];
            dis[i*K_CONST+j].dist=knn_set[i][j];
            dis[i*K_CONST+j].index=i;
        }
    }
    for (int m=0; m<13*K_CONST; m++){
        for (int n = 13*K_CONST-2; n >= 0; --n) {
            if (n >= m) {
                if (allsort[n] > allsort[n+1]){
                    ap_uint<8> t=allsort[n]; // sorting the 13*K distances
                    allsort[n]=allsort[n+1];
                    allsort[n+1]=t;
                    distance tt = dis[n]; // sorting the digit with distances
                    dis[n] = dis[n+1];
                    dis[n+1]= tt;
                }
            }
        }
    }
    struct comdigit{ // build struct for digit and its occuring times
        ap_uint<4> digits;
        ap_uint<4> times;
    };
    struct comdigit mostcommon[K_CONST]; //k digits and their occuring times
    for (int k1=0; k1<K_CONST; k1++){ // get the most common K digits
        mostcommon[k1].digits=dis[k1].index;
        mostcommon[k1].times=0;
        for (int k2=0; k2<K_CONST; k2++){ // count the number of common digits
            if (mostcommon[k1].digits==mostcommon[k2].digits)
                mostcommon[k1].times++;
        }
    }
    for (int k3=0; k3<K_CONST; k3++){  // find the most common digit
        if (mostcommon[0].times<mostcommon[k3].times){
            comdigit t=mostcommon[0];
            mostcommon[0]=mostcommon[k3];
            mostcommon[k3]=t;
        }
    }
    return mostcommon[0].digits;
}
