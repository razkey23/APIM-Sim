#include "Adc.h"
//#include "config.h"

Adc::Adc(sc_module_name nm) : sc_module(nm) {

    
    SC_THREAD(clock_pos);
	sensitive << clock.pos();
	SC_THREAD(clock_neg);
	sensitive << clock.neg();

    SC_THREAD(execute);
	sensitive << e_activation << clock.pos();
	dont_initialize();
}


Adc::~Adc(){ 
}

void Adc::execute(){
    while (true)
	{
		wait(e_activation);
        p_done_ADC.write(0);
        cout<<"Activated at timestamp "<<sc_time_stamp()<<" ";
        cout << p_ADCVCS_in_data->rangeStart << " " <<p_ADCVCS_in_data->rangeEnd << " "<< p_ADCVCS_in_data->columnId<<endl;
        for (int i=p_ADCVCS_in_data->rangeStart;i<p_ADCVCS_in_data->rangeEnd;i++){
            int index = i*ADC_SHARING_FACTOR+p_ADCVCS_in_data->columnId;
            p_ADC_data_out->data[index] = p_ADCVSH_in_data->data[index];
        }
        wait(20, SC_NS);
        p_done_ADC.write(1);
    }
}


void Adc::clock_pos()
{
	wait();
}

void Adc::clock_neg()
{
	wait();
}

void Adc::activation(){
    e_activation.notify();
}

