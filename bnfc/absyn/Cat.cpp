//
// Created by Anton on 2020-07-18.
//

#include "Cat.h"

void CatSquare::accept(CatVisitor* v) {
    v->visit(this);
}

void CatIdent::accept(CatVisitor* v) {
    v->visit(this);
}
