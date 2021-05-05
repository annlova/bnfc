//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTITEM_H
#define BNFC_BNFC_ABSYN_LISTITEM_H

#include "Item.h"

struct ListItem {
    virtual ~ListItem() = default;

    virtual void accept(struct ListItemVisitor* v) = 0;
};

struct ListItemEpsilon : public ListItem {
    explicit ListItemEpsilon () = default;

    void accept(ListItemVisitor* v) override;
};

struct ListItemItemList : public ListItem {
    const Item* const _v1;
    const ListItem* const _v2;
    explicit ListItemItemList (const Item* const v1, const ListItem* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListItemVisitor* v) override;
};

struct ListItemVisitor {
    virtual void visit(ListItemEpsilon* token) = 0;
    virtual void visit(ListItemItemList* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LISTITEM_H
