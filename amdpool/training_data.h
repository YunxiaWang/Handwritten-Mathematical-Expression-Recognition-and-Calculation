// defines a 2D array that includes 10 training sets, where each set contains 2000 training data.


#ifndef TRAINING_DATA_H
#define TRAINING_DATA_H

#include "ap_int.h"

#define TRAINING_SIZE 2000
#define TRAINING_SIZE_O 105

const ap_uint<196> training_data[1 * TRAINING_SIZE] = {
#include "training_set_0.dat"
#include "training_set_1.dat"
#include "training_set_2.dat"
#include "training_set_3.dat"
#include "training_set_4.dat"
#include "training_set_5.dat"
#include "training_set_6.dat"
#include "training_set_7.dat"
#include "training_set_8.dat"
#include "training_set_9.dat"
};
const ap_uint<196> training_data_O[3 * TRAINING_SIZE_O] = {
  #include "training_set_minus.dat"
  #include "training_set_plus.dat"
  #include "training_set_mul.dat"
}
#endif
