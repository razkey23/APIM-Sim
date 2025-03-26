#ifndef INTERFACES_H
#define INTERFACES_H
#include <string>
#include <cstdlib>
#include "config.h"
#include <thread>
#include <systemc.h>

namespace interfaces 
{   

    /*
        Interface From Decoder to IBuffer for RSR
    */
    class LDRSR_in_if : public sc_interface{ 
        public:
            int row_group;
    };

    /*
        Interface From IBuffer to RSR
    */
    class LDRSR_out_if :public sc_interface {
        public:
            int row_group;
            char data[NROW_GROUPS][ROWS_PER_GROUP];
    };
    
    
    /*
        Interface from Decoder to CSR
        
    */
    class LDCSRIMM_in_if : public sc_interface {
        public:
            int column_group;
            char data[COLUMNS_PER_GROUP];
    };


    /*
        Interface from RSR to Xbar 
    */ 
    class RSELRSR_out_if : public sc_interface {
        public:
            char data[NROW_GROUPS][ROWS_PER_GROUP];
    };

    /*
        Interface from CSR to Xbar
    */
    class CSELCSR_out_if : public sc_interface {
        public:
            char data[NCOL_GROUPS][COLUMNS_PER_GROUP];
    };


    
    /*
        Interface from Xbar to SH
    */
    class SH_out_if : public sc_interface {
        public :
            int data[NCOLS];
            //int data[NCOLS+1];
    };

    /*
        Interface for column selection provided by the Decoder
    */
    class ADCV_CS_in_if : public sc_interface{
        public:
            int rangeStart;
            int rangeEnd;
            int columnId;
    };

    class ADCV_in_if : public sc_interface {
        public:
            int data[NCOLS];
    };

    class ADCV_out_if : public sc_interface {
        public:
            int data[NCOLS];
    };

    // Data coming out of the IADD
    class IADD_out_if : public sc_interface {
        public:
            int data[NCOLS/WEIGHT_PRECISION];
    };

    class ST_in_if : public sc_interface {
        public:
            int mode;
    };
    //class ADCV_
    /*
        Interface for CFG command. Decoder to...we'll see
    */
    class CFG_if : public sc_interface {
        public:
            char data[4];
    };


}
#endif




