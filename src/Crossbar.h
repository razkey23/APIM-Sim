#ifndef CROSSBAR_H
#define CROSSBAR_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class Crossbar : public sc_module {
    private:
        int memory[NROW_GROUPS][ROWS_PER_GROUP][NCOL_GROUPS][COLUMNS_PER_GROUP]; //
    public:


        // INPUTS
        sc_in<bool> p_DoA;
        sc_port<interfaces::RSELRSR_out_if> p_RSELRSR_data_in;
        sc_port<interfaces::CSELCSR_out_if> p_CSELCSR_data_in;
        

        //long double energy_consumption;

        // OUTPUTS
        sc_port<interfaces::SH_out_if> p_SH_data_out;
        sc_out<bool>                   p_done_crossbar;

        Crossbar(sc_module_name nm);
        ~Crossbar();
        void initArray();
        SC_HAS_PROCESS(Crossbar);
        friend class Decoder;

        void execute();
        void print_state();
        
};  

#endif