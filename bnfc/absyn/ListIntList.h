//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTINTLIST_H
#define BNFC_BNFC_ABSYN_LISTINTLIST_H

#include "IntList.h"

struct ListIntList {
    virtual ~ListIntList() = default;

    virtual void accept(struct ListIntListVisitor* v) = 0;
};

struct ListIntListEpsilon : public ListIntList {
    explicit ListIntListEpsilon () = default;

    void accept(ListIntListVisitor* v) override;
};

struct ListIntListIntList : public ListIntList {
    const IntList* const _v1;
    explicit ListIntListIntList (const IntList* const v1) : _v1(v1) {}

    void accept(ListIntListVisitor* v) override;
};

struct ListIntListIntListList : public ListIntList {
    const IntList* const _v1;
    const ListIntList* const _v2;
    explicit ListIntListIntListList (const IntList* const v1, const ListIntList* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListIntListVisitor* v) override;
};

struct ListIntListVisitor {
    virtual void visit(ListIntListEpsilon* token) = 0;
    virtual void visit(ListIntListIntList* token) = 0;
    virtual void visit(ListIntListIntListList* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LISTINTLIST_H
