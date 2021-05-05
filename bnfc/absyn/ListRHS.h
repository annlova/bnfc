//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTRHS_H
#define BNFC_BNFC_ABSYN_LISTRHS_H

#include "RHS.h"

struct ListRHS {
    virtual ~ListRHS() = default;

    virtual void accept(struct ListRHSVisitor* v) = 0;
};

struct ListRHSRHS : public ListRHS {
    const RHS* const _v1;
    explicit ListRHSRHS (const RHS* const v1) : _v1(v1) {}

    void accept(ListRHSVisitor* v) override;
};

struct ListRHSList : public ListRHS {
    const RHS* const _v1;
    const ListRHS* const _v2;
    ListRHSList (const RHS* const v1, const ListRHS* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListRHSVisitor* v) override;
};

struct ListRHSVisitor {
    virtual void visit(ListRHSRHS* token) = 0;
    virtual void visit(ListRHSList* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LISTRHS_H
