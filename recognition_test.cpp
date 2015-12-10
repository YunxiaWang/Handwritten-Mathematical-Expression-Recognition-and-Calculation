/*
 * recognition_test.cpp
 *
 *  Created on: 2015/12/7
 *      Author: Wang
 */

#include <stdio.h>
#include <iostream>

#include "training_data.h"
#include "knn_recognition.h"
#include "ap_int.h"

int main() {
 //read the input as ap_uint<196>
  // from the 0 training set
  ap_uint<196> test0 = ap_uint<196>("0000000000000000000000000000000001111000000000111111000000011110111000000110000110000001100001110000111000001100000110000011000001110001100000001111110000000001111000000000000000000000000000000000", 2);
  // from the 5 training set
  ap_uint<196> test1 = ap_uint<196>("0000000000000000000000000000000000011100000000001111110000000110000000000011000000000000111111000000001111110000000000001100000000000011000000000001100000000001110000000000010000000000000000000000", 2);

  ap_uint<8> result0 = digitrec(test0);
  ap_uint<8> result1 = digitrec(test1);

  std::cout << "interpreted_digit from test0: " << result0 << std::endl;
  std::cout << "interpreted_digit from test1: " << result1 << std::endl;

  return 0;
}



