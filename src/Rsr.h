#ifndef RSH_H
#define RSH_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class Rsr : public sc_module {
    private:
        bool rsr_data[NROW_GROUPS][ROWS_PER_GROUP];
    public:
        sc_port<interfaces::LDRSR_out_if>		p_RSR_data_in; // data to RSR
        sc_port<interfaces::RSELRSR_out_if>      p_RSELRSR_data_out; // from RSR to Crossbar


        sc_in_clk clock;

        Rsr(sc_module_name nm);
        ~Rsr();
        SC_HAS_PROCESS(Rsr);
        void clock_pos();
        void clock_neg();
        void update_state();
        void print_state();
        void RSELRSR();
        friend class Decoder;
        
};  

#endif