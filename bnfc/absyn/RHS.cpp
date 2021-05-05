//
// Created by Anton on 2020-07-18.
//

#include "RHS.h"

void RHSListItem::accept(RHSVisitor* v) {
    v->visit(this);
}
