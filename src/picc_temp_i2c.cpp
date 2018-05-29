#include <picc_temp_i2c.h>


int hdc1010::numDev = 0 ;

hdc1010::hdc1010() {
	numDev++ ;
	return ;
}

hdc1010::~hdc1010()
{
	#ifdef HDC1010_DEBUG
	fprintf(stderr,"%s : %d : Closing device %x.\n" , __FILE__ , __LINE__ , _address) ;
	fprintf(stderr,"%s : %d : Device %x Device Bus: %x.\n" ,__FILE__,__LINE__,_address,i2cdevbus);
	#endif
	
	numDev-- ;	
	
	close(i2cdevbus);
}


bool hdc1010::begin ( uint8_t address )
{
	_address = address ;
	
	i2cdevbus = open(I2C_FILE, O_RDWR) ;

	if ( i2cdevbus < 0 )
	{
		fprintf ( stderr,"FILE %s Line %d Function %s: Error opening device %s, %s.\n" , __FILE__ , __LINE__ , __FUNCTION__ , I2C_FILE , strerror ( errno ) ) ;
		return false ;
	}

	if ( ioctl ( i2cdevbus , I2C_SLAVE, address ) < 0 ) 
	{
		fprintf ( stderr,"FILE %s Line %d: ioctl error: %s\n", __FILE__ , __LINE__ , strerror(errno) ) ;
	}

	writeData(CONFIGURATION) ; //write to the configuration register
	writeData(0x00) ; //rawData
	writeData(0x00) ; //flags

	_mode = 0b00 ;

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
	uint16_t rawT ;
	if ( _mode ) //simultaneous
	{
		#ifdef HDC1010_DEBUG
		fprintf(stderr,"FILE %s line %d Function %s: Simultaneous mode, flag: 0x%x\n" , __FILE__ , __LINE__ , __FUNCTION__ , _trh );
		#endif
		if ( _trh == 0b01 || _trh == 0b00 ) //if unset or temperature has been read before
			getTRH() ; //make measurement because temperature data not available anymore
		rawT = (uint16_t)(_TRH_BUF>>16) ;
		if ( _trh == 0b11)
		{
			_trh = 0b01 ; //temperature has been read
		}
		if ( _trh == 0b10 ) //if humidity has been read set both to 0
		{
			_trh = 0b00 ;
			_TRH_BUF = 0x0000000 ;
		}
	}
	else
	{
		rawT = readData(TEMPERATURE) ;
	}
	return (1.0*rawT/PICC_POW16) * 165 - 40 ;
}

float hdc1010::readH() {
	uint16_t rawH ;

	if ( _mode ) //simultaneous
	{
		#ifdef HDC1010_DEBUG
		fprintf(stderr,"FILE %s line %d Function %s: Simultaneous mode, flag: 0x%x\n" , __FILE__ , __LINE__ , __FUNCTION__ , _trh );
		#endif
		if ( _trh == 0b10 || _trh == 0b00 ) //if humidity unavailable
			getTRH() ; //try to make measurement
		rawH = (uint16_t)(_TRH_BUF) ;
		if ( _trh == 0b11 )
			_trh = 0b10 ; //humidity has been read
		if ( _trh == 0b01 ) //if temp was read before, set flag to 00
		{
			_trh = 0b00 ;
			//also clear the buffer
			_TRH_BUF = 0x0000000 ;
		}
	}
	else
		rawH = readData(HUMIDITY) ;
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
	usleep(PICC_TIME_USEC);
}

void hdc1010::heatUp(uint8_t seconds) {
	hdc1010_regs reg = readReg();
	reg.Heater = 1;
	reg.ModeOfAcquisition = 1;
	writeReg(reg);

	uint8_t buf[4];
	for (int i = 1; i < (seconds*66); i++) {
		writeData(TEMPERATURE);
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
	uint8_t buf[2] ;
	read ( i2cdevbus , buf , 2 ) ;
	return (((uint16_t)buf[0]) << 8 | buf[1]) ;
}

void hdc1010::writeData(uint8_t val)
{
	uint8_t buf = val ;
	write ( i2cdevbus , &buf , 1 ) ;
	usleep ( PICC_TIME_USEC ) ; //wait 20ms before release to give time for response
}

void hdc1010::readBytes(uint8_t * buf , uint8_t n )
{
	read(i2cdevbus,buf,n) ;
	return ;
}

void hdc1010::sleep(uint32_t ms)
{
	while(ms--)
		usleep(1000);
	return ;
}

void hdc1010::acquisition_mode(bool state)
{
    if ( state )
    {
        hdc1010_regs reg = readReg();
        reg.Heater = 0 ; //just making sure
        reg.ModeOfAcquisition = 0 ; //non-simultaneous
        writeReg(reg) ;
        _mode = 0 ;
        return ;
    }
    else
    {
        hdc1010_regs reg = readReg();
        reg.Heater = 0 ; //just making sure
        reg.ModeOfAcquisition = 1 ; //simultaneous
        writeReg(reg) ;
        _mode = 1 ;
        return ;
    }
    return ;
}

uint32_t hdc1010::getTRH() //High word: T, Low word: RH
{
    uint32_t result = 0x0000 ;

    //check flags
    if ( _mode == 0b1 )
  	{
    	uint8_t buf[4];
    	writeData(0x00);
    	usleep(PICC_TIME_USEC);
    	readBytes(buf,4) ;
    	//encoding into 32bit word
    	result = ((uint32_t)buf[0]<<24)|((uint32_t)buf[1]<<16)|((uint32_t)buf[2]<<8)|buf[3] ;
    	_TRH_BUF = result ;
    	_trh = 0b11 ; //gcc only, both are available for read
    }
    return result ;
}
