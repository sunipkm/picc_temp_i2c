#include <picc_temp_i2c.h>

int main ( void )
{
	hdc1010 dev1 , dev2 ;
	dev1.begin(0x41) ; dev2.begin(0x43) ;

	cout << "Mf ID Dev1: " << dev1.readMfID() << endl ;
	cout << "Mf ID Dev2: " << dev2.readMfID() << endl ;

	return 0 ;
}