//
// Created by Anton on 2020-07-18.
//

#include "ListProfItem.h"

void ListProfItemProfItem::accept(ListProfItemVisitor* v) {
    v->visit(this);
}

void ListProfItemList::accept(ListProfItemVisitor* v) {
    v->visit(this);
}
