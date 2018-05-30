#include <picc_temp_i2c.h>
#include <iostream>
#include <iomanip>
using namespace std ;

int main ( void )
{
	hdc1010 * dev1 , * dev2 ;
	dev1 = new hdc1010 ; dev2 = new hdc1010 ;
	dev1 -> begin(0x41) ; dev2 -> begin(0x43) ;

	cout << fixed ;
	cout <<  setprecision(3);

	cout << "Mf ID Dev1: 0x" << hex << dev1 -> readMfId()  << endl ;
	cout << "Mf ID Dev2: 0x" << hex << dev2 -> readMfId()  << endl ;
	uint64_t time = 0 ;
	while(true)
	{
		cout << "At " << dec << time << " seconds:" << endl ;
		cout << "Temp Dev1: " << dev1->readT() << " C" << endl ;
		cout << "Temp Dev2: " << dev2->readT() << " C" << endl ;
		cout << endl ;
		cout << "RH Dev1: " << dev1->readH() << " %" << endl ;
		cout << "RH Dev2: " << dev2->readH() << " %" << endl ;
		cout << endl ;
		time = time + 10 ;
		if ( time > 20 )
			break ;
		hdc1010::sleep(10000) ; //spit out numbers in 1 second intervals
		cout << "\x1b[A" ;
		cout << "\x1b[A" ;
		cout << "\x1b[A" ;
		cout << "\x1b[A" ;
		cout << "\x1b[A" ;
		cout << "\x1b[A" ;
		cout << "\x1b[A" ;
	}
	cout << endl ;
	cout << endl ;
	cout << endl ;
	cout << endl ;
	cout << endl ;
	cout << endl ;
	cout << endl ;

	cout << "Changing mode: dev1" << endl ;
	dev1->acquisition_mode(false) ;
	hdc1010::sleep(1000) ;
	time = 0 ;
	for ( time = 0 ; time < 20 ; time += 10 )
	{
		cout << "At " << dec << time << " seconds:" << endl ;
		//uint32_t val = dev1->getTRH() ;
		//uint16_t rh = val & 0xffff ;
		//uint16_t temp = (val>>16) & 0xffff ;
		//cout << hex << "Raw: 0x" << val << endl ;
		//cout << hex << "Temp: 0x" << (uint16_t)(val>>16) << endl ;
		//cout << hex << "Raw: 0x" << val << endl ;
		//cout << hex << "RH: 0x" << (uint16_t)(val) << endl ;
		//cout << hex << "Raw: 0x" << val << endl ;
		cout << "T: " << dev1 -> readT() << " C" << endl ;
		//cout << hex << "Raw: 0x" << val << endl ;
		cout << "RH: " << dev1 -> readH() << " %" << endl ;
		//cout << hex << "Raw: 0x" << val << endl ;
		cout << endl ; cout << endl ; cout << endl ; cout << endl ;
		hdc1010::sleep(10000) ;
	}
	dev1->acquisition_mode(true);
	delete dev1 ; 
	delete dev2 ;
	return 0 ;
}
