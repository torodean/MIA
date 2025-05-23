#ifndef MAGE_H
#define MAGE_H
#define WINVER 0x0500
#include <windows.h>
#include "QString"

class Mage
{
private:
    QString lastSpellCast;
    double GCD, globalTimer, frostBoltCast, frostFireBoltCast, iceNovaTimer, frozenOrbTimer, iceNovaCD, frozenOrbCD, icyVeinsCD, icyVeinsTimer, mirrorImageCD, mirrorImageTimer;
    bool iceNovaOnCD, frozenOrbOnCD, icyVeinsOnCD, mirrorImageOnCD, openingRotationRun, normalRotationRun;

public:
    Mage();
    void castFrostBolt();
    void castIceLance();
    void castFrostfireBolt();
    void castFrozenOrb();
    void castIceNova();
    void castMirrorImage();
    void castIcyVeins();
    void setLastSpellCast(QString spell);
    QString getLastSpellCast();
    void waitGCD();
    void openingRotation();
    void normalRotation();
    void addTime(double time);
    void setOpeningRotationRun(bool x);
    void setNormalRotationRun(bool x);
    void tab();
    void right();
    void left();
    void petguru();
    void leftclick();
};

#endif // MAGE_H
