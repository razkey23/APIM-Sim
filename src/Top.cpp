#include <systemc.h>

#include "Decoder.h"
#include "Rsr.h"
#include "IBuffer.h"
#include "Crossbar.h"
#include "Adc.h"
#include "interfaces.h"
#include "Csr.h"
#include "Adder.h"
#include "OReg.h"

int sc_main(int argc, char* argv[])  {
    sc_clock clk("clk",10,SC_NS);

    // 2) Create signals
    sc_signal<bool> p_done_crossbar("p_done_crossbar");
    //sc_signal<bool> p_DoA("p_DoA");
    sc_signal<bool> p_done_SH("p_done_SH");
    sc_signal<bool> p_done_ADC("p_done_ADC");

    sc_signal<bool> doA_signal("doA_signal");
    sc_signal<bool> doS_signal("doS_signal");
    sc_signal<bool> doR_signal("doR_signal");

    //sc_signal<bool> p_done_crossbar("p_done_crossbar");

    

    interfaces::LDRSR_in_if ldrsr_data_in;
    interfaces::LDRSR_out_if ldrsr_data_out;
    interfaces::LDCSRIMM_in_if ldcsr_data_in;
    interfaces::RSELRSR_out_if rselrsr_data_out;
    interfaces::CSELCSR_out_if cselsr_data_out;
    interfaces::SH_out_if sh_data_out;
    interfaces::ADCV_in_if adc_data_out;
    interfaces::ADCV_CS_in_if adcv_cs_data;
    interfaces::ADCV_out_if adcv_out_data;
    //interfaces::ADCV_out_if oreg_in_adc_data;
    interfaces::ST_in_if    st_data_in;
    interfaces::IADD_out_if iadd_data_out;


    // 3) Instantiate the “other” modules
    Crossbar crossbar_obj("crossbar_obj");
    Rsr      rsr_obj("rsr_obj");
    Csr      csr_obj("csr_obj");
    Adc      adc_obj("adc_obj");
    IBuffer  ibuffer_obj("ibuffer_obj");
    Sh       sh_obj("sh_obj");
    Adder    adder_obj("adder_obj");
    OReg     oreg_obj("oreg_obj");

    Decoder decoder("decoder", &oreg_obj, &adder_obj,&sh_obj, &csr_obj,&crossbar_obj, &rsr_obj, &adc_obj, &ibuffer_obj);
    
    // Input Buffer Ports
    ibuffer_obj.p_LDRSR_data_in(ldrsr_data_in);
    ibuffer_obj.p_RSR_data_out(ldrsr_data_out);
   
    // Row Select Register Ports
    rsr_obj.p_RSR_data_in(ldrsr_data_out);
    rsr_obj.p_RSELRSR_data_out(rselrsr_data_out);
    
    // Column Select Register Ports
    csr_obj.p_CSELCSR_data_out(cselsr_data_out);
    csr_obj.p_LDCSRIMM_data_in(ldcsr_data_in);

    
    // Crossbar Ports
    crossbar_obj.p_RSELRSR_data_in(rselrsr_data_out);
    crossbar_obj.p_CSELCSR_data_in(cselsr_data_out);
    crossbar_obj.p_SH_data_out(sh_data_out);
    crossbar_obj.p_done_crossbar(p_done_crossbar);
    crossbar_obj.p_DoA(doA_signal);
    

    // Sample and Hold Ports
    sh_obj.p_SH_data_in(sh_data_out);
    sh_obj.p_SH_data_out(adc_data_out);
    sh_obj.p_DoS(doS_signal);
    sh_obj.p_done_SH(p_done_SH);
    
    // ADC ports
    adc_obj.p_ADCVCS_in_data(adcv_cs_data);
    adc_obj.p_ADCVSH_in_data(adc_data_out);
    adc_obj.p_ADC_data_out(adcv_out_data);
    adc_obj.p_done_ADC(p_done_ADC);

    // Adder
    adder_obj.p_ADC_data_in(adcv_out_data);
    adder_obj.p_IADD_data_out(iadd_data_out);
    
    // Output Register Ports
    oreg_obj.p_ST(st_data_in);
    oreg_obj.p_ADC_data_in(adcv_out_data);
    oreg_obj.p_Adder_data_in(iadd_data_out);
    


    // Bind the clock signal
    oreg_obj.clock(clk);
    adc_obj.clock(clk);
    adder_obj.clock(clk);
    ibuffer_obj.clock(clk);
    rsr_obj.clock(clk);
    csr_obj.clock(clk);


    // 5) Bind ports and signals
    decoder.clock(clk);

    decoder.p_done_crossbar(p_done_crossbar);
    decoder.p_done_SH(p_done_SH);
    decoder.p_done_ADC(p_done_ADC);
    decoder.p_DoA(doA_signal);
    decoder.p_DoS(doS_signal);
    decoder.p_DoR(doR_signal);
    decoder.p_LDCSRIMM(ldcsr_data_in);
    decoder.p_LDRSR(ldrsr_data_in);
    decoder.p_ADCVCS(adcv_cs_data);
    decoder.p_ST(st_data_in);
    
    // ✅ Create a VCD trace file
    sc_trace_file *wf = sc_create_vcd_trace_file("trace");
    wf->set_time_unit(1, SC_NS);

    // ✅ Trace the signals of interest
    sc_trace(wf, clk, "clk");
    sc_trace(wf, doA_signal, "doA_signal");
    sc_trace(wf, doS_signal, "doS_signal");
    sc_trace(wf, p_done_crossbar, "p_done_crossbar");
    sc_trace(wf, p_done_SH, "p_done_SH");
    // Add others if needed, e.g. doS_signal, doR_signal, done_SH_signal, etc.

    sc_start(10, SC_NS);  // Let the simulation start
    
    std::cout << "Triggering decode() event at " << sc_time_stamp() << std::endl;
    //decoder.event_done_crossbar.notify();  // ✅ This will wake up `decode()`
    //decoder.event_done_crossbar.write(0);
    sc_start(1500, SC_NS);  // Continue simulation
    //decoder.event_done_crossbar.notify();
    //sc_start(50, SC_NS);  // Continue simulation
    

    std::cout << "Simulation finished at: " << sc_time_stamp() << std::endl;
    sc_close_vcd_trace_file(wf);
    return 0;
}   