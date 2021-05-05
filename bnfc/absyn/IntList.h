//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_INTLIST_H
#define BNFC_BNFC_ABSYN_INTLIST_H


#include "ListInteger.h"

struct IntList {
    virtual ~IntList() = default;

    virtual void accept(struct IntListVisitor* v) = 0;
};

struct IntListListInteger : public IntList {
    const ListInteger* const _v1;
    explicit IntListListInteger (const ListInteger* const v1) : _v1(v1) {}

    void accept(IntListVisitor* v) override;
};

struct IntListVisitor {
    virtual void visit(IntListListInteger* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_INTLIST_H
