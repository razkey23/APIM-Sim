#include "Csr.h"

Csr::Csr(sc_module_name nm) : sc_module(nm) {
    
    for (int i = 0; i < NCOL_GROUPS; i++) {
        for (int j = 0; j < COLUMNS_PER_GROUP; j++) {
            csr_data[i][j] = false;
        }
    }

    SC_THREAD(clock_pos);
	sensitive << clock.pos();
	SC_THREAD(clock_neg);
	sensitive << clock.neg();



    SC_THREAD(LDCSRIMM);
}


Csr::~Csr(){
    
}



void Csr::clock_pos()
{
	wait();
}

void Csr::clock_neg()
{
	wait();
}


void Csr::LDCSRIMM() {
    clock_pos();
   
    //std::cout << "\nCSR updating its state at row group " << p_LDCSRIMM_data_in->column_group<<" "<< sc_time_stamp() << endl;

    for (int j=0; j < COLUMNS_PER_GROUP; j++) {
        cout<<p_LDCSRIMM_data_in->column_group<<" "<< j<< " "<<p_LDCSRIMM_data_in->data[j]<<endl;
        csr_data[p_LDCSRIMM_data_in->column_group][j] = (p_LDCSRIMM_data_in->data[j] == '1') ? 1 : 0;
    }
    //std::cout << "CSR state updated." << std::endl;
}

void Csr::CSELCSR() {
    clock_pos();
    for (int i=0; i < NCOL_GROUPS;i++) {
        for (int j=0 ; j < COLUMNS_PER_GROUP;j++) p_CSELCSR_data_out->data[i][j] = csr_data[i][j];
    }
}

void Csr::print_state() {
    std::cout << "[CSR] State at " << sc_time_stamp() << std::endl;
    std::cout << "TOTAL ROWS: " << (NCOL_GROUPS * COLUMNS_PER_GROUP) << std::endl;

    for (int i = 0; i < NCOL_GROUPS; i++) {
        for (int j = 0; j < COLUMNS_PER_GROUP; j++) {
            std::cout << "Row " << (i * ROWS_PER_GROUP + j) << " : " << csr_data[i][j] << std::endl;
        }
    }
}