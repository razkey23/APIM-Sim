#include "Rsr.h"

Rsr::Rsr(sc_module_name nm) : sc_module(nm) {
    
    for (int i = 0; i < NROW_GROUPS; i++) {
        for (int j = 0; j < ROWS_PER_GROUP; j++) {
            rsr_data[i][j] = false;
        }
    }

    SC_THREAD(clock_pos);
	sensitive << clock.pos();
	SC_THREAD(clock_neg);
	sensitive << clock.neg();



    SC_THREAD(update_state);
    dont_initialize();
}


Rsr::~Rsr(){
    
}



void Rsr::clock_pos()
{
	wait();
}

void Rsr::clock_neg()
{
	wait();
}

void Rsr::RSELRSR() 
{   
    clock_pos();
    for (int i=0; i < NROW_GROUPS; i++) {
        for (int j=0; j < ROWS_PER_GROUP; j++) p_RSELRSR_data_out->data[i][j] = rsr_data[i][j]; 
    }
}

void Rsr::update_state() 
{
    clock_pos();
    
    
    //std::cout << "\nRSR updating its state at row group " << p_RSR_data_in->row_group<<" "<< sc_time_stamp() << endl;
    //print_state();
    
    for (int j=0; j < ROWS_PER_GROUP; j++) {
        rsr_data[p_RSR_data_in->row_group][j] = p_RSR_data_in->data[p_RSR_data_in->row_group][j];
    }
    //print_state();
    std::cout << "RSR state updated." << std::endl;
    
}

void Rsr::print_state() 
{
    std::cout << "[RSR] State at " << sc_time_stamp() << std::endl;
    std::cout << "TOTAL ROWS: " << (NROW_GROUPS * ROWS_PER_GROUP) << std::endl;

    for (int i = 0; i < NROW_GROUPS; i++) {
        for (int j = 0; j < ROWS_PER_GROUP; j++) {
            std::cout << "Row " << (i * ROWS_PER_GROUP + j) << " : " << rsr_data[i][j] << std::endl;
        }
    }
}