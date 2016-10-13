/*!
 * @file DFRobot_LIS2DH12.cpp
 * @brief DFRobot's Read LIS2DH12 data
 * @n This example is in order to achieve the serial port to receive LIS2DH12 back to the data
 *
 * @copyright	[DFRobot](http://www.dfrobot.com), 2016
 * @copyright	GNU Lesser General Public License
 * @author [Wuxiao](xiao.wu@dfrobot.com)
 * @version  V1.0
 * @date  2016-10-13
 * @https://github.com/DFRobot/DFRobot_LIS2DH12
 */

#include <DFRobot_LIS2DH12.h>
#include <Wire.h>

uint8_t DFRobot_LIS2DH12::sensorAddress = 0x19; // 0x18 or 0x19

uint8_t DFRobot_LIS2DH12::init(void)
{
	uint8_t ctrl_reg_values[] = {0x2F, 0x00, 0x00, 0x00, 0x00, 0x00};
	return writeReg(0xA0, ctrl_reg_values, sizeof(ctrl_reg_values));
}

void DFRobot_LIS2DH12::readXYZ(int16_t &x, int16_t &y, int16_t &z) //read x, y, z data
{
	uint8_t sensorData[6];
	readReg(0xA8, sensorData, 6);
	x = ((int8_t)sensorData[1])*256+sensorData[0]; //return values
	y = ((int8_t)sensorData[3])*256+sensorData[2];
	z = ((int8_t)sensorData[5])*256+sensorData[4];
}
void DFRobot_LIS2DH12::mgScale(int16_t &x, int16_t &y, int16_t &z)
{
	x = (int32_t)x*1000/(1024*32/2); //transform data to millig, for 2g scale axis*1000/(1024*16),
	y = (int32_t)y*1000/(1024*32/2); //for 4g scale axis*1000/(1024*8),
	z = (int32_t)z*1000/(1024*32/2); //for 8g scale axis*1000/(1024*4)
}
uint8_t DFRobot_LIS2DH12::readReg(uint8_t regAddress)
{
	uint8_t regValue;
	Wire.beginTransmission(sensorAddress);
    Wire.write(regAddress);
    Wire.endTransmission();
    Wire.requestFrom(sensorAddress, (uint8_t)1);
	regValue = Wire.read();
	Wire.endTransmission(true);
	return regValue;
}

void DFRobot_LIS2DH12::readReg(uint8_t regAddress, uint8_t *regValue, uint8_t quanity, bool autoIncrement)
{   
	regAddress = 0x80 | regAddress; //set MSB to 1 to auto increment
	Wire.beginTransmission(sensorAddress);
	if(autoIncrement){
		Wire.write(regAddress);
		Wire.endTransmission(false);
		Wire.requestFrom(sensorAddress, quanity);
		for(int i=0 ; i<quanity ; i++)
			regValue[i] = Wire.read();
	}
	else {
		for(size_t i=0 ; i<quanity ; i++){
			Wire.write(regAddress+i);
			Wire.endTransmission(false);
			Wire.requestFrom(sensorAddress, (uint8_t)1);
			regValue[i] = Wire.read();
			if( i<(quanity-1) ){
				Wire.endTransmission(false);
				Wire.beginTransmission(sensorAddress);
			}
		}
	}
		Wire.endTransmission(true);
}
uint8_t DFRobot_LIS2DH12::writeReg(uint8_t regAddress, uint8_t regValue)
{
	Wire.beginTransmission(sensorAddress);
	Wire.write(regAddress);
	Wire.write(regValue);
	return Wire.endTransmission(true);
}
uint8_t DFRobot_LIS2DH12::writeReg(uint8_t regAddress, uint8_t *regValue, size_t quanity, bool autoIncrement)
{   
	Wire.beginTransmission(sensorAddress);
	if(autoIncrement) {
		Wire.write(regAddress);
		Wire.write(regValue, quanity);
	}
	else {
		for(size_t i=0 ; i<quanity ; i++){
			Wire.write(regAddress+i);
			Wire.write(regValue[i]);
			if( i<(quanity-1) ){
				Wire.endTransmission(false);
				Wire.beginTransmission(sensorAddress);
			}
		}
	}
	return Wire.endTransmission(true);
}