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
		uint8_t HumidityMeasurementResolution : 2; //8,9
		uint8_t TemperatureMeasurementResolution : 1; //10
		uint8_t BatteryStatus : 1; //11
		uint8_t ModeOfAcquisition : 1; //12
		uint8_t Heater : 1; //13
		uint8_t ReservedAgain : 1; //14
		uint8_t SoftwareReset : 1; //15
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
   	uint32_t getTRH() ; //return 4 bytes in 32bit int format, Temp H|L, Humid H|L.
	
	void heatUp(uint8_t seconds);

	float readT();
	float readH();

	static void sleep(uint32_t) ;
	
	static int numDev ;

private:
	uint8_t _address;
	uint16_t readData(uint8_t pointer);
	void writeData(uint8_t val) ;
	void readBytes(uint8_t * buf , uint8_t n ) ;

	int i2cdevbus ;
};


const double PICC_POW16 = 65536.0 ;

#define PICC_TIME_USEC 30000

#define PICC_CONVERT_T(x,y) ((((uint16_t)x<<8|y)*1.0/65536.0)*165-40)
#define PICC_CONVERT_H(x,y) ((((uint16_t)x<<8|y)*1.0/65536.0)*100)

#define PICC_WORD_T(x) ((x*1.0/65536.0)*165-40)
#define PICC_WORD_RH(x) (x*100./65536.)

#endif
