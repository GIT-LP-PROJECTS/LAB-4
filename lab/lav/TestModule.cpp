
#include "systemc.h"
#include "TestModule.h"
#include "S1.h"
#include "S2.h"

//definition of multdiv method
void TestModule::mainFunc()
{
	s1.o1(toA);
	s1.o2(toB);
	s2.in1(toA);
	s2.in2(toB);

	s1.clk(clk);	
	s2.clk(clk);
	
	s2.o1(o1);
	s2.o2(o2);
} // end of multdiv