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

NameMinMaxAveBreachVals::NameMinMaxAveBreachVals()
{
	//cout << "Setting up MinMax vals" << endl;

	for (int i = 0; i <= MAXSENSORS; i++)
	{
		maxTemps[i] = VERYLOWTEMPERATURE;
		maxReadTime[i] = time(NULL);
		minTemps[i] = VERYHIGHTEMPERATURE;
		tooHot[i] = tooCold[i] = false;
		tooHot_time[i] = tooHot_duration[i] = tooCold_time[i] = tooCold_duration[i] = 0;
	}
	//cout << "Ending up MinMax vals" << endl;
}


float NameMinMaxAveBreachVals::getMaxTemp(OneWireAddress inputID)
{
	int indexCount = find_ID(inputID);
	return maxTemps[indexCount];
}

float NameMinMaxAveBreachVals::getMinTemp(OneWireAddress inputID)
{
	int indexCount = SensorData::find_ID(inputID);
	return minTemps[indexCount];
}
time_t NameMinMaxAveBreachVals::getMaxTime(OneWireAddress inputID)
{
	int indexCount = SensorData::find_ID(inputID);
	return maxReadTime[indexCount];
}

time_t NameMinMaxAveBreachVals::getMinTime(OneWireAddress inputID)
{
	int indexCount = SensorData::find_ID(inputID);
	return minReadTime[indexCount];
}

float NameMinMaxAveBreachVals::get_temp(OneWireAddress inputID)
{
	int indexCount = SensorData::find_ID(inputID);
	return SensorData::currentTemp[indexCount];

} //read current temp for display

uint8_t NameMinMaxAveBreachVals::get_status(OneWireAddress inputID)
{
	int indexCount = SensorData::find_ID(inputID);
	return SensorData::statusByte[indexCount];
}
void NameMinMaxAveBreachVals::set_temp(OneWireAddress inputID , float inputTemp)
{
    //int sensor_resolve =0;
    int indexCount =  SensorData::find_ID(inputID);
	cout << "new temp: " << inputTemp << " maxtemp " << maxTemps[indexCount] << " minTemp " << minTemps[indexCount] << endl;

	if ((inputTemp < maxValidtemp) && (inputTemp > minValidtemp))
	{
		currentTemp[indexCount] = inputTemp;
		if (inputTemp > maxTemps[indexCount])
		{
			maxTemps[indexCount] = inputTemp;
			maxReadTime[indexCount] = time(NULL);

		}
		if (inputTemp < minTemps[indexCount])
		{
			minTemps[indexCount] = inputTemp;
			minReadTime[indexCount] = time(NULL);
		}

	}
    cout << "new temp: " << inputTemp << " maxtemp " << maxTemps[indexCount] << " minTemp " << minTemps[indexCount] << endl;
} //set current temp, also set min and max
