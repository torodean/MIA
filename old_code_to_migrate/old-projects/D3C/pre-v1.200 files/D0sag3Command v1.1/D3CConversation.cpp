/*
 * D3CConversation.cpp
 *
 *  Created on: Oct 17, 2014
 *      Author: antonius
 */

#include <iostream>
#include "D3CConversation.h"

namespace std {

D3CConversation::D3CConversation() {
    // TODO Auto-generated constructor stub

}

bool D3CConversation::isTerm(string term){
    if(term == "hello" || term == "Hello" || term == "yo" || term == "Yo"
            || term == "idiot" || term == "Idiot"
            || term == "What's up?" || term == "what's up?" || term == "whats up? || term == Whats up?" || term == "whats up" || term == "what's up" || term == "Whats up" || term == "whats up"
            || term == "lol" || term == "LOL" || term == "Lol"
            || term == "Your mom." || term == "your mom" || term == "Your mom" || term == "your mom."){
        return true;
    }else{
        return false;
    }
}

string D3CConversation::reply(string term){
    if(term == "hello" || term == "Hello" || term == "yo" || term == "Yo"){
        return "Hello good sir.";
    }else if(term == "idiot" || term == "Idiot"){
        return "As a computational device, if I am an idiot what does that make you?";
    }else if(term == "What's up?" || term == "what's up?" || term == "whats up? || term == Whats up?" || term == "whats up" || term == "what's up" || term == "Whats up" || term == "whats up"){
        return "Many things are 'up', although if you are referring to what I am doing, than not much; just cogitating.";
    }else if(term == "lol" || term == "LOL" || term == "Lol"){
        return "What is so funny?";
    }else if(term == "Your mom." || term == "your mom" || term == "Your mom" || term == "your mom."){
        return "Your mom is so fat, I took a picture of her last Christmas and it's still printing.";
    }else{
        return NULL;
    }
}

D3CConversation::~D3CConversation() {
    // TODO Auto-generated destructor stub
}

} /* namespace std */
