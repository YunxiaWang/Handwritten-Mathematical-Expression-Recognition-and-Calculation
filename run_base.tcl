#=============================================================================
# run_base.tcl 
#=============================================================================
# @brief: A Tcl script for synthesizing the baseline recongnition design.

# Proje	```````````````````` ct name
set hls_prj recognition.prj

# Open/reset the project
open_project ${hls_prj} -reset

# Top function of the design is "dut"
set_top dut

# Add design and testbench files
add_files knn_recognition.cpp
add_files -tb calculation.cpp
add_files -tb img_processing.cpp
add_files -tb recognition_test.cpp
add_files -tb training_set_0.dat
add_files -tb training_set_1.dat
add_files -tb training_set_2.dat
add_files -tb training_set_3.dat
add_files -tb training_set_4.dat
add_files -tb training_set_5.dat
add_files -tb training_set_6.dat
add_files -tb training_set_7.dat
add_files -tb training_set_8.dat
add_files -tb training_set_9.dat
add_files -tb training_set_minus.dat
add_files -tb training_set_mul_dot.dat
add_files -tb training_set_plus.dat

open_solution "solution1"
# Use Zynq device
set_part {xc7z020clg484-1}

# Target clock period is 10ns
create_clock -period 10

### You can insert your own directives here ###

############################################

# Simulate the C++ design
#csim_design
# Synthesize the design
csynth_design
# Co-simulate the design
#cosim_design
exit
