//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_ITEM_H
#define BNFC_BNFC_ABSYN_ITEM_H

#include "Cat.h"

struct Item {
    virtual ~Item() = default;

    virtual void accept(struct ItemVisitor* v) = 0;
};

struct ItemString : public Item {
    const char* const _v1;
    explicit ItemString (const char* const v1) : _v1(v1) {}

    void accept(ItemVisitor* v) override;
};

struct ItemCat : public Item {
    const Cat* const _v1;
    explicit ItemCat (const Cat* const v1) : _v1(v1) {}

    void accept(ItemVisitor* v) override;
};

struct ItemVisitor {
    virtual void visit(ItemString* token) = 0;
    virtual void visit(ItemCat* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_ITEM_H
