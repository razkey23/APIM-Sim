#include "Sh.h"

Sh::Sh(sc_module_name nm) : sc_module(nm) , p_done_SH(0){
    for (int i=0; i < NCOLS;i++) sh_data[i] = 0;
    
    
    SC_THREAD(SH);
    sensitive<<p_DoS.pos();
    dont_initialize();
}

Sh::~Sh() {}

void Sh::SH() {
    //wait();
    cout<<"Got into SH "<<sc_time_stamp()<<endl;
    while(true) {
        //wait();
        p_done_SH.write(0);
        wait(20,SC_NS);
        //p_done_SH.write(0);

        //wait(10,SC_NS);
        //p_done_SH.write(0);
        for (int i=0;i<NCOLS;i++){
            sh_data[i] = p_SH_data_in->data[i];
            p_SH_data_out->data[i] = sh_data[i];
        }
        cout<<"Current State at: "<<sc_time_stamp()<<endl;
        print_state();
        
        p_done_SH.write(1);
        //wait(20,SC_NS);
        wait();
        //wait(10, SC_NS);
        //p_done_SH.write(false);
        
    }
}

void Sh::print_state(){
    for (int i=0;i<NCOLS;i++) {
        cout<<sh_data[i]<<" ";
    }
    cout<<endl;
}