#include "IBuffer.h"
#include "config.h"
#include <sstream>    // for std::stringstream
#include <fstream>    // for std::ifstream

IBuffer::IBuffer(sc_module_name nm) : sc_module(nm) {
    /*for (int i = 0; i < NROW_GROUPS; i++) {
        for (int j = 0; j < ROWS_PER_GROUP; j++) {
            for (int k = 0; k < INPUT_PRECISION; k++) {
                if ((i+j+k)%2==0) {
                    ibuffer[i][j][k] = true;
                }
                else {
                    ibuffer[i][j][k] = true;
                }
            }
        }
    } */
    
    // Init
    
    std::ifstream infile("/home/razkey23/playground/acimsim/tests/functional_sim/out_vector_bits.txt");
    if (!infile.is_open()){
        SC_REPORT_FATAL("IBuffer", "Unable to open vector bits file");
    }
    for (int row=0; row <NROWS;row++) {
        int i = row/ROWS_PER_GROUP;
        int j = row % ROWS_PER_GROUP;
        std::string line;
        if (!std::getline(infile,line)) {
            SC_REPORT_FATAL("IBuffer", "Vector file mismatch");
        }
        std::stringstream ss(line);
        for (int k=0;k<INPUT_PRECISION;k++) {
            int bit_val;
            if(!(ss >> bit_val)) { 
                SC_REPORT_FATAL("IBuffer", "Vector file mismatch");
            }
            if (bit_val==1) { 
                ibuffer[i][j][k]=true; 
            }
            else if (bit_val == 0) { 
                
                ibuffer[i][j][k]=false; 
            }
           
        }
        cout<<endl;
    }
    infile.close(); 

    

    
    SC_THREAD(clock_pos);
	sensitive << clock.pos();


    SC_THREAD(LDRSR);
    dont_initialize();
}

void IBuffer::clock_pos()
{
	wait();
}



void IBuffer::LDRSR() {
    
    
    //clock_pos();
    wait(SC_ZERO_TIME);
    //print_state();
    p_RSR_data_out->row_group = p_LDRSR_data_in->row_group;
    for (int i =0 ;i<NROW_GROUPS; i++) {
        for (int j=0 ; j<ROWS_PER_GROUP; j++) p_RSR_data_out->data[i][j] = ibuffer[i][j][INPUT_PRECISION-1];
    }
    
    bool new_bit = false; // we only need to shift the bits of the chosen row_group

    
    for (int j=0; j < ROWS_PER_GROUP; j++) {
            for (int k= INPUT_PRECISION-1; k>0;k--) {
                ibuffer[p_LDRSR_data_in->row_group][j][k] = ibuffer[p_LDRSR_data_in->row_group][j][k-1];
            }
            ibuffer[p_LDRSR_data_in->row_group][j][0] = new_bit; 
    }
    //print_state();
    
}



void IBuffer::print_state() {
    std::cout << "\n[IBuffer] State at " << sc_time_stamp() << std::endl;
    std::cout << "TOTAL ROWS: " << (NROW_GROUPS * ROWS_PER_GROUP) << std::endl;

    for (int i = 0; i < NROW_GROUPS; i++) {
        for (int j = 0; j < ROWS_PER_GROUP; j++) {
            std::cout << "Row " << (i * ROWS_PER_GROUP + j) << " : ";
            for (int k = 0; k < INPUT_PRECISION; k++) {
                std::cout << ibuffer[i][j][k];
            }
            std::cout << std::endl;
        }
    }
}

IBuffer::~IBuffer(){
    
}