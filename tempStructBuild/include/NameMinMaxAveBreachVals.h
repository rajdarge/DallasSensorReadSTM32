#ifndef NAMEMINMAXAVEBREACHVALS_H
#define NAMEMINMAXAVEBREACHVALS_H

//#define VERYHIGHTEMPERATURE 200
//#define VERYLOWTEMPERATURE -200
//#define minValidtemp  -10
//#define maxValidtemp  50
//#define MAXSENSORS 6 // set the number of sensors rth



class NameMinMaxAveBreachVals : public SensorData
{
private:
    float maxTemps[MAXSENSORS]; // max temp for each sensor                     4*10
    time_t maxReadTime[MAXSENSORS]; //time of last max reading                 4*10

    float minTemps[MAXSENSORS]; // min temp for each sensor                     4*10
    time_t minReadTime[MAXSENSORS]; //time of last min reading                 4*10

 	bool tooHot[MAXSENSORS];
	time_t tooHot_time[MAXSENSORS];
	time_t tooHot_duration[MAXSENSORS];

	bool tooCold[MAXSENSORS];
	time_t tooCold_time[MAXSENSORS];
	time_t tooCold_duration[MAXSENSORS];
public:
	uint8_t systemStatus; // status byte, not sure of the values yet 1*10
	NameMinMaxAveBreachVals();
	void set_temp(OneWireAddress,float); //set current temp, also set min and maxreturn index number
    float get_temp(OneWireAddress) ; //read current temp for display
    //void set_status(); // set status byte with relevant bit.
	float getMaxTemp(OneWireAddress) ;
    time_t getMaxTime(OneWireAddress) ;
    float getMinTemp(OneWireAddress) ;
    time_t getMinTime(OneWireAddress) ;
	uint8_t get_status(OneWireAddress) ; // read status byte for display
};

#endif // NAMEMINMAXAVEBREACHVALS_H
