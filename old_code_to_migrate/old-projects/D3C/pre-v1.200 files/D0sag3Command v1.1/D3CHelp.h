/*
 * D3CHelp.h
 *
 *  Created on: Oct 10, 2014
 *      Author: antonius
 */

#ifndef D3CHelp_H_
#define D3CHelp_H_

#include <string>

namespace std {

class D3CHelp {
public:
    D3CHelp();
    string helpRunner();
    string helpRunnerExtended();
    virtual ~D3CHelp();
};

} /* namespace std */

#endif /* D3CHelp_H_ */
