//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_GRAMMAR_H
#define BNFC_BNFC_ABSYN_GRAMMAR_H

#include "ListDef.h"

struct Grammar {
    virtual ~Grammar() = default;

    virtual void accept(struct GrammarVisitor* v) = 0;
};

struct GrammarListDef : public Grammar {
    const ListDef* const _v1;
    explicit GrammarListDef (const ListDef* const v1) : _v1(v1) {}

    void accept(GrammarVisitor* v) override;
};

struct GrammarVisitor {
    virtual void visit(GrammarListDef* token) = 0;
};

#endif //BNFC_BNFC_ABSYN_GRAMMAR_H
