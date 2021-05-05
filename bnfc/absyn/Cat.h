//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_CAT_H
#define BNFC_BNFC_ABSYN_CAT_H


struct Cat {
    virtual ~Cat() = default;

    virtual void accept(struct CatVisitor* v) = 0;
};

struct CatSquare : public Cat {
    const Cat* const _v1;
    explicit CatSquare (const Cat* const v1) : _v1(v1) {}

    void accept(CatVisitor* v) override;
};

struct CatIdent : public Cat {
    const char* const _v1;
    explicit CatIdent (const char* const v1) : _v1(v1) {}

    void accept(CatVisitor* v) override;
};

struct CatVisitor {
    virtual void visit(CatSquare* token) = 0;
    virtual void visit(CatIdent* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_CAT_H
