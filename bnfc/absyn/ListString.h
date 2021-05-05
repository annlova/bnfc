//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTSTRING_H
#define BNFC_BNFC_ABSYN_LISTSTRING_H

struct ListString {
    virtual ~ListString() = default;

    virtual void accept(struct ListStringVisitor* v) = 0;
};

struct ListStringString : public ListString {
    const char* const _v1;
    explicit ListStringString (const char* const v1) : _v1(v1) {}

    void accept(ListStringVisitor* v) override;
};

struct ListStringList : public ListString {
    const char* const _v1;
    const ListString* const _v2;
    ListStringList (const char* const v1, const ListString* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListStringVisitor* v) override;
};

struct ListStringVisitor {
    virtual void visit(ListStringString* token) = 0;
    virtual void visit(ListStringList* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LISTSTRING_H
