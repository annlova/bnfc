//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTDEF_H
#define BNFC_BNFC_ABSYN_LISTDEF_H


#include "Def.h"

struct ListDef {
    virtual ~ListDef() = default;

    virtual void accept(struct ListDefVisitor* v) = 0;
};

struct ListDefEpsilon : public ListDef {
    explicit ListDefEpsilon () = default;

    void accept(ListDefVisitor* v) override;
};

struct ListDefList : public ListDef {
    const Def* const _v1;
    const ListDef* const _v2;
    explicit ListDefList (const Def* const v1, const ListDef* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListDefVisitor* v) override;
};

struct ListDefVisitor {
    virtual void visit(ListDefEpsilon* token) = 0;
    virtual void visit(ListDefList* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LISTDEF_H
