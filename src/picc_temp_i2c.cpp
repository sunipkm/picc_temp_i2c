#include <picc_temp_i2c.h>

hdc1010::hdc1010() {
	return ;
}



bool hdc1010::begin ( uint8_t address )
{
	_address = address ;
	
	i2cdevbus = open(I2C_FILE, O_RDWR) ;

	if ( i2cdevbus < 0 )
	{
		printf ( "FILE %s Line %d: Error opening device %s, %s.\n" , __FILE__ , __LINE__ , I2C_FILE , strerror ( errno ) ) ;
		return false ;
	}

	if ( ioctl ( i2cdevbus , I2C_SLAVE, address ) < 0 ) 
	{
		printf ( "FILE %s Line %d: ioctl error: %s\n", __FILE__ , __LINE__ , strerror(errno) ) ;
		return false ;
	}

	writeData(CONFIGURATION) ; //write to the configuration register
	writeData(0x00) ; //rawData
	writeData(0x00) ; //flags

	/* config: default.
	Heater: Off
	Mode: T or RH
	T: 14 bits
	RH: 14 bits
	*/

	return true ;
}

float hdc1010::readT()
{
	uint16_t rawT = readData(TEMPERATURE);
	return (1.0*rawT/PICC_POW16) * 165 - 40 ;
}

float hdc1010::readH() {
	uint16_t rawH = readData(HUMIDITY);
	return (1.0*rawH / PICC_POW16) * 100;
}

uint16_t hdc1010::readMfId() {
	return readData(MANUFACTURER_ID);
}

hdc1010_regs hdc1010::readReg() {
	hdc1010_regs reg;
	reg.rawData = (readData(CONFIGURATION) >> 8);
	return reg;
}

void hdc1010::writeReg(hdc1010_regs reg) {
	writeData(CONFIGURATION);
	writeData(reg.rawData);
	writeData(0x00);
	usleep(20000);
}

void hdc1010::heatUp(uint8_t seconds) {
	hdc1010_regs reg = readReg();
	reg.Heater = 1;
	reg.ModeOfAcquisition = 1;
	writeReg(reg);

	uint8_t buf[4];
	for (int i = 1; i < (seconds*66); i++) {
		writeData(0x00);
		usleep(20000);
		readBytes(buf,4) ;
	}
	reg.Heater = 0;
	reg.ModeOfAcquisition = 0;
	writeReg(reg);
}

uint16_t hdc1010::readDevId() {
	return readData(DEVICE_ID);
}

uint16_t hdc1010::readData(uint8_t pointer)
{
	writeData(pointer) ;
	uint8_t buf1 , buf0 ;
	read ( i2cdevbus , &buf1 , 1 ) ;
	read ( i2cdevbus , &buf0 , 1 ) ;
	return (buf0 << 8 | buf1) ;
}

void hdc1010::writeData(uint8_t val)
{
	uint8_t buf = val ;
	write ( i2cdevbus , &buf , 1 ) ;
	usleep ( 20000 ) ; //wait 20ms before release to give time for response
}

void hdc1010::readBytes(uint8_t * buf , uint8_t n )
{
	read(i2cdevbus,buf,n) ;
	return ;
}
