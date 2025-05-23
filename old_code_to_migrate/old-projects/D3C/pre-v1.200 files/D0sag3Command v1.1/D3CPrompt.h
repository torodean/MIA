/*
 * D3CPrompt.h
 *
 *  Created on: Oct 5, 2014
 *      Author: Antonius Torode
 */

#ifndef D3CPrompt_H_
#define D3CPrompt_H_

#include <string>

namespace std {

class D3CPrompt {
public:
    D3CPrompt();
    int caseDecider(string function);
    virtual ~D3CPrompt();
};

} /* namespace std */

#endif /* D3CPrompt_H_ */
