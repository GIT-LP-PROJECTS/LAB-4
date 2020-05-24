
#include "systemc.h"
#include "S1.h"

//definition of multdiv method


void S1::mainFunc()
{
	o1.write(X < Y ? X - Y : Y - X);
	//o2.write(Y > 0 ? fact(Y) : fact(X));
	o2.write(Y > 0 ? ~Y : ~X );
}

/*long double S1::fact(int N)
{
	if (N < 0)
		return 0;
	if (N == 0)
		return 1;
	else
		return N * fact(N - 1);
}*/