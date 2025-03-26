#ifndef DECODER_H
#define DECODER_H
#include "config.h"
#include <systemc.h>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#include <iostream>
#include "interfaces.h"
#include "Rsr.h"
#include "Crossbar.h"
#include "Adc.h"
#include "IBuffer.h"
#include "Csr.h"
#include "Sh.h"
#include "Adder.h"
#include "OReg.h"

/*
We have to include Write_Data, Row_Data, XBar, ADC -> that's the components we will send signals to
*/

using namespace std;

class Decoder : public sc_module {
    private:
        // Useful Internal Signals
        bool sc_done_crossbar, sc_done_SH, sc_done_ADC, sc_done_CSR;
        interfaces::CFG_if cfg_buffer;

        // Events
        //sc_event event_done_crossbar, event_done_SH, even_done_ADC;
        sc_event event_clock_pos, event_clock_neg;

        sc_event event_stage_exe;


        string opcode1,opcode2;
        
        // For branching
        map<string,streampos> labelMap;
        int loopreg; // Loop register

        fstream infile;

    public: 
        // DEBUG
        sc_event event_done_crossbar, event_done_SH, event_done_ADC;

        // Friend Classes , Here would be Crossbar, ADC, RSR, IBuffer
        Rsr* rsr; 
        Crossbar* crossbar;
        Adc* adc;
        IBuffer* ibuffer;
        Csr* csr;
        Sh* sh;
        Adder* adder;
        OReg* oreg;


        sc_in_clk clock;
        sc_in<bool> p_done_crossbar; // Comes from Crossbar
        sc_in<bool> p_done_SH; // Comes from Sample & Hold
        sc_in<bool> p_done_ADC; // Comes from ADC
        

        // Here we need some ports to send data to the components
        sc_port<interfaces::LDRSR_in_if> p_LDRSR;
        sc_port<interfaces::LDCSRIMM_in_if> p_LDCSRIMM;
        sc_port<interfaces::ADCV_CS_in_if> p_ADCVCS;
        sc_port<interfaces::ST_in_if> p_ST;
        //sc_port<interfaces::LDCSRIMM_in_if> p_LDCSRIMM;
        



        sc_out<bool> p_DoA;
        sc_out<bool> p_DoS;
        sc_out<bool> p_DoR; 


        Decoder(sc_module_name nm,OReg *oreg_obj,Adder *adder_obj,Sh *sh_obj,Csr *csr_obj, Crossbar *crossbar_obj, Rsr *rsr_obj, Adc *adc_obj, IBuffer *ibuffer_obj);
        ~Decoder();
        SC_HAS_PROCESS(Decoder);

        void clock_pos();
        void clock_neg();

        void fun_done_crossbar();
        void fun_done_SH();
        void fun_done_ADC();
        
        void decode();
        void exe();
};

#endif