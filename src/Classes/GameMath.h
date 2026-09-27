#ifndef __GAMEMATH_H__
#define __GAMEMATH_H__

#include "EAttribute.h"
#include "TypeDefinitions.h"

class CCar;
class CTuning;
class CPart;

namespace GameMath
{
    struct Weights
    {
        float handling;
        float acceleration;
        float topspeed;
        float stock;
    };

    struct Result
    {
        float rawTopspeed;
        float rawAcceleration;
        float rawHandling;
        tValue topspeed;
        tValue acceleration;
        tValue handling;
        Weights weights;
    };

    Weights WeightsFromSums(int _Topspeed, int _Acceleration, int _Handling);
    Weights WeightsFromTuning(const CTuning* _Tuning);

    Result CalculateFromSums(const CCar* _Car, int _Topspeed, int _Acceleration, int _Handling);
    Result Calculate(const CCar* _Car, const CTuning* _Tuning);
    Result Calculate(const CCar* _Car, const CPart* _Engine, const CPart* _Induction,
            const CPart* _Transmission, const CPart* _Suspension, const CPart* _Brakes, const CPart* _Tires);
}

#endif
