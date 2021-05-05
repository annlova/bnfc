//
// Created by Anton on 2020-07-18.
//

#include "Grammar.h"


void GrammarListDef::accept(GrammarVisitor* v) {
    v->visit(this);
}
