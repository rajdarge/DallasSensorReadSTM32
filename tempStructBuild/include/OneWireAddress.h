#ifndef ONEWIREADDRESS_H
#define ONEWIREADDRESS_H

#define VERYHIGHTEMPERATURE 200
#define VERYLOWTEMPERATURE -200
#define minValidtemp  -50
#define maxValidtemp  50

#define MAXSENSORS 6 // set the number of sensors rth
union OneWireAddress
{
    uint64_t ow_address_64;
    uint32_t ow_address_32[2];
    uint8_t ow_address_byte[8];
};

#endif // ONEWIREADDRESS_H

// just checking if stuff changes.
