//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LISTPROFITEM_H
#define BNFC_BNFC_ABSYN_LISTPROFITEM_H

#include "ProfItem.h"

struct ListProfItem {
    virtual ~ListProfItem() = default;

    virtual void accept(struct ListProfItemVisitor* v) = 0;
};

struct ListProfItemProfItem : public ListProfItem {
    const ProfItem* const _v1;
    explicit ListProfItemProfItem (const ProfItem* const v1) : _v1(v1) {}

    void accept(ListProfItemVisitor* v) override;
};

struct ListProfItemList : public ListProfItem {
    const ProfItem* const _v1;
    const ListProfItem* const _v2;
    ListProfItemList (const ProfItem* const v1, const ListProfItem* const v2) : _v1(v1), _v2(v2) {}

    void accept(ListProfItemVisitor* v) override;
};

struct ListProfItemVisitor {
    virtual void visit(ListProfItemProfItem* token) = 0;
    virtual void visit(ListProfItemList* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LISTPROFITEM_H
