//
// Created by Anton on 2020-07-18.
//

#include "ListItem.h"

void ListItemEpsilon::accept(ListItemVisitor* v) {
    v->visit(this);
}

void ListItemItemList::accept(ListItemVisitor* v) {
    v->visit(this);
}
