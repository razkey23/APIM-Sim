#ifndef ADC_H
#define ADC_H
#include "config.h"
#include "interfaces.h"
#include <systemc.h>

class Adc : public sc_module {
    private:     
        sc_event e_activation;
    public:
        // Input
        sc_in_clk   clock;
        sc_port<interfaces::ADCV_in_if> p_ADCVSH_in_data;
        sc_port<interfaces::ADCV_CS_in_if>  p_ADCVCS_in_data;
        
        //sc_in<bool>                    p_ADCV;
        
        //output
        sc_export<interfaces::ADCV_out_if> p_ADC_data_out;
        sc_out<bool>                   p_done_ADC;
        

        SC_HAS_PROCESS(Adc);
        //void execution();
        void activation();
        void execute();
        Adc(sc_module_name nm);
        ~Adc();

        void clock_pos();
        void clock_neg();

        
};  

#endif