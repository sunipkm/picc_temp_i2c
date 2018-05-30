extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <errno.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
}
//HDC1010, big-endian device

#ifndef PICC_TEMP_I2C
#define PICC_TEMP_I2C

#ifndef I2C_FILE
#define I2C_FILE "/dev/i2c-1" //for arduino
#endif

#ifndef I2C_ADDR
#define I2C_ADDR 0x40
#endif


typedef enum { //registers available
	TEMPERATURE = 0x00,
	HUMIDITY = 0x01,
	CONFIGURATION = 0x02,
	MANUFACTURER_ID = 0xFE,
	DEVICE_ID = 0xFF,
	SERIAL_ID_FIRST = 0xFB,
	SERIAL_ID_MID = 0xFC,
	SERIAL_ID_LAST = 0xFD,
} hdc1010_ptrs ;

typedef union {
	uint8_t rawData ;
	struct {
		//register bits
		uint8_t HumidityMeasurementResolution : 2; //8,9 [00 = 14bit res, 01 = 11bit res, 10 = 8bit res]
		uint8_t TemperatureMeasurementResolution : 1; //10 [0 for 14 bit, 1 for 11 bit]
		uint8_t BatteryStatus : 1; //11 [power indicator]
		uint8_t ModeOfAcquisition : 1; //12 [0 for stand-alone, 1 for simultaneous]
		uint8_t Heater : 1; //13 [0: Heater off, 1: Heater on.]
		uint8_t ReservedAgain : 1; //14 [Reserved bit]
		uint8_t SoftwareReset : 1; //15 [Reset device at 1]
	} ;	
} hdc1010_regs ;

class hdc1010 {
public:
	hdc1010();
	~hdc1010();
	bool begin(uint8_t address);
	
	uint16_t readMfId(); // 0x5449 ID of Texas Instruments
	uint16_t readDevId(); // 0x1000 ID of the device

	hdc1010_regs readReg();	
	void writeReg(hdc1010_regs reg);

   	void acquisition_mode(bool) ; //true for separate measure, false for simultaneous
	
	void heatUp(uint8_t seconds);

	float readT();
	float readH();

	static void sleep(uint32_t) ;
	
	static int numDev ;

private:
	uint8_t _address; //i2c slave address of the device
	
	uint16_t readData(uint8_t pointer); //read Data from the given register
	void writeData(uint8_t val) ; //write byte to the device (data/register addr)
	void readBytes(uint8_t * buf , uint8_t n ) ; //read n bytes from device and store it in buffer

	uint32_t getTRH() ; //return 4 bytes in 32bit int format, Temp H|L, Humid H|L and also stores it in the internal buffer for access through readT and readH functions. Output can be discarded in that case.

	int i2cdevbus ; //i2c bus that has been opened for the device

	uint8_t _mode : 2 ; //2 bit acquisition mode flag
	/*
	_mode = 00 : Non-simultaneous measurement
	_mode = 01 : Simultaneous measurement 
	*/
	uint8_t _trh : 2 ; //2 bit read flag
	/*
	_trh = 11 : T and RH data available
	_trh = 10 : T data available, RH has been read
	_trh = 01 : RH data available, T has been read
	_trh = 00 : Make measurement again
	*/

	/*
	_trh flag and _TRH_BUF are used only when simultaneous measurements are being made.
	*/

	uint32_t _TRH_BUF ; //dword buffer
};


const double PICC_POW16 = 65536.0 ;

#ifndef PICC_TIME_USEC
#define PICC_TIME_USEC 30000 //30ms (14bit temp: ~7ms, 14bit RH: ~7ms), with lots of head room.
#endif

#define PICC_CONVERT_T(x,y) ((((uint16_t)x<<8|y)*1.0/65536.0)*165-40) //higher byte, lower byte
#define PICC_CONVERT_H(x,y) ((((uint16_t)x<<8|y)*1.0/65536.0)*100)

#define PICC_WORD_T(x) ((x*1.0/65536.0)*165-40) //word
#define PICC_WORD_RH(x) (x*100./65536.)

#define PICC_INT_T(x)(PICC_WORD_T((uint16_t)(x>>16))) //dword
#define PICC_INT_RH(x)(PICC_WORD_RH((uint16_t)x)

#endif
