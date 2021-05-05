//
// Created by Anton on 2020-07-18.
//

#include "Item.h"

void ItemString::accept(ItemVisitor* v) {
    v->visit(this);
}

void ItemCat::accept(ItemVisitor* v) {
    v->visit(this);
}
