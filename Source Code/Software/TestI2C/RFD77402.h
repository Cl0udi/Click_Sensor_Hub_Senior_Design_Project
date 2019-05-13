/*
  This is a library written for the RFD77402 Time of Flight sensor.
  SparkFun sells these at its website: www.sparkfun.com
  Written by Nathan Seidle @ SparkFun Electronics, June 9th, 2017
  The VCSEL (vertical-cavity surface-emitting laser) Time of Flight sensor 
  can accurately measure from 10cm to 200cm (2m) with milimeter precision.
  This library handles the initialization of the RFD77402 and bringing in of
  various readings.
  https://github.com/sparkfun/SparkFun_RFD77402_Arduino_Library
  Do you like this library? Help support SparkFun. Buy a board!
  Development environment specifics:
  Arduino IDE 1.8.1
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
  I just ported the sourcecode provided by sparkfun 
  (on https://github.com/sparkfun/SparkFun_RFD77402_Arduino_Library) to the 
  mbed-environment. So if you like it, please support them by e.g. buying a
  sparkfun-board :)
*/
 
#ifndef RFD77402_H_
#define RFD77402_H_
#endif
#ifndef MBED_H_
#define MBED_H_
#endif
 
#define RFD77402_ADDR 0x98 //8-bit --> 0x4C in 7-bit I2C Address
 
//Register addresses
#define RFD77402_ICSR 0x00
#define RFD77402_INTERRUPTS 0x02
#define RFD77402_COMMAND 0x04
#define RFD77402_DEVICE_STATUS 0x06
#define RFD77402_RESULT 0x08
#define RFD77402_RESULT_CONFIDENCE 0x0A
#define RFD77402_CONFIGURE_A 0x0C
#define RFD77402_CONFIGURE_B 0x0E
#define RFD77402_HOST_TO_MCPU_MAILBOX 0x10
#define RFD77402_MCPU_TO_HOST_MAILBOX 0x12
#define RFD77402_CONFIGURE_PMU 0x14
#define RFD77402_CONFIGURE_I2C 0x1C
#define RFD77402_CONFIGURE_HW_0 0x20
#define RFD77402_CONFIGURE_HW_1 0x22
#define RFD77402_CONFIGURE_HW_2 0x24
#define RFD77402_CONFIGURE_HW_3 0x26
#define RFD77402_MOD_CHIP_ID 0x28
 
#define RFD77402_MODE_MEASUREMENT 0x01
#define RFD77402_MODE_STANDBY 0x10
#define RFD77402_MODE_OFF 0x11
#define RFD77402_MODE_ON 0x12
 
#define CODE_VALID_DATA 0x00
#define CODE_FAILED_PIXELS 0x01
#define CODE_FAILED_SIGNAL 0x02
#define CODE_FAILED_SATURATED 0x03
#define CODE_FAILED_NOT_NEW 0x04
#define CODE_FAILED_TIMEOUT 0x05
 
#define I2C_SPEED_STANDARD        100000
#define I2C_SPEED_FAST            400000
 
class RFD77402 {
  public:
 
    //By default use Wire, standard I2C speed, and the defaul AK9750 address
    bool begin(I2C &i2cPort);
 
    uint8_t takeMeasurement(); //Takes a single measurement and sets the global variables with new data
    uint16_t getDistance(); //Returns the local variable to the caller
    uint8_t getValidPixels(); //Returns the number of valid pixels found when taking measurement
    uint16_t getConfidenceValue(); //Returns the qualitative value representing how confident the sensor is about its reported distance
    uint8_t getMode(); //Read the command opcode and covert to mode
 
    bool goToStandbyMode(); //Tell MCPU to go to standby mode. Return true if successful
    bool goToOffMode(); //Tell MCPU to go to off state. Return true if successful
    bool AgoToOnMode(); //Tell MCPU to go to on state. Return true if successful
    bool goToMeasurementMode(); //Tell MCPU to go to measurement mode. Takes a measurement. If measurement data is ready, return true
    
    uint8_t getPeak(); //Returns the VCSEL peak 4-bit value
    void setPeak(uint8_t peakValue); //Sets the VCSEL peak 4-bit value
    uint8_t getThreshold(); //Returns the VCSEL Threshold 4-bit value
    void setThreshold(uint8_t threshold); //Sets the VCSEL Threshold 4-bit value
    uint8_t getFrequency(); //Returns the VCSEL Frequency 4-bit value
    void setFrequency(uint8_t threshold); //Sets the VCSEL Frequency 4-bit value
 
    uint16_t AgetMailbox(); //Gets whatever is in the 'MCPU to Host' mailbox. Check ICSR bit 5 before reading.
    void reset(); //Software reset the device
    uint16_t getChipID(); //Returns the chip ID. Should be 0xAD01 or higher.
 
    //Retreive 2*27 bytes from MCPU for computation of calibration parameters
    //Reads 54 bytes into the calibration[] array
    //Returns true if new cal data is loaded
    bool getCalibrationData();
 
    uint16_t readRegister16(char addr); //Reads two bytes from a given location from the RFD77402
    uint8_t readRegister(char addr); //Reads from a given location from the RFD77402
    void writeRegister16(char addr, uint16_t val); //Write a 16 bit value to a spot in the RFD77402
    void writeRegister(char addr, char val); //Write a value to a spot in the RFD77402
 
    //Variables
    uint16_t distance;
    uint8_t validPixels;
    uint16_t confidenceValue;
    uint8_t calibrationData[54]; //Loaded by the 0x006 mailbox command
 
  private:
 
    //Variables
    I2C *_i2cPort; //The generic connection to user's chosen I2C hardware
};