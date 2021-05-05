//
// Created by Anton on 2020-07-18.
//

#include "IntList.h"

void IntListListInteger::accept(IntListVisitor* v) {
    v->visit(this);
}
