#ifndef SENSORDATA_H
#define SENSORDATA_H

//#define VERYHIGHTEMPERATURE 200
//#define VERYLOWTEMPERATURE -200
//#define minValidtemp  -10
//#define maxValidtemp  50
//#define MAXSENSORS 6 // set the number of sensors rth

class SensorData
{
private:
    const char preamble[2] = {'T','P'}; //TP = temperature probe, no space.     2
    OneWireAddress ow_addr[MAXSENSORS]; // array of MAXSENSORS 64bit addresses   8*10
	char label[MAXSENSORS][5]; //quick access to label for printing             5*10
protected:
	float currentTemp[MAXSENSORS];
	time_t timeofCurrentTemp[MAXSENSORS];
	uint8_t statusByte[MAXSENSORS];
    uint16_t PktSize;                                                     //      2
	const char postamble[2] = {'z','Z'};                                  //      2
    uint32_t CRC32; // must be the last bit of the struct.                        4
public:
    int set_ID(OneWireAddress); // set Id from reading of address label
    OneWireAddress get_ID(int) const; // read lable to idenfiy which sensor
	int find_ID(OneWireAddress); //find index of crrent oneWireaddress, return -1 if not found (new)
	bool checkValidAddress(OneWireAddress) const;
    void setLabel(char);
    const char* getLabel(OneWireAddress);
    const char* getLabel(int) const;

    void setInternal_sens(int);
    voit getStatus

	uint16_t getPktSize(void) const;
    uint32_t getCRC32(void) const;

    friend void setPktSize(SensorData*);
    friend bool setCRC32(SensorData*);
	SensorData();
};

#endif // SENSORDATA_H
/* status byte

bit:
0	internal sensor
1	temperature breach
2
3
4
5
6
7


*/
