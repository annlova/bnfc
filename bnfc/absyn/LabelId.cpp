//
// Created by Anton on 2020-07-18.
//

#include "LabelId.h"

void LabelIdIdent::accept(struct LabelIdVisitor* v) {
    v->visit(this);
}

void LabelIdUnderscore::accept(LabelIdVisitor* v) {
    v->visit(this);
}

void LabelIdSquareBrackets::accept(struct LabelIdVisitor* v) {
    v->visit(this);
}

void LabelIdBracketsColon::accept(struct LabelIdVisitor* v) {
    v->visit(this);
}

void LabelIdBracketsColonSquare::accept(LabelIdVisitor* v) {
    v->visit(this);
}
