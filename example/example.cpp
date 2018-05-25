#include <picc_temp_i2c.h>
#include <iostream>
#include <iomanip>

using namespace std ;

int main ( void )
{
	hdc1010 dev1 , dev2 ;
	dev1.begin(0x41) ; dev2.begin(0x43) ; //two devices

	cout << fixed << setprecision(3) ;

	cout << "Mf ID Dev1:0x" << hex << dev1.readMfId()  << endl ;
	cout << "Mf ID Dev2: 0x" << hex << dev2.readMfId()  << endl ;
	uint64_t time = 0 ;
	while(true)
	{	
		cout << "At " << dec << ++time << " seconds:" << endl ;
		cout << "Temp Dev1: " << dev1.readT() << " C" << endl ;
		cout << "Temp Dev2: " << dev2.readT() << " C" << endl ;
		cout << endl ;
		cout << "RH Dev1: " << dev1.readH() << " %" << endl ;
		cout << "RH Dev2: " << dev2.readH() << " %" << endl ;
		cout << endl ;
		hdc1010::sleep(1000) ; //spit out numbers in 1 second intervals
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
	}
	return 0 ;
}
