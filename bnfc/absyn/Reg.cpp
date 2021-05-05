//
// Created by Anton on 2020-07-18.
//

#include "Reg.h"

void RegReg::accept(RegVisitor* v) {
    v->visit(this);
}

void RegChar::accept(RegVisitor* v) {
    v->visit(this);
}

void RegLower::accept(RegVisitor* v) {
    v->visit(this);
}

void RegUpper::accept(RegVisitor* v) {
    v->visit(this);
}

void RegLetter::accept(RegVisitor* v) {
    v->visit(this);
}

void RegDigit::accept(RegVisitor* v) {
    v->visit(this);
}

void RegSquareString::accept(RegVisitor* v) {
    v->visit(this);
}

void RegCurlyString::accept(RegVisitor* v) {
    v->visit(this);
}

void RegEps::accept(RegVisitor* v) {
    v->visit(this);
}

void RegStar::accept(RegVisitor* v) {
    v->visit(this);
}

void RegPlus::accept(RegVisitor* v) {
    v->visit(this);
}

void RegQuestion::accept(RegVisitor* v) {
    v->visit(this);
}

void RegMinus::accept(RegVisitor* v) {
    v->visit(this);
}

void RegOr::accept(RegVisitor* v) {
    v->visit(this);
}

void RegDoubleReg::accept(RegVisitor* v) {
    v->visit(this);
}

void RegRealChar::accept(RegVisitor* v) {
    v->visit(this);
}
