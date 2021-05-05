//
// Created by Anton on 2020-07-18.
//

#include "ListRHS.h"

void ListRHSRHS::accept(ListRHSVisitor* v) {
    v->visit(this);
}

void ListRHSList::accept(ListRHSVisitor* v) {
    v->visit(this);
}
