//
// Created by Anton on 2020-07-18.
//

#include "ListIdent.h"

void ListIdentIdent::accept(ListIdentVisitor* v) {
    v->visit(this);
}

void ListIdentIdentList::accept(ListIdentVisitor* v) {
    v->visit(this);
}
