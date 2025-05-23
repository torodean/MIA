/*
 * D3CConvert.h
 *
 *  Created on: Oct 12, 2014
 *      Author: antonius
 */

#include <string>
#ifndef D3CConvert_H_
#define D3CConvert_H_

namespace std {

class D3CConvert {
    int conversionCase;
    string output;
    double value, amu, carat, ug, t, shortcwt, st, dram, mg, g, kg, ST, LT, MT, longcwt, shortqtr, longqtr, lb, oz, gr, troypound, ozt, dwt;

public:
    D3CConvert();
    void massConversion(double unconvertedMass, string unconvertedMassUnit);
    string convertHelp(string Units);
    double getmasskg();
    double getmassmg();
    double getmassug();
    double getmassoz();
    double getmasslb();
    double getmassamu();
    double getmasscarat();
    double getmassdwt();
    double getmassozt();
    double getmasstroypound();
    double getmassgr();
    double getmassdram();
    double getmassst();
    double getmasslongqtr();
    double getmasslongcwt();
    double getmassshortcwt();
    double getmassMT();
    double getmassLT();
    double getmassST();
    double getmasst();
    double getmassg();
    virtual ~D3CConvert();
};

} /* namespace std */

#endif /* D3CConvert_H_ */
