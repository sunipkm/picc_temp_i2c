# PICTURE-C Temperature Controller on I2C Bus

Controller driver for [HDC1010 Temperature and Humidity Sensor from Texas Instruments](http://www.ti.com/product/HDC1010).
The system is on an I2C bus.

#

# How to get

On command line execute `git clone https://github.com/sunipkmukherjee/picc_temp_i2c.git`

# How to build

After cloning the repository, `cd` into the directory and execute `make`.
This will built the shared library `build/libpicc_i2c_thsensor.so` and the example binary `example/example`. To execute the example binary you can execute `make install` (with `sudo` if you do not have write permission in `/usr/local/lib`) to install the library in `/usr/local/lib` and then fire up the example binary. This will work if you have your `LD_LIBRARY_PATH` variable set to include `/usr/local/lib`. Otherwise, you can execute 
```
export LD_LIBRARY_PATH=$PWD/build:$LD_LIBRARY_PATH
``` 
on a BASH shell or 
```
setenv LD_LIBRARY_PATH $PWD/build:$LD_LIBRARY_PATH
```
on a Corn shell to append the current build path to the shared library file search path, and then execute the example binary.

You can execute `make clean` to delete the built objects, and `make spotless` to restore the directory to the state when it was cloned.

`make uninstall` can be executed to delete the installed shared library file from `/usr/local/lib` if it was installed.

The debug signals can be turned on by specifying the `HDC1010_DEBUG` option during make in `CXXFLAGS` using 
```
make CXXFLAGS=-DHDC1010_DEBUG
```

Further, the option `I2C_FILE` can be used to provide the I2C device file name to the library.

Note: Since the variable is being passed to the preprocessor through make, it should be done in the following format:
```
make CXXFLAGS="-DI2C_FILE=\\\"/path/to/i2c\\\""
```
This ensures that the string is passed properly to the preprocessor.


# Functionalities provided by the library:

The driver is written in C++, hence each device present can be described by individual objects. The following methods are defined on these objects:

The objects are of the class `hdc1010`.
  
1. `begin(address)`:

   This method opens the slave device address (e.g. `0x41`) on the I2C bus specified by `I2C_FILE` macro during compilation. Returns `true` on success. This should be tracked in order to obtain reliable information from the device. Returns `false` if the device fails to respond to `ioctl` requests or if the I2C bus fails to open. After the device is opened, it is by default set to measure temperature and humidity independently.
  
2. `acquisition_mode(bool)`:

   This method sets the acquisition mode of the device. If this method is called with `false` as the input, then it sets the device for simultaneous measurement of temperature and relative humidity. If this method is called with `true` as the input, then it sets the device for independent measurement of temperature and relative humidity. The functions to retrieve the temperature and relative humidity from the device are agnostic to the acquisition method used.
   
3. `readMfID()`:

   This method returns the manufacturer ID of the device (`0x5449` for Texas Instrument).
   
4. `readDevID()`:

   This method returns the address of the device on the I2C bus (should match the address initialized with).
   
5. `readReg()`:

   This method reads the configuration register (register `0x02`) and returns its contents as `hdc1010_regs` structure. The structure has the following accessible members:
      * `TemperatureMeasurementResolution` (1 bit):
         0 indicates 14 bit resolution, 1 indicates 11 bit resolution.
      * `HumidityMeasurementResolution` (2 bit):
         00 indicates 14 bit resolution, 01 indicates 11 bit resolution, 10 indicates 8 bit resolution.
      * `BatteryStatus` (1 bit):
         High value of this bit indicates that the system is not getting enough power to operate.
      * `ModeOfAcquisition` (1 bit):
         0 indicates stand-alone measurement for temperature and relative humidity. 1 indicates simultaneous measure of temperature and relative humidity.
      * `Heater` (1 bit):
         Turns on an included resistive element to heat up the chip. This is used to get rid of any condensation on the chip or for testing purposes.
      * `SoftwareReset` (1 bit):
         Resets the system to its initial state.
