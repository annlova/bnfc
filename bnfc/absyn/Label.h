//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LABEL_H
#define BNFC_BNFC_ABSYN_LABEL_H

#include "LabelId.h"
#include "ListProfItem.h"

struct Label {
    virtual ~Label() = default;

    virtual void accept(struct LabelVisitor* v) = 0;
};

struct LabelLabelId : public Label {
    const LabelId* const _v1;
    explicit LabelLabelId (const LabelId* const v1) : _v1(v1) {}

    void accept(LabelVisitor* v) override;
};

struct LabelLabelIdListProfItem : public Label {
    const LabelId* const _v1;
    const ListProfItem* const _v2;
    LabelLabelIdListProfItem (const LabelId* const v1, const ListProfItem* const v2) : _v1(v1), _v2(v2) {}

    void accept(LabelVisitor* v) override;
};

struct LabelDoubleLabelIdListProfItem : public Label {
    const LabelId* const _v1;
    const LabelId* const _v2;
    const ListProfItem* const _v3;
    LabelDoubleLabelIdListProfItem (const LabelId* const v1, const LabelId* const v2, const ListProfItem* const v3) : _v1(v1), _v2(v2), _v3(v3) {}

    void accept(LabelVisitor* v) override;
};

struct LabelVisitor {
    virtual void visit(LabelLabelId* token) = 0;
    virtual void visit(LabelLabelIdListProfItem* token) = 0;
    virtual void visit(LabelDoubleLabelIdListProfItem* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_LABEL_H
