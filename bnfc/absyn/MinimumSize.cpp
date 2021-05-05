//
// Created by Anton on 2020-07-18.
//

#include "MinimumSize.h"

void MinimumSizeNonempty::accept(MinimumSizeVisitor* v) {
    v->visit(this);
}

void MinimumSizeEpsilon::accept(MinimumSizeVisitor* v) {
    v->visit(this);
}
