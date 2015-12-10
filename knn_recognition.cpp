// k-nearest-neighbors implementation for recognition

#include "knnrecognition.h"
#include "training_data.h"
#include <iostream>
using namespace std;

#define K 3
#define TRAINING_SIZE 2000

// top function for recognition
// @param[in] : input - the training data
// @return : the recognized digit (0~9) and operator (10~12)
ap_uint<4> digitrec( ap_uint<196> input )
		{
	// This array stores K minimum distances per training set
	ap_uint<8> knn_set[10][K];
	// Initialize the knn set
	for ( int i = 0; i < 10; ++i )
		for ( int k = 0; k < K; ++k )
			// The max distance is 196
			knn_set[i][k] = 197;
	for ( int i = 0; i < TRAINING_SIZE; ++i ) {
		for ( int j = 0; j < 10; j++ ) {
			// Read a new instance from the training set
			ap_uint<196> training_set = training_data[j * TRAINING_SIZE + i];
			// Update the KNN set
			update_knn( input, training_set, knn_set[j] );
		}
	}
	// Compute the final output
	return knn_vote( knn_set );
		}

// maintains/updates an array of K minimum distances per training set.
// @param[in] : test_inst - the testing data
// @param[in] : train_inst - the training data
// @param[in/out] : min_distances[K_CONST] - the array that stores the current K minimum distance values per training set
void update_knn( ap_uint<196> test_data, ap_uint<196> training_set, ap_uint<8> min_distances[K] ) {
	int bitdif=0; // the number of different bits
	for (int i=0; i<196; i++){ // 196 bits per instance to caculate distance
		if (test_data[i]!=training_set[i]) // XOR
			bitdif++;
	}
	int maxdis=0;
	int maxindex=0;
	for (int k=0; k<K; k++){ //find the biggest distance
		if (min_distances[k]>maxdis){
			maxdis=min_distances[k];
			maxindex=k;
		}
	}
	if (bitdif<min_distances[maxindex]) // sustitute the biggest with small distance
		min_distances[maxindex]=bitdif;
}

// finds the actual K nearest neighbors within 10*K minimum distance values and determines the
// final output based on the most common digit and operator represented by these nearest neighbors
// @param[in] : knn_set - 10xK min distance values
// @return : the recognized digit or operator
ap_uint<4> knn_vote( ap_uint<8> knn_set[10][K] ) {
	struct distance{ // build struct for distance and its ap_uint<196>
		ap_uint<8> dist;
		ap_uint<4> index;
	};
	struct distance dis[10*K];
	ap_uint<8> allsort[10*K];
	for (int i=0; i<10; i++){ // input arrays
		for (int j=0; j<K; j++){
			allsort[i*K+j]=knn_set[i][j];
			dis[i*K+j].dist=knn_set[i][j];
			dis[i*K+j].index=i;
		}
	}
	for (int m=0; m<10*K; m++){
		for (int n = 10*K-2; n >= 0; --n) {
			if (n >= m) {
				if (allsort[n] > allsort[n+1]){
					ap_uint<8> t=allsort[n]; // sorting the 10*K distances
					allsort[n]=allsort[n+1];
					allsort[n+1]=t;
					distance tt = dis[n]; // sorting the ap_uint<196> with distances
					dis[n] = dis[n+1];
					dis[n+1]= tt;
				}
			}
		}
	}
	struct comdigit{ // build struct for ap_uint<196> and its occuring times
		ap_uint<4> digits;
		int times;
	};
	struct comdigit mostcommon[K]; //k digits and their occuring times
	for (int k1=0; k1<K; k1++){ // get the most common K digits
		mostcommon[k1].digits=dis[k1].index;
		mostcommon[k1].times=0;
		for (int k2=0; k2<K; k2++){ // count the number of common digits
			if (mostcommon[k1].digits==mostcommon[k2].digits)
				mostcommon[k1].times++;
		}
	}
	for (int k3=0; k3<K; k3++){  // find the most common ap_uint<196>
		if (mostcommon[0].times<mostcommon[k3].times){
			comdigit t=mostcommon[0];
			mostcommon[0]=mostcommon[k3];
			mostcommon[k3]=t;
		}
	}
	return mostcommon[0].digits;
}



