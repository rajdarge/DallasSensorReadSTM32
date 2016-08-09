#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <assert.h>
#include <cstring>

#include "OneWireAddress.h"
#include "SensorData.h"
#include "NameMinMaxAveBreachVals.h"

//#define VERYHIGHTEMPERATURE 200
//#define VERYLOWTEMPERATURE -200
//#define minValidtemp  -10
//#define maxValidtemp  50
//#define MAXSENSORS 6 // set the number of sensors rth

//#define OutputDebug =1

using namespace std;
typedef uint8_t byte;


//create a 64bit value to hold a one wire address, but leave access to high and low byte
//running into endian problems?


/**************************************************************************
// FUNCTION PROTOTYPES
**************************************************************************/
void DS18_RandAddValid (uint32_t& HighVal, uint32_t& LowVal);
uint8_t crc8(uint8_t *addr, uint8_t len);
void Send_Bytes(byte *message, uint16_t messageLen);
uint32_t reassembleStruct(char * asciiHexValues , byte *inputStruct, uint16_t structLen);
uint32_t crc32b(byte *message, uint16_t messageLen );
void setPktSize(SensorData* rawDatainput);
bool setCRC32(SensorData* rawDatainput);
/**************************************************************************
// MAIN
**************************************************************************/
int main()
{
   NameMinMaxAveBreachVals SensorSetup1;
   SensorData * ptrSensors1 = &SensorSetup1;
    //******* unit Testing *******//
    srand ( (unsigned) (time(0)));
    OneWireAddress TempAddresses[MAXSENSORS];
    float randomTemp[MAXSENSORS];
    for (int i = 0; i < MAXSENSORS; i++) //initialise array
    {
        DS18_RandAddValid(TempAddresses[i].ow_address_32[1],TempAddresses[i].ow_address_32[0]);
        //printf("OneWireAddress64 %llx\n",TempAddresses[i].ow_address_64);
        //randomTemp[i] = 1.0 +  10 * (float)( rand())/ (float) RAND_MAX;
        //printf("OneWireAddress32 %x %x\n",TempAddresses[i].ow_address_32[1],TempAddresses[i].ow_address_32[0]);
        OneWireAddress newtemp = TempAddresses[i];
        SensorSetup1.set_ID(newtemp);
    }
    printf("\n");

    for (int i = 0; i < 50; i++) //set temp for array by searching.
	{
		float randomTemp = -10.0  +  60 * (float)( rand())/ (float) RAND_MAX;
		int randSensor = rand() % MAXSENSORS; //randomize acrross available sensors only
		SensorSetup1.set_temp(TempAddresses[randSensor], randomTemp); //get new index
		float storedTemp = SensorSetup1.get_temp(TempAddresses[randSensor]);
		//printf("Index: %d idx: %llx StoredTemp: %f\n", randSensor , TempAddresses[randSensor].ow_address_64,storedTemp); //output to screen
	}

	for (int i = 0; i < MAXSENSORS ; i++)
	{
		//char * currentLabel[5] = &SensorSetup1.getLabel(i) ;
		cout << SensorSetup1.getLabel(i) << endl;
		//strcpy(currentLabel,SensorSetup1.getLabel(i));
		printf("Sensor: %d %s, MaxTemp: %f rTime: %ld\n", i, SensorSetup1.getLabel(i) , SensorSetup1.getMaxTemp(TempAddresses[i]),SensorSetup1.getMaxTime(TempAddresses[i]));
		printf("Sensor: %d %s, MinTemp: %f rTime: %ld\n", i, SensorSetup1.getLabel(i) , SensorSetup1.getMinTemp(TempAddresses[i]),SensorSetup1.getMinTime(TempAddresses[i]));
	}

	//uint16_t structSize = sizeof(Sensors1);
	setPktSize(ptrSensors1);
	setCRC32(ptrSensors1); // disregard return value
	//Sensors1.setPktSize(structSize); //set pkt size.
	//uint32_t newCRC = crc32b((byte *) &Sensors1, ( structSize - 4));
	//Sensors1.setCRC32(newCRC);
	Send_Bytes((byte *) ptrSensors1 , ptrSensors1->getPktSize());

   return 0;
}

/***************
FUNCTION:DS18_RandAddValid
ARG:
PURPOSE:generate  semi random address for use with struct
RESOURCES:
RETURN:change actua
TODO:
NOTES:
****************/
void DS18_RandAddValid (uint32_t& HighVal, uint32_t& LowVal)
{
/*
// structure of one wire bus is
//LSByte = type of device
//MSByte = CRC8
//address[0] = 0x10 for ds18b20
//address[7] = CRC8
*/
    uint8_t OneWireAdd[8];
    OneWireAdd[0]= 0x10; // set value for DS18b20
	//printf("byte 0 %x", OneWireAdd[0])
    for (int i = 1; i < 7; i++)
    {
        OneWireAdd[i] = rand();
    }
    OneWireAdd[7] = crc8(OneWireAdd,7);

    //cout << " valid address: ";
    //cout << endl;
    HighVal = (OneWireAdd[0] << 24) + (OneWireAdd[1] << 16) + (OneWireAdd[2] << 8) + OneWireAdd[3];
    LowVal = (OneWireAdd[4] << 24) + (OneWireAdd[5] << 16) + (OneWireAdd[6] << 8) + OneWireAdd[7];
    //printf("high and low byte %x %x \n", HighVal, LowVal);
}
/***************
FUNCTION:crc8
ARG: pointer to byte array, length of byte array
PURPOSE:Calculate CRC8 for an array of bytes of length len
RESOURCES:
RETURN:return CRC8 as uint8_t
TODO:
NOTES:taken from onewire library
****************/
uint8_t crc8(uint8_t *addr, uint8_t len)
{
	uint8_t crc = 0;
	while (len--)
	{
		uint8_t inbyte = *addr++;
		for (uint8_t i = 8; i; i--)
		{
			uint8_t mix = (crc ^ inbyte) & 0x01;
			crc >>= 1;
			if (mix) crc ^= 0x8C;
                inbyte >>= 1;
		}
	}
	return crc;
}


/***************
FUNCTION:Send_Bytes
ARG:
  Pointer to a struct of any sort cast as a byte pointer
  Length of struct to sent.
PURPOSE:
  Send a struct of type Weather Data over Serial port with CRC32
RESOURCES: Weather Underground data, NTP Clock
RETURN: nil
TODO:
WIll need a weather data Packer Recieve on STM32
with ability to resend a packet.

NOTES:  will initialise with a text character
****************/
void Send_Bytes(byte *message, uint16_t messageLen)
{
	printf("T%04d\n",messageLen);
	uint8_t * BytePointer = (uint8_t *) message;
	for (int i = 0; i < messageLen; i ++)
	{
		//char ByteToPrint[3];
		printf("%02x", BytePointer[i]);
		//cout <<  ByteToPrint ;
		//Serial.write(BytePointer[i]);
	}
	printf("\n");
	BytePointer = (uint8_t *) message; // reset pointer
	for (int i = 0; i < messageLen; i ++)
	{
		//char ByteToPrint[3];
		byte SingledOut = ((char) BytePointer[i]);
		if (isalnum((int) SingledOut))
		{
			cout << SingledOut << " ";
		}
		else
		{
			cout << "  ";
		}
		//cout <<  ByteToPrint ;
		//Serial.write(BytePointer[i]);
	}
	printf("\n");
	//Serial.flush();
}

/***************
FUNCTION: reassembleStruct
ARG: asciiHexValues - char string of hex values must be zero terminated, inputStruct - struct must be cast as byte pointer, structLen - length of struct in byte
PURPOSE: Take and ascii set of values with no space and convert them to binary - to allow tokenizing of data.
RESOURCES: needs struct created, will replace values,
RETURN:  CRC32bit
TODO: actual code calculate CRC
NOTES: Utilized in TOKENized serial input.
****************/
uint32_t reassembleStruct(char * asciiHexValues , byte *inputStruct, uint16_t structLen)
{
	//uint16_t memCntr; //place hodler for memory count
	cout << "structlen " << structLen << endl;
	uint8_t *rx_buffer = (uint8_t*) malloc(structLen+1);
	for(uint16_t memCntr=0 ;memCntr <= (strlen(asciiHexValues) - 1) ; memCntr++)
	{
		static uint16_t bufferCounter = 0; // initialize buffer counter
		static uint8_t lowNibble, highNibble; //store low nibble and high nibble values
		uint8_t currAsciNib = asciiHexValues[memCntr]; //load character from zero terminated string
		if(!(memCntr % 2)) //every second value starting with 0
		{
			if (currAsciNib <58 )
				highNibble = currAsciNib - 48; //ascii values corresponding to 0-9
			else
				highNibble = currAsciNib - 87; //ascii values in lowercase a - f
		}
		else //every second value starting with 1
		{
			if (currAsciNib <58 )
				lowNibble = currAsciNib - 48; //ascii values corresponding to 0-9
			else
				lowNibble = currAsciNib - 87; //ascii values in lowercase a - f
			int byteVal = lowNibble + (highNibble << 4);
			//printf("EndByteVal: %d %02X \n", bufferCounter, byteVal);
			rx_buffer[bufferCounter] = byteVal;
			bufferCounter++;
		}
	}
	memcpy(inputStruct,rx_buffer,structLen);
	uint32_t calcNewCRC = crc32b(inputStruct, structLen - 4);
	return calcNewCRC;
}


/***************
FUNCTION:Calculate crc32 of any message.
ARG:
  Pointer to a weatherData_packet struct
  Length of struct to sent.
PURPOSE:
  Send a struct of type Weather Data over Serial port with CRC32
RESOURCES: Weather Underground data, NTP Clock
RETURN: nil
TODO:
WIll need a weather data Packer Recieve on STM32
with ability to resend a packet.

NOTES:  will initialise with a text character
****************/
uint32_t crc32b(byte *message, uint16_t messageLen )
{
   int i, j;
   unsigned int cur_byte, crc, mask;
   i = 0;
   crc = 0xFFFFFFFF;
   while (i < messageLen)
   {
      cur_byte = message[i];            // Get next byte.
      crc = crc ^ cur_byte;
      for (j = 7; j >= 0; j--)
    {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i++;
   }
   return ~crc;
}
void setPktSize(SensorData* rawDatainput) {	rawDatainput->PktSize = sizeof(*rawDatainput);}
bool setCRC32(SensorData* rawDatainput)
{
	 if (rawDatainput->PktSize != 0 ) // check if the PktSize has been initialised.
	 	 crc32b((byte *) rawDatainput, ((rawDatainput->PktSize) - 4));
	 else return false;
	 return true;
}
