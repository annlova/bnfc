//
// Created by Anton on 2021-05-07.
//

#include "../h/Token.h"

#include <cassert>

bool Parser::Token::operator==(const Token& other) const {
    bool equal = id == other.id;
    assert(!equal || equal && terminal == other.terminal);
    return equal;
}


bool Parser::Token::operator!=(const Parser::Token& other) const {
    bool unequal = id != other.id;
    assert(unequal || !unequal && terminal == other.terminal);
    return unequal;
}
