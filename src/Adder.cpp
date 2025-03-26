#include "Adder.h"

Adder::Adder(sc_module_name nm) : sc_module(nm) {

    for (int i=0;i<NCOLS/WEIGHT_PRECISION;i++) {
        out[i]=0;
        psum[i]=0;
        
    }
    counter=0;
    SC_THREAD(clock_pos);
	sensitive << clock.pos();
	SC_THREAD(clock_neg);
	sensitive << clock.neg();

    SC_THREAD(execute);
    


}

Adder::~Adder(){

}

void Adder::clock_pos()
{
	wait();
}

void Adder::clock_neg()
{
	wait();
}

void Adder::execute(){
    clock_pos();
    //wait(SC_ZERO_TIME);
    cout<<"Execute is triggered at "<<sc_time_stamp()<<endl;
    cout <<" Counter is "<<counter<<endl;
    // Word Accumulator
    for (int col=0; col <NCOLS;col++){
        int index = (col/WEIGHT_PRECISION);
        int bit_shifts = WEIGHT_PRECISION-(col%WEIGHT_PRECISION)-1; 
        psum[index] += (p_ADC_data_in->data[col] << bit_shifts);
    }

    // Shift and Add 
    for (int i=0; i<NCOLS/WEIGHT_PRECISION;i++) {
        out[i] += psum[i] << counter;
        psum[i] = 0;
        p_IADD_data_out->data[i] = out[i];
    }

    for (int i=0;i<NCOLS/WEIGHT_PRECISION;i++) {
        cout<<"IADD Data Out is "<<p_IADD_data_out->data[i]<<endl;
    }
    counter++;
}