
#ifndef S1_H
#define S1_H

// X = 9 - номер варіанту
// Y = R + L = 82 + 76 = 158 - сума ASCII першої літери Пр і Імя

const int X = 9;
const int Y = 158;

SC_MODULE(S1) {
    sc_out<long> o1, o2;  //output 1
    sc_in<bool>    clk;  //clock

    void mainFunc();       //method implementing functionality

    //long double fact(int N);

    //Counstructor
    SC_CTOR(S1) {
        SC_METHOD(mainFunc);   //Declare mainFunc as SC_METHOD and
        sensitive_pos << clk;  //make it sensitive to positive clock edge
    }
};

#endif