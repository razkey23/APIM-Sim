#include "Decoder.h"
#include <math.h>
#include <systemc.h>
#include <typeinfo>

Decoder::Decoder(sc_module_name nm, OReg *oreg_obj,Adder *adder_obj,Sh *sh_obj,Csr *csr_obj, Crossbar *crossbar_obj, Rsr *rsr_obj, Adc *adc_obj, IBuffer *ibuffer_obj) : sc_module(nm) ,
    p_DoA("p_DoA"),
    p_DoS("p_DoS"),
    p_DoR("p_DoR"),
    p_done_crossbar("p_done_crossbar"),
    p_done_SH("p_done_SH"),
    p_done_ADC("p_done_ADC"),
    p_LDRSR("p_LDRSR"),
    p_LDCSRIMM("p_LDCSRIMM"),
    crossbar(crossbar_obj),
    rsr(rsr_obj),
    adc(adc_obj),
    ibuffer(ibuffer_obj),
    csr(csr_obj),
    sh(sh_obj),
    adder(adder_obj),
    oreg(oreg_obj),
    loopreg(0)
{

    sc_done_crossbar = 1; sc_done_SH=0; sc_done_ADC=1; sc_done_CSR=1;

    
    

    SC_THREAD(clock_pos);
    sensitive<<clock.pos();
    dont_initialize();

    SC_THREAD(clock_neg);
    sensitive<<clock.neg();
    dont_initialize();

    SC_THREAD(fun_done_crossbar);
    sensitive<<p_done_crossbar.pos();
    

    SC_THREAD(fun_done_SH);
    sensitive<<p_done_SH.pos();
    

    SC_THREAD(fun_done_ADC);
    sensitive<<p_done_ADC.pos();

    SC_THREAD(decode);
    sensitive<< event_done_crossbar << event_done_SH << event_done_ADC << event_clock_pos;
    dont_initialize();
    
    SC_THREAD(exe);
    sensitive<<event_stage_exe << event_clock_pos;;
    dont_initialize();


    infile.open(INSTRUCTION_FILE,fstream::in);

}

Decoder::~Decoder() {
    infile.close();
}

void Decoder::clock_pos() {
    while(true){
        wait();
        event_clock_pos.notify();
    }
}

void Decoder::clock_neg() {
    while(true){
        wait();
        event_clock_neg.notify();
    }
}

void Decoder::fun_done_crossbar() {
    while(true){
        wait();
        sc_done_crossbar=1;
        event_done_crossbar.notify();
    }
}

void Decoder::fun_done_SH() {
    while(true){
        wait();
        sc_done_SH = 1;
        event_done_SH.notify();
    }
}

void Decoder::fun_done_ADC() {
    while(true){
        wait();
        sc_done_ADC = 1;
        event_done_ADC.notify();
    }
}

void Decoder::decode() {
    
    enum opcodes {
        CFG,
        LDRSR,
        LDCSRIMM,
        RSELRSR,
        CSELCSR,
        DoA, 
        DoS,
        ADCV,
        SH,
        IADD,
        ST,
        LABEL,
        SETLOOP,
        DEC,
        BNE
    };
    map<string, opcodes> opcode;

    opcode["CFG"] = CFG;
    opcode["LDRSR"] = LDRSR;
    opcode["RSELRSR"] = RSELRSR;
    opcode["LDCSRIMM"] = LDCSRIMM;
    opcode["CSELCSR"] = CSELCSR;
    opcode["DoA"] = DoA;
    opcode["SH"] = SH;
    opcode["ADCV"] = ADCV;
    opcode["IADD"] = IADD;
    opcode["ST"] = ST;

    opcode["LABEL"]    = LABEL;
    opcode["SETLOOP"]  = SETLOOP;
    opcode["DEC"]      = DEC;
    opcode["BNE"]      = BNE;
    

    //string line; 
    wait(SC_ZERO_TIME);

    //while(getline(infile,line) && !infile.eof()) {
    while(true) {
        // NEW LINES
        streampos linePos = infile.tellg();  // store file position after reading line
        string line;
        if(!getline(infile,line)) break;

        // 2) Save position in case it's a label
        
        istringstream iss1(line);
        iss1 >> opcode1;
        for (int i=1;i<=1;i++) wait(event_clock_pos);
        p_DoA = false;
        p_DoS = false;
        //for (int i=1;i<=1;i++) wait(event_clock_pos);
        opcode2 = opcode1;
        wait(SC_ZERO_TIME);
        switch (opcode[opcode1]) {
            case LDRSR:
            {
                cout<<"LDRSR decoded at "<<sc_time_stamp()<<endl;
                int row_group;
                iss1 >> row_group;
                p_LDRSR->row_group = row_group;
                //ibuffer->LDRSR();
                //rsr->update_state();
                event_stage_exe.notify();
                break;
            }
            case LDCSRIMM:
            {
                cout<<"LDCSRIMM decoded at "<<sc_time_stamp()<<endl;
                // Read data from Instruction
                int column_group; 
                iss1 >> column_group; // Read col_group
                p_LDCSRIMM->column_group = column_group;
                string immediate;  // Read Immediate
                iss1 >> immediate;
                
                // Execute : Pass data to p_LDCSRIMM port
                for (int i=0; i<COLUMNS_PER_GROUP;i++) {
                    p_LDCSRIMM->data[i] = immediate[i];
                }
                event_stage_exe.notify();
                break;
            }
            case RSELRSR:
            {
                cout<<"RSELRSR decoded at "<<sc_time_stamp()<<endl;
                // We will trigger a function in RSR to set data out to its content
                event_stage_exe.notify();
                break;
            }
            case CSELCSR:
            {
                cout<<"CSELCSR decoded at "<<sc_time_stamp()<<endl;
                event_stage_exe.notify();
                break;
            }
            case DoA:
            {
                
                cout<<"DoA decoded at "<<sc_time_stamp()<<endl;
                p_DoA.write(true);
                sc_done_crossbar = 0;
                wait(SC_ZERO_TIME);
                break;
            }
            case SH:
            {
                if (sc_done_crossbar==0)
					wait(event_done_crossbar);
                cout<<"SH decoded at "<<sc_time_stamp()<<endl;
                sc_done_SH=0; 
                p_DoS = true;
                //sc_done_SH=0; 
                //p_DoS = true;
                //wait(SC_ZERO_TIME);
                //event_stage_exe.notify();
                break;
            }
            case ADCV:
            {
                cout<<"SC DONE SH is" << sc_done_SH<< sc_time_stamp()<< endl;
                if (sc_done_SH==0) {
                    wait(event_done_SH);
                }
                if (sc_done_ADC==0) {
                    wait(event_done_ADC);
                }
                cout<<"ADCV decoded at "<<sc_time_stamp()<<endl;
                sc_done_ADC=0;
                int rangeStart,rangeEnd,columnId; 
                iss1 >> rangeStart;
                iss1 >> rangeEnd;
                iss1 >> columnId;
                p_ADCVCS->rangeStart = rangeStart;
                p_ADCVCS->rangeEnd = rangeEnd;
                p_ADCVCS->columnId = columnId;
                event_stage_exe.notify();
                break;

            }
            case IADD:
            {   
                if (sc_done_ADC == 0) wait(event_done_ADC);

                cout<<"IADD decoded at "<<sc_time_stamp()<<endl;
                event_stage_exe.notify();
                break;
            }
            case ST:
            {
                cout<<"ST decoded at "<<sc_time_stamp()<<endl;
                int mode;
                iss1 >> mode;
                p_ST->mode = mode;
                event_stage_exe.notify();
                break;
            }
            case LABEL:
            {
                string labelName;
                iss1 >> labelName;
                labelMap[labelName] = linePos;
                cout << "[Decoder] Found label " << labelName << " at " << linePos << endl;
                break;
            }
            case SETLOOP:
            {
                int val;
                iss1 >> val;
                loopreg = val;
                cout << "[Decoder] SETLOOP " << val << " => loopReg=" << loopreg << endl;
                break;
            }
            case DEC:
            {
                loopreg--;
                cout << "[Decoder] DEC => loopReg=" << loopreg << endl;
                break;
            }
            case BNE:
            {
                string labelName;
                iss1 >> labelName;
                cout << "[Decoder] BNE " << labelName << " => loopReg=" << loopreg << endl;
                if (loopreg != 0) {
                    infile.clear();
                    infile.seekg(labelMap[labelName],ios::beg);
                    // skip the final "pc++" effect
                    continue;
                }
                break;
            }
            default:
                break;
        }
        
    }

}

void Decoder::exe() {
    enum opcodes {
        CFG,
        LDRSR,
        LDCSRIMM,
        RSELRSR,
        CSELCSR,
        DoA, 
        DoS,
        ADCV,
        SH,
        IADD,
        ST
    };
    map<string, opcodes> opcode;

    opcode["CFG"] = CFG;
    opcode["LDRSR"] = LDRSR;
    opcode["RSELRSR"] = RSELRSR;
    opcode["LDCSRIMM"] = LDCSRIMM;
    opcode["CSELCSR"] = CSELCSR;
    opcode["DoA"] = DoA;
    opcode["SH"] = SH;
    opcode["ADCV"] = ADCV;
    opcode["IADD"] = IADD;
    opcode["ST"] = ST;
    while (true) {
        wait(event_stage_exe);
        switch (opcode[opcode2]) {
            case LDRSR:
            {
                ibuffer->LDRSR();
                rsr->update_state();
                break;
            }
            case LDCSRIMM:
            {
                csr->LDCSRIMM();
                break;
            }
            case RSELRSR:
            {
                // We will trigger a function in RSR to set data out to its content
                rsr->RSELRSR();
                break;
            }
            case CSELCSR:
            {
                csr->CSELCSR();
                break;
            }
            case ADCV:
            {
                adc->activation();
                break;
            }
            case IADD:
            {
                adder->execute();
                break;
            }
            case ST:
            {
                oreg->ST();
                break;
            }
            default:
                break;
        }
    }
}