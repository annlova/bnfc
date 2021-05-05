//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTINTEGER_H
#define BNFC_BNFC_ABSYN_LISTINTEGER_H

struct ListInteger {
    virtual ~ListInteger() = default;

    virtual void accept(struct ListIntegerVisitor* v) = 0;
};

struct ListIntegerEmpty : public ListInteger {
    ListIntegerEmpty () = default;

    void accept(ListIntegerVisitor* v) override;
};

struct ListIntegerInteger : public ListInteger {
    const int _v1;
    explicit ListIntegerInteger (int v) : _v1(v) {}

    void accept(ListIntegerVisitor* v) override;
};

struct ListIntegerIntegerList : public ListInteger {
    const int _v1;
    const ListInteger* const _v2;
    ListIntegerIntegerList (int v1, const ListInteger* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListIntegerVisitor* v) override;
};

struct ListIntegerVisitor {
    virtual void visit(ListIntegerEmpty* token) = 0;
    virtual void visit(ListIntegerInteger* token) = 0;
    virtual void visit(ListIntegerIntegerList* token) = 0;
};
#endif //BNFC_BNFC_ABSYN_LISTINTEGER_H
