
#ifndef S2_H
#define S2_H


SC_MODULE(S2) {
    sc_in<long> in1, in2;
    sc_out<double> o1, o2;
    sc_in<bool>    clk;  //clock

    void mainFunc();       //method implementing functionality

    //Counstructor
    SC_CTOR(S2) {
        SC_METHOD(mainFunc);   //Declare mainFunc as SC_METHOD and  
        sensitive_pos << clk;  //make it sensitive to positive clock edge
    }

};

#endif