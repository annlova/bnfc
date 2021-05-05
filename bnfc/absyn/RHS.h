//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_RHS_H
#define BNFC_BNFC_ABSYN_RHS_H

#include "ListItem.h"

struct RHS {
    virtual ~RHS() = default;

    virtual void accept(struct RHSVisitor* v) = 0;
};

struct RHSListItem : public RHS {
    const ListItem* const _v1;
    explicit RHSListItem (const ListItem* const v1) : _v1(v1) {}

    void accept(RHSVisitor* v) override;
};

struct RHSVisitor {
    virtual void visit(RHSListItem* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_RHS_H
