#include <iostream>
#include <cmath>

using namespace std;

void sigma(){
    double Nll_sb = -286277.0, Nll_b = -285933.0;
    double q0 = 2*(Nll_b - Nll_sb);
	double Z = sqrt(q0);

	cout << "--------------------------------" << endl;
	cout << "NLL (s+b) = " << Nll_sb << endl;
	cout << "NLL (b)   = " << Nll_b << endl;
	cout << "q0        = " << q0 << endl;
	cout << "Significance Z = " << Z << " sigma" << endl;
	cout << "--------------------------------" << endl;
}
    