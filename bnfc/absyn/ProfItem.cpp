//
// Created by Anton on 2020-07-18.
//

#include "ProfItem.h"

void ProfItemBrackets::accept(ProfItemVisitor* v) {
    v->visit(this);
}
