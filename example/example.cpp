#include <picc_temp_i2c.h>
#include <iostream>
using namespace std ;

int main ( void )
{
	hdc1010 dev1 , dev2 ;
	dev1.begin(0x41) ; dev2.begin(0x43) ;

	cout << "Mf ID Dev1: " << dev1.readMfId()  << endl ;
	cout << "Mf ID Dev2: " << dev2.readMfId()  << endl ;

	cout << "Temp Dev1: " << dev1.readT() << endl ;
	cout << "Temp Dev2: " << dev2.readT() << endl ;

	return 0 ;
}
