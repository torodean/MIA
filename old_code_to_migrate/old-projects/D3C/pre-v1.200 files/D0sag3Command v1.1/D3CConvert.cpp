/*
 * D3CConvert.cpp
 *
 *  Created on: Oct 12, 2014
 *      Author: antonius
 */

#include <iostream>
#include <math.h>
#include "D3CConvert.h"

namespace std {

D3CConvert::D3CConvert() : conversionCase (0), value (0), amu(0), carat(0), ug(0), t(0), shortcwt(0), st(0), dram(0), mg(0), g(0), kg(0), ST(0), LT(0), MT(0), longcwt(0), shortqtr(0), longqtr(0), lb(0), oz(0), gr(0), troypound(0), ozt(0), dwt(0){
    // TODO Auto-generated constructor stub
}

void D3CConvert::massConversion(double unconvertedMass, string unconvertedMassUnit){
    if(unconvertedMassUnit == "g"){
        g = unconvertedMass;
    }else if(unconvertedMassUnit == "amu"){
        g = unconvertedMass*1.66053892 * pow(10, -24);
    }else if(unconvertedMassUnit == "carat"){
        g = unconvertedMass * 5;
    } else if (unconvertedMassUnit == "ug" || unconvertedMassUnit == "μg") {
        g = unconvertedMass * pow(10, -6);
    } else if (unconvertedMassUnit == "mg") {
        g = unconvertedMass * pow(10, -3);
    } else if (unconvertedMassUnit == "kg") {
        g = unconvertedMass * pow(10, 3);
    } else if (unconvertedMassUnit == "t") {
        g = unconvertedMass * pow(10, 6);
    } else if (unconvertedMassUnit == "ST") {
        g = unconvertedMass * 907185;
    } else if (unconvertedMassUnit == "LT") {
        g = unconvertedMass * 1.01605 * pow(10, 6);
    } else if (unconvertedMassUnit == "MT") {
        g = unconvertedMass * pow(10, 12);
    } else if (unconvertedMassUnit == "scwt") {
        g = unconvertedMass * 45359;
    } else if (unconvertedMassUnit == "lcwt") {
        g = unconvertedMass * 50802;
    } else if(unconvertedMassUnit == "lqtr"){
        g = unconvertedMass * 254000;
    } else if (unconvertedMassUnit == "st") {
        g = unconvertedMass * 6350.29;
    } else if (unconvertedMassUnit == "lb") {
        g = unconvertedMass * 453.592;
    } else if (unconvertedMassUnit == "oz") {
        g = unconvertedMass * 28.3495;
    } else if (unconvertedMassUnit == "dram") {
        g = unconvertedMass * 1.7718452;
    } else if (unconvertedMassUnit == "gr") {
        g = unconvertedMass * 0.064799;
    } else if (unconvertedMassUnit == "tlb") {
        g = unconvertedMass * 373.24;
    } else if (unconvertedMassUnit == "toz") {
        g = unconvertedMass * 31.1034768;
    } else if (unconvertedMassUnit == "dwt") {
        g = unconvertedMass * 1.5550;
    }
    kg = g/pow(10, 3);
    mg = g/pow(10, -3);
    ug = g/pow(10, -6);
    oz = g/28.3495;
    lb = g/453.592;
    amu = g/(1.66053892 * pow(10, -24));
    carat = g/5;
    dwt = g/1.5550;
    ozt = g/31.1034768;
    troypound = g/373.24;
    gr = g/0.064799;
    dram = g/1.7718452;
    st = g/6350.29;
    longqtr = g/254000;
    longcwt = g/50802;
    shortcwt = g/45359;
    MT = g/pow(10, 12);
    LT = g/(1.01605 * pow(10, 6));
    ST = g/907185;
    t = g/pow(10, 6);
}

double D3CConvert::getmassg(){
    return g;
}

double D3CConvert::getmasskg(){
    return kg;
}
double D3CConvert::getmassmg(){
    return mg;
}
double D3CConvert::getmassug(){
    return ug;
}
double D3CConvert::getmassoz(){
    return oz;
}
double D3CConvert::getmasslb(){
    return lb;
}
double D3CConvert::getmassamu(){
    return amu;
}
double D3CConvert::getmasscarat(){
    return carat;
}
double D3CConvert::getmassdwt(){
    return dwt;
}
double D3CConvert::getmassozt(){
    return ozt;
}
double D3CConvert::getmasstroypound(){
    return troypound;
}
double D3CConvert::getmassgr(){
    return gr;
}
double D3CConvert::getmassdram(){
    return dram;
}
double D3CConvert::getmassst(){
    return st;
}
double D3CConvert::getmasslongqtr(){
    return longqtr;
}
double D3CConvert::getmasslongcwt(){
    return longcwt;
}
double D3CConvert::getmassshortcwt(){
    return shortcwt;
}
double D3CConvert::getmassMT(){
    return MT;
}
double D3CConvert::getmassLT(){
    return LT;
}
double D3CConvert::getmassST(){
    return ST;
}
double D3CConvert::getmasst(){
    return t;
}



string D3CConvert::convertHelp(string units){
    string help;
    if(units == "help"){
        help = "~~ Mass Units ~~""\n"
        "-- amu (atomic mass units)""\n"
        "-- carat (carat)""\n"
               "-- μg/ug (microgram)""\n"
               "-- mg (milligram)""\n"
               "-- g(gram)""\n"
               "-- kg (kilogram)""\n"
        "-- tonne (tonne)""\n"
               "-- ST (short ton)""\n"
               "-- LT (long ton)""\n"
               "-- MT (mega ton)""\n"
        "-- short cwt (short hundredweight)""\n"
               "-- long cwt (long hundredweight)""\n"
               "-- long qtr (long quarter)""\n"
        "-- st (stone)""\n"
               "-- lb (pound)""\n"
               "-- oz (ounce)""\n"
        "-- dram (drachm)""\n"
               "-- gr (grain)""\n"
               "-- troy lb (troy pound)""\n"
               "-- troy oz (troy ounce)""\n"
               "-- dwt (pennyweight)""\n"
        "\n"
        "~~ Length Units ~~""\n"
        "-- Gm (gigametre""\n"
               "-- Mm (megametre)""\n"
               "-- km (kilometre)""\n"
               "-- hm (hectometre)""\n"
               "-- dam (decametre)""\n"
               "-- m (metre)""\n"
               "-- dm (decimetre) ""\n"
        "- cm (centimetre)""\n"
               "-- mm (millimetre)""\n"
               "-- μm/um (micrometre)""\n"
               "-- nm (nanometre)""\n"
        "-- Å/Ang (ångström)""\n"
        "-- furlong (furlong)""\n"
               "-- chain (chain)""\n"
               "-- rd (rod)""\n"
               "-- pole (pole)""\n"
               "-- perch (perch)""\n"
               "-- fathom (fathom)""\n"
        "-- mi (mile)""\n"
               "-- yd (yard)""\n"
               "-- ft (foot)""\n"
               "-- hand (hand)""\n"
               "-- in (inch)""\n"
               "-- nmi (nautical mile)""\n"
        "-- Gpc (gigaparsec)""\n"
               "-- Mpc (megaParsec)""\n"
               "-- kpc (kiloparsec)""\n"
               "-- pc (parsec)""\n"
               "-- Gly (gigalight-year)""\n"
               "-- Mly (megalight-year) ""\n"
            "- kly (kilolight-year)""\n"
               "-- ly (light-year)""\n"
               "-- AU (astronomical unit)""\n";

    }else if(units == "help-mass"){
        help = "~~ Mass Units ~~""\n"
        "-- amu (atomic mass units)""\n"
        "-- carat (carat)""\n"
        "-- μg/ug (microgram)""\n"
               "-- mg (milligram)""\n"
               "-- g(gram)""\n"
               "-- kg (kilogram)""\n"
        "-- t (tonne)""\n"
               "-- ST (short ton)""\n"
               "-- LT (long ton)""\n"
               "-- MT (mega ton)""\n"
        "-- short cwt (short hundredweight)""\n"
               "-- long cwt (long hundredweight)""\n"
               "-- long qtr (long quarter)""\n"
        "-- st (stone)""\n"
               "-- lb (pound)""\n"
               "-- oz (ounce)""\n"
        "-- dram (drachm)""\n"
               "-- gr (grain)""\n"
               "-- troy pound (troy pound)""\n"
               "-- ozt (troy ounce)""\n"
               "-- dwt (pennyweight)""\n";
    }
    return help;
}

D3CConvert::~D3CConvert() {
    // TODO Auto-generated destructor stub
}

} /* namespace std */
