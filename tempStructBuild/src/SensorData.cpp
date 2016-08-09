#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstdio>
#include <assert.h>
#include <cstring>

#include "OneWireAddress.h"
#include "SensorData.h"
#include "NameMinMaxAveBreachVals.h"



using namespace std;

/***************
FUNCTION:SensorData Constructor
****************/
SensorData::SensorData()
{
    //cout << "Setting up SensorData" <<endl;
    const char LabelList[9][5] = {"Room","Frst","Scnd","Thrd","Frth","Ffth","Sixt","Svth","Eith"};//setup and initialise label.
    for (int i = 0; i <MAXSENSORS; i++)
	{
		strcpy(label[i],LabelList[i]);
		currentTemp[i] = VERYHIGHTEMPERATURE;
		timeofCurrentTemp[i] = time(NULL);
		ow_addr[i].ow_address_64 = 0;
	}
	PktSize = CRC32 = 0;
	//cout << "end of default constructor" << endl;
}

/***************
FUNCTION:SensorData::get_ID
ARG: int idIdx
PURPOSE: read OneWireAddress
RESOURCES: ow_addr[idIdx] from struct
RETURN: onewireaddress from array of addresses
TODO:
NOTES:
****************/
int SensorData::set_ID(OneWireAddress inputID) // return index of sensor
{
	static int indexCount = 0;
	while (indexCount <MAXSENSORS)
	{
		if (ow_addr[indexCount].ow_address_64 == 0 )
		{
			ow_addr[indexCount].ow_address_64 =inputID.ow_address_64;
			//cout << " " << indexCount << " value: " <<  ow_addr[indexCount].ow_address_64 << endl;
			//printf("zero address: %d value: %llx\n", indexCount , ow_addr[indexCount].ow_address_64 );
			indexCount++;
			return indexCount;
		}
		else if (ow_addr[indexCount].ow_address_64 != inputID.ow_address_64)
		{
			ow_addr[indexCount].ow_address_64 =inputID.ow_address_64;
			//cout << "found non-duplicate address " << indexCount << " value: " <<  ow_addr[indexCount].ow_address_64 << endl;
			//printf("found non-duplicate address:  %d value: %llx\n", indexCount,  ow_addr[indexCount].ow_address_64 );
			indexCount++;
			return indexCount;
		}
		indexCount++;
		return -1;
	}
	return -1;
}

/***************
FUNCTION:SensorData::get_ID
ARG: int idIdx
PURPOSE: read OneWireAddress
RESOURCES: ow_addr[idIdx] from struct
RETURN: onewireaddress from array of addresses
TODO:
NOTES:
****************/
int SensorData::find_ID(OneWireAddress inputID)
{

    int indexCount = 0;
	while (indexCount <MAXSENSORS)
    {
        OneWireAddress tempAddress = ow_addr[indexCount];
        if (tempAddress.ow_address_64 == inputID.ow_address_64)
            return indexCount;
        else
            indexCount++;
    }
    indexCount = set_ID(inputID);
    return indexCount;
}
OneWireAddress SensorData::get_ID(int idIdx) const {return ow_addr[idIdx];} // read lable to idenfiy which sensor
const char* SensorData::getLabel(OneWireAddress inputID)
{
	int indx = find_ID(inputID);
	return label[indx];

}
const char* SensorData::getLabel(int indx) const {return label[indx];}
uint16_t SensorData::getPktSize(void) const {return PktSize;}

bool SensorData::checkValidAddress(OneWireAddress inputID) const
{
	/*OneWireAddress * tempAddess = inputID
	inputID.ow_address_byte[7] = crc8(&tempAddess.ow_address_byte,7);*/
	// need to complete this section.
	//return (inputID.ow_address_byte[7] = crc8(inputID.ow_address_byte, 7));
}

uint32_t SensorData::getCRC32(void) const {return CRC32;}

//void SensorData::setCRC32(uint32_t input) {CRC32 = input;}
