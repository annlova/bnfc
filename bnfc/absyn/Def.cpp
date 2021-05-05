//
// Created by Anton on 2020-07-18.
//

#include "Def.h"

void DefLabel::accept(DefVisitor* v) {
    v->visit(this);
}

void DefLineComment::accept(DefVisitor* v) {
    v->visit(this);
}

void DefBlockComment::accept(DefVisitor* v) {
    v->visit(this);
}

void DefInternal::accept(DefVisitor* v) {
    v->visit(this);
}

void DefToken::accept(DefVisitor* v) {
    v->visit(this);
}

void DefPositionToken::accept(DefVisitor* v) {
    v->visit(this);
}

void DefEntryPoints::accept(DefVisitor* v) {
    v->visit(this);
}

void DefSeparator::accept(DefVisitor* v) {
    v->visit(this);
}

void DefTerminator::accept(DefVisitor* v) {
    v->visit(this);
}

void DefCoercions::accept(DefVisitor* v) {
    v->visit(this);
}

void DefRules::accept(DefVisitor* v) {
    v->visit(this);
}

void DefLayout::accept(DefVisitor* v) {
    v->visit(this);
}

void DefLayoutStop::accept(DefVisitor* v) {
    v->visit(this);
}

void DefLayoutTopLevel::accept(DefVisitor* v) {
    v->visit(this);
}


