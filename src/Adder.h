#ifndef ADDER_H
#define ADDER_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class Adder : public sc_module {
    private:
        int out[NCOLS/WEIGHT_PRECISION];
        int psum[NCOLS/WEIGHT_PRECISION]; // Stores first Stage Addition
        int counter;  // Will be used as a HW loop to shift of psum
    public:
        sc_in_clk clock;
        sc_port<interfaces::ADCV_out_if> p_ADC_data_in;
        sc_port<interfaces::IADD_out_if> p_IADD_data_out;
        
        SC_HAS_PROCESS(Adder);
        Adder(sc_module_name nm);
        ~Adder();
        void clock_pos();
        void clock_neg();
        void execute();

};

#endif