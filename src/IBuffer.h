#ifndef IBuffer_H
#define IBuffer_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class IBuffer : public sc_module {
    private:
        bool ibuffer[NROW_GROUPS][ROWS_PER_GROUP][INPUT_PRECISION];
    public:
        sc_in_clk clock;
        
        

        IBuffer(sc_module_name nm);
        ~IBuffer();

        // Event notifications
        sc_event event_LDRSR_done; // Event to notify RSR module
    
        // Input
        sc_port<interfaces::LDRSR_in_if> p_LDRSR_data_in; 
        // Output
        sc_port<interfaces::LDRSR_out_if> p_RSR_data_out; 


        SC_HAS_PROCESS(IBuffer);
        void LDRSR();
        void clock_pos();

        void print_state();

        friend class Decoder;
};  

#endif