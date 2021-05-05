//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_LABELID_H
#define BNFC_BNFC_ABSYN_LABELID_H

struct LabelId {
    virtual ~LabelId() = default;

    virtual void accept(struct LabelIdVisitor* v) = 0;
};

struct LabelIdIdent : public LabelId {
    const char* const _id;
    explicit LabelIdIdent (const char* id) : _id(id) {}

    void accept(LabelIdVisitor* v) override;
};

struct LabelIdUnderscore : public LabelId {
    LabelIdUnderscore () = default;

    void accept(LabelIdVisitor* v) override;
};

struct LabelIdSquareBrackets : public LabelId {
    LabelIdSquareBrackets () = default;

    void accept(LabelIdVisitor* v) override;
};

struct LabelIdBracketsColon : public LabelId {
    LabelIdBracketsColon () = default;

    void accept(LabelIdVisitor* v) override;
};

struct LabelIdBracketsColonSquare : public LabelId {
    LabelIdBracketsColonSquare  () = default;

    void accept(LabelIdVisitor* v) override;
};

struct LabelIdVisitor {
    virtual void visit(LabelIdIdent* token) = 0;
    virtual void visit(LabelIdUnderscore* token) = 0;
    virtual void visit(LabelIdSquareBrackets* token) = 0;
    virtual void visit(LabelIdBracketsColon* token) = 0;
    virtual void visit(LabelIdBracketsColonSquare* token) = 0;
};

#endif //BNFC_BNFC_ABSYN_LABELID_H
