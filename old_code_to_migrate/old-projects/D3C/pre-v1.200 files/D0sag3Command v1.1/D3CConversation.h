/*
 * D3CConversation.h
 *
 *  Created on: Oct 17, 2014
 *      Author: antonius
 */

#include <string>
#ifndef D3CConversation_H_
#define D3CConversation_H_

namespace std {

class D3CConversation {
public:
    D3CConversation();
    bool isTerm(string term);
    string reply(string term);
    virtual ~D3CConversation();
};

} /* namespace std */

#endif /* D3CConversation_H_ */
