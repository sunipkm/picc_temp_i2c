#include <picc_temp_i2c.h>
#include <iostream>
#include <iomanip>
<<<<<<< HEAD
=======

>>>>>>> fd6c6f643c366fd355b54c0aab7eeeb6c80b45d2
using namespace std ;

int main ( void )
{
<<<<<<< HEAD
	hdc1010 * dev1 , * dev2 ;
	dev1 = new hdc1010 ; dev2 = new hdc1010 ;
	dev1 -> begin(0x41) ; dev2 -> begin(0x43) ;
=======
	hdc1010 dev1 , dev2 ;
	dev1.begin(0x41) ; dev2.begin(0x43) ; //two devices

	cout << fixed << setprecision(3) ;
>>>>>>> fd6c6f643c366fd355b54c0aab7eeeb6c80b45d2

	cout << fixed ;
	cout <<  setprecision(3);

	cout << "Mf ID Dev1: 0x" << hex << dev1 -> readMfId()  << endl ;
	cout << "Mf ID Dev2: 0x" << hex << dev2 -> readMfId()  << endl ;
	uint64_t time = 0 ;
	while(true)
<<<<<<< HEAD
	{
		cout << "At " << dec << time << " seconds:" << endl ;
		cout << "Temp Dev1: " << dev1->readT() << " C" << endl ;
		cout << "Temp Dev2: " << dev2->readT() << " C" << endl ;
=======
	{	
		cout << "At " << dec << ++time << " seconds:" << endl ;
		cout << "Temp Dev1: " << dev1.readT() << " C" << endl ;
		cout << "Temp Dev2: " << dev2.readT() << " C" << endl ;
>>>>>>> fd6c6f643c366fd355b54c0aab7eeeb6c80b45d2
		cout << endl ;
		cout << "RH Dev1: " << dev1->readH() << " %" << endl ;
		cout << "RH Dev2: " << dev2->readH() << " %" << endl ;
		cout << endl ;
<<<<<<< HEAD
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
		uint32_t val = dev1->getTRH() ;
		//uint16_t rh = val & 0xffff ;
		//uint16_t temp = (val>>16) & 0xffff ;
		cout << hex << "Raw: 0x" << val << endl ;
		cout << hex << "Temp: 0x" << (uint16_t)(val>>16) << endl ;
		cout << hex << "Raw: 0x" << val << endl ;
		cout << hex << "RH: 0x" << (uint16_t)(val) << endl ;
		cout << hex << "Raw: 0x" << val << endl ;
		cout << "T: " << PICC_WORD_T((uint16_t)(val>>16)) << " C" << endl ;
		cout << hex << "Raw: 0x" << val << endl ;
		cout << "RH: " << PICC_WORD_RH((uint16_t)(val)) << " %" << endl ;
		cout << hex << "Raw: 0x" << val << endl ;
		cout << endl ; cout << endl ; cout << endl ; cout << endl ;
		hdc1010::sleep(10000) ;
=======
		hdc1010::sleep(1000) ; //spit out numbers in 1 second intervals
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
		cout << "x1b[A" ;
>>>>>>> fd6c6f643c366fd355b54c0aab7eeeb6c80b45d2
	}
	dev1->acquisition_mode(true);
	delete dev1 ; 
	delete dev2 ;
	return 0 ;
}
