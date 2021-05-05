//
// Created by Anton on 2020-07-18.
//

#include "Label.h"

void LabelLabelId::accept(LabelVisitor* v) {
    v->visit(this);
}

void LabelLabelIdListProfItem::accept(LabelVisitor* v) {
    v->visit(this);
}

void LabelDoubleLabelIdListProfItem::accept(LabelVisitor* v) {
    v->visit(this);
}
