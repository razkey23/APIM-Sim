#ifndef OReg_H
#define OReg_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class OReg : public sc_module {
    private:
        int out_reg[NCOLS];

    public:
        // Input Ports
        sc_in_clk clock;
        sc_port<interfaces::ST_in_if> p_ST;
        sc_port<interfaces::ADCV_out_if> p_ADC_data_in;
        sc_port<interfaces::IADD_out_if> p_Adder_data_in;

        
        SC_HAS_PROCESS(OReg);

        OReg(sc_module_name nm);
        ~OReg();
        void clock_pos();
        void clock_neg();
        void ST();

};

#endif