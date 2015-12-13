/*
 * recognition_test.cpp
 *
 *  Created on: 2015/12/7
 *      Author: Wang
 */

#include <stdio.h>
#include <iostream>
#include <hls_stream.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>

#include "training_data.h"
#include "img_processing.h"
#include "knn_recognition.h"
#include "calculation.h"
#include "ap_int.h"
#include "timer.h"

int main(int argc, char *argv[]) {
    // get the input bmp file name from argv[1] and process the bmp file
    // segment and shrink the img to 14*14 bit vecter
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
    
    // HLS streams for communicating with the block
    hls::stream< ap_uint<32> > digitrec_in;
    hls::stream< ap_uint<32> > digitrec_out;
    
    // Arrays to store test data
    ap_uint<196> inputs[7];
    
    // Timer
    Timer timer("digitrec FPGA");
    
    // number of tests
    //const int N = 7;
    int nbytes;
    int count = 0;
    
    // array to store the output of the knn_alg
    int number[7] = {-1,-1,-1,-1,-1,-1,-1};
    
    if ( myfile.is_open() ) {
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        std::cout << "                          START OF KNN ALGORITHM                          " << std::endl;
        std::cout << "--------------------------------------------------------------------------" << std::endl;
        
        while ( std::getline( myfile, line) ) {
            ap_uint<49>  input_digit3 = strtoul( line.substr(0,49).c_str(), NULL, 2);
            ap_uint<49>  input_digit2 = strtoul( line.substr(49,49).c_str(), NULL, 2);
            ap_uint<49>  input_digit1 = strtoul( line.substr(98,49).c_str(), NULL, 2);
            ap_uint<49>  input_digit0 = strtoul( line.substr(147,49).c_str(), NULL, 2);
            ap_uint<196> input_digit  = (input_digit3,input_digit2,input_digit1,input_digit0);
            
            // std::cout << line << std::endl;
            // knn_alg for digit and operators recognition
            // ap_uint<4> interpreted_digit = digitrec(input_digit);
            
            
            inputs[count] = input_digit;
            
            count++;
        }
        
        timer.start();
        
        for (int i = 0; i < count; i++) {
            nbytes = write (fdw, (void*)&inputs[i], sizeof(inputs[i]));
            assert (nbytes == sizeof(inputs[i]));
        }
        
        for (int i = 0; i < count; i++) {
            nbytes = read (fdr, (void*)&interpreted_digit, sizeof(interpreted_digit));
            assert (nbytes == sizeof(interpreted_digit));
        }
        
        timer.stop();
        
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



