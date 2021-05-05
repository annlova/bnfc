//
// Created by Anton on 2020-07-21.
//

#include "Dud.h"

void Def1::accept(struct DefVisitor* v) {
    v->visit(this);
}

void Def2::accept(struct DefVisitor* v) {
    v->visit(this);
}




