//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTIDENT_H
#define BNFC_BNFC_ABSYN_LISTIDENT_H


struct ListIdent {
    virtual ~ListIdent() = default;

    virtual void accept(struct ListIdentVisitor* v) = 0;
};

struct ListIdentIdent : public ListIdent {
    const char* const _v1;
    explicit ListIdentIdent (const char* const v1) : _v1(v1) {}

    void accept(ListIdentVisitor* v) override;
};

struct ListIdentIdentList : public ListIdent {
    const char* const _v1;
    const ListIdent* const _v2;
    explicit ListIdentIdentList (const char* const v1, const ListIdent* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListIdentVisitor* v) override;
};

struct ListIdentVisitor {
    virtual void visit(ListIdentIdent* token) = 0;
    virtual void visit(ListIdentIdentList* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LISTIDENT_H
