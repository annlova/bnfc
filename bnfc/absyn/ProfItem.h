//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_PROFITEM_H
#define BNFC_BNFC_ABSYN_PROFITEM_H

#include "ListIntList.h"
#include "ListInteger.h"

struct ProfItem {
    virtual ~ProfItem() = default;

    virtual void accept(struct ProfItemVisitor* v) = 0;
};

struct ProfItemBrackets : public ProfItem {
    const ListIntList* const _v1;
    const ListInteger* const _v2;
    explicit ProfItemBrackets (const ListIntList* const v1, const ListInteger* const v2) : _v1(v1), _v2(v2) {}

    void accept(ProfItemVisitor* v) override;
};

struct ProfItemVisitor {
    virtual void visit(ProfItemBrackets* token) = 0;
};

#endif //BNFC_BNFC_ABSYN_PROFITEM_H
