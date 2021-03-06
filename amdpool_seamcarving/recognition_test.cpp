/*
 * recognition_test.cpp
 *
 *  Created on: 2015/12/7
 *      Author: Wang
 */

#include <stdio.h>
#include <iostream>

#include "training_data.h"
#include "img_processing.h"
#include "knn_recognition.h"
#include "calculation.h"
#include "ap_int.h"

int main(int argc, char *argv[]) {
    
    std::cout << "------------------------------------------------------------------------" << std::endl;
    std::cout << "                          START IMG PROCESSING                          " << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    img_processing(argv[1]);
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "                          END IMG PROCESSING                          " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    
    // Read input file for the testing set
    std::string line;
    std::ifstream myfile ("testing_set.dat");
    int number[7] = {-1,-1,-1,-1,-1,-1,-1};
    
    if ( myfile.is_open() ) {
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        std::cout << "                          START OF KNN ALGORITHM                          " << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        int i = 0;
        while ( std::getline( myfile, line) ) {
            ap_uint<49>  input_digit3 = strtoul( line.substr(0,49).c_str(), NULL, 2);
            ap_uint<49>  input_digit2 = strtoul( line.substr(49,49).c_str(), NULL, 2);
            ap_uint<49>  input_digit1 = strtoul( line.substr(98,49).c_str(), NULL, 2);
            ap_uint<49>  input_digit0 = strtoul( line.substr(147,49).c_str(), NULL, 2);
            ap_uint<196> input_digit  = (input_digit3,input_digit2,input_digit1,input_digit0);
            
            ap_uint<4> interpreted_digit = digitrec(input_digit);
            std::cout << "interpreted_digit_or_operator "<< i << " : " << std::dec << interpreted_digit << std::endl;
            number[i] = interpreted_digit;
            i++;
        }
        // Close input file for the testing set
        myfile.close();
        std::cout << "------------------------------------------------------------------------" << std::endl;
        std::cout << "                          END OF KNN ALGORITHM                          " << std::endl;
        std::cout << "------------------------------------------------------------------------" << std::endl;
        std::cout << std::endl;
    }
    else {
        std::cout << "Unable to open file for the testing set!" << std::endl;
    }
    
    std::cout << "------------------------------------------------------------------------" << std::endl;
    std::cout << "                          START OF CALCULATION                          " << std::endl;
    std::cout << "------------------------------------------------------------------------" << std::endl;
    // calculate out the result of the whole expression from the input array
    calculation(number, 7);
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << "                          END OF CALCULATION                          " << std::endl;
    std::cout << "----------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
    
    return 0;
}



