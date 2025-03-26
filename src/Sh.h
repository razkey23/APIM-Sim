#ifndef SH_H
#define SH_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class Sh : public sc_module {
    private:
        int sh_data[NCOLS];
    
    public:
        sc_port<interfaces::SH_out_if> p_SH_data_in;
        sc_in<bool>                    p_DoS;


        sc_port<interfaces::ADCV_in_if> p_SH_data_out;
        sc_out<bool>                   p_done_SH;

        Sh(sc_module_name nm);
        ~Sh();
        SC_HAS_PROCESS(Sh);
        
        void SH();
        void print_state();
        friend class Decoder;
};

#endif