#include "OReg.h"

OReg::OReg(sc_module_name nm) : sc_module(nm) {
    for (int i=0;i<NCOLS;i++) out_reg[i] = 0;
    SC_THREAD(clock_pos);
    sensitive << clock.pos();
    SC_THREAD(clock_neg);
    sensitive << clock.neg();
    SC_THREAD(ST);
    //dont_initialize();
}

OReg::~OReg() {
}

void OReg::clock_pos() {
    wait();
}

void OReg::clock_neg() {
    wait();
}

void OReg::ST() {
    clock_pos();
    //cout << "ST decoded at " << sc_time_stamp() << endl;
    int mode;
    mode = p_ST->mode;
    if (mode == 0) {
        for (int i=0;i<NCOLS;i++) {
            out_reg[i] = p_ADC_data_in->data[i];
        }
    } else {
        for (int i=0;i<NCOLS;i++) {
            out_reg[i] = p_Adder_data_in->data[i];
        }
    }
    cout << "OReg State Updated" << endl;
    for (int i=0;i<NCOLS;i++) {
        cout << out_reg[i] << " ";
    }
    cout << endl;
}