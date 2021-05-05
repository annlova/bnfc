//
// Created by Anton on 2020-07-18.
//

#include "ListIntList.h"

void ListIntListEpsilon::accept(ListIntListVisitor* v) {
    v->visit(this);
}

void ListIntListIntList::accept(ListIntListVisitor* v) {
    v->visit(this);
}

void ListIntListIntListList::accept(ListIntListVisitor* v) {
    v->visit(this);
}
