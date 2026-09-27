#include "GameMath.h"

#include "CCar.h"
#include "CTuning.h"
#include "CPart.h"

namespace
{
    const float kOneHundredth = 0.01f;
    // float32 constant used by the recovered NFSW calculation (2/3).
    const float kTwoThirds = 0.6666666865348816f;

    GameMath::Weights Normalize(float h, float a, float t)
    {
        float total = t + a;
        total += h;
        float denom = total * kTwoThirds;
        denom += 1.0f;
        const float fragment = 1.0f / denom;

        GameMath::Weights w;
        w.handling = h * fragment;
        w.acceleration = a * fragment;
        w.topspeed = t * fragment;
        w.stock = 1.0f - w.handling;
        w.stock -= w.acceleration;
        w.stock -= w.topspeed;
        return w;
    }

    float Blend(const CCar* car, const EAttribute attr, const GameMath::Weights& w)
    {
        float value = car->NFSWBase(attr, 0) * w.stock;
        value += car->NFSWBase(attr, 1) * w.handling;
        value += car->NFSWBase(attr, 2) * w.acceleration;
        value += car->NFSWBase(attr, 3) * w.topspeed;
        return value;
    }

    tValue Display(const float v)
    {
        // NFSW uses CVTTSS2SI: truncate toward zero.
        if( v <= 0.0f )
            return 0;
        if( v >= 65535.0f )
            return 65535;
        return (tValue)((int)v);
    }

    GameMath::Result OldFallback(const CCar* car, int t, int a, int h)
    {
        GameMath::Result r;
        r.weights = GameMath::WeightsFromSums(t, a, h);
        const float maxT = 700.0f;
        r.rawTopspeed = ( maxT - car->Topspeed() ) * t / 100.0f + car->Topspeed();
        r.rawAcceleration = ( maxT - car->Acceleration() ) * a / 100.0f + car->Acceleration();
        r.rawHandling = ( maxT - car->Handling() ) * h / 100.0f + car->Handling();
        r.topspeed = Display(r.rawTopspeed);
        r.acceleration = Display(r.rawAcceleration);
        r.handling = Display(r.rawHandling);
        return r;
    }
}

GameMath::Weights GameMath::WeightsFromSums(int _Topspeed, int _Acceleration, int _Handling)
{
    const float h = _Handling * kOneHundredth;
    const float a = _Acceleration * kOneHundredth;
    const float t = _Topspeed * kOneHundredth;
    return Normalize(h, a, t);
}

GameMath::Weights GameMath::WeightsFromTuning(const CTuning* _Tuning)
{
    float h = 0.0f, a = 0.0f, t = 0.0f;
    if( !_Tuning )
        return Normalize(h, a, t);

    for(IType i = ITypeBegin; i <= ITypeEnd; ++i)
    {
        const CPart* p = _Tuning->Part((EType)i);
        if( !p )
            continue;
        // Deliberately accumulate six float32 contributions sequentially, as the game does.
        h += p->Handling() * kOneHundredth;
        a += p->Acceleration() * kOneHundredth;
        t += p->Topspeed() * kOneHundredth;
    }
    return Normalize(h, a, t);
}

GameMath::Result GameMath::CalculateFromSums(const CCar* _Car, int _Topspeed, int _Acceleration, int _Handling)
{
    if( !_Car )
    {
        Result empty = { 0, 0, 0, 0, 0, 0, WeightsFromSums(0, 0, 0) };
        return empty;
    }

    if( !_Car->HasNFSWPerformanceModel() )
        return OldFallback(_Car, _Topspeed, _Acceleration, _Handling);

    Result r;
    r.weights = WeightsFromSums(_Topspeed, _Acceleration, _Handling);
    r.rawTopspeed = Blend(_Car, aTOPSPEED, r.weights);
    r.rawAcceleration = Blend(_Car, aACCELERATION, r.weights);
    r.rawHandling = Blend(_Car, aHANDLING, r.weights);
    r.topspeed = Display(r.rawTopspeed);
    r.acceleration = Display(r.rawAcceleration);
    r.handling = Display(r.rawHandling);
    return r;
}

GameMath::Result GameMath::Calculate(const CCar* _Car, const CTuning* _Tuning)
{
    if( !_Car )
    {
        Result empty = { 0, 0, 0, 0, 0, 0, WeightsFromSums(0, 0, 0) };
        return empty;
    }

    if( !_Tuning )
        return CalculateFromSums(_Car, 0, 0, 0);

    if( !_Car->HasNFSWPerformanceModel() )
        return OldFallback(_Car, _Tuning->Topspeed(), _Tuning->Acceleration(), _Tuning->Handling());

    Result r;
    r.weights = WeightsFromTuning(_Tuning);
    r.rawTopspeed = Blend(_Car, aTOPSPEED, r.weights);
    r.rawAcceleration = Blend(_Car, aACCELERATION, r.weights);
    r.rawHandling = Blend(_Car, aHANDLING, r.weights);
    r.topspeed = Display(r.rawTopspeed);
    r.acceleration = Display(r.rawAcceleration);
    r.handling = Display(r.rawHandling);
    return r;
}

GameMath::Result GameMath::Calculate(const CCar* _Car, const CPart* _Engine, const CPart* _Induction,
        const CPart* _Transmission, const CPart* _Suspension, const CPart* _Brakes, const CPart* _Tires)
{
    CTuning tuning(_Engine, _Induction, _Transmission, _Suspension, _Brakes, _Tires);
    return Calculate(_Car, &tuning);
}
