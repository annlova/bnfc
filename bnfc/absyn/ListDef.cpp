//
// Created by Anton on 2020-07-18.
//

#include "ListDef.h"

void ListDefEpsilon::accept(ListDefVisitor* v) {
    v->visit(this);
}

void ListDefList::accept(ListDefVisitor* v) {
    v->visit(this);
}
