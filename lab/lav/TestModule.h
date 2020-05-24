
#ifndef TestModule_H
#define TestModule_H

#include "S1.h"
#include "S2.h"

SC_MODULE(TestModule) {
    sc_out<double> o1, o2;
    sc_in<bool> clk;  //clock

	S1 s1;
	S2 s2;

	sc_signal<long> toA, toB;

    void mainFunc();       //method implementing functionality

    //Counstructor
    SC_CTOR( TestModule ) : s1("S1"), s2("S2") {
		s1.o1(toA);
		s1.o2(toB);
		s2.in1(toA);
		s2.in2(toB);

		s1.clk(clk);
		s2.clk(clk);

		s2.o1(o1);
		s2.o2(o2); 

		sensitive_pos << clk;  //make it sensitive to positive clock edge
    }

};

#endif