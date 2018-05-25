#include <picc_temp_i2c.h>
#include <iostream>
using namespace std ;

int main ( void )
{
	hdc1010 dev1 , dev2 ;
	dev1.begin(0x41) ; dev2.begin(0x43) ;

	cout << "Mf ID Dev1: " << hex << dev1.readMfId()  << endl ;
	cout << "Mf ID Dev2: " << hex << dev2.readMfId()  << endl ;

	while(true)
	{
		cout << "Temp Dev1: " << dev1.readT() << " C" << endl ;
		cout << "Temp Dev2: " << dev2.readT() << " C" << endl ;
		cout << "RH Dev1: " << dev1.readH() << " %" << endl ;
		cout << "RH Dev2: " << dev2.readH() << " %" << endl ;
		hdc1010::sleep(1000) ; //spit out numbers in 1 second intervals
	}
	return 0 ;
}
