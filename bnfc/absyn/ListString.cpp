//
// Created by Anton on 2020-07-18.
//

#include "ListString.h"

void ListStringString::accept(ListStringVisitor* v) {
    v->visit(this);
}

void ListStringList::accept(ListStringVisitor* v) {
    v->visit(this);
}
