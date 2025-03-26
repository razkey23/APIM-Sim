#ifndef CSR_H
#define CSR_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class Csr : public sc_module {
    private:
        bool csr_data[NCOL_GROUPS][COLUMNS_PER_GROUP];
    public:
        sc_port< interfaces::LDCSRIMM_in_if>		p_LDCSRIMM_data_in; 
        sc_port<interfaces::CSELCSR_out_if>         p_CSELCSR_data_out;
        
        sc_in_clk clock;

        Csr(sc_module_name nm);
        ~Csr();
        SC_HAS_PROCESS(Csr);
        void clock_pos();
        void clock_neg();
        void LDCSRIMM();
        void CSELCSR();
        void print_state();
        friend class Decoder;
        
};  

#endif