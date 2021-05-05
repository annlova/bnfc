//
// Created by Anton on 2020-07-18.
//

#include "ListInteger.h"

void ListIntegerEmpty::accept(ListIntegerVisitor* v) {
    v->visit(this);
}

void ListIntegerInteger::accept(ListIntegerVisitor* v) {
    v->visit(this);
}

void ListIntegerIntegerList::accept(ListIntegerVisitor* v) {
    v->visit(this);
}
