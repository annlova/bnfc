//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_DEF_H
#define BNFC_BNFC_ABSYN_DEF_H


#include "Label.h"
#include "Cat.h"
#include "ListItem.h"
#include "Reg.h"
#include "ListIdent.h"
#include "MinimumSize.h"
#include "ListRHS.h"
#include "ListString.h"

struct Def {
    virtual ~Def() = default;

    virtual void accept(struct DefVisitor* v) = 0;
};

struct DefLabel : public Def {
    const Label* const _v1;
    const Cat* const _v2;
    const ListItem* const _v3;
    DefLabel (const Label* const v1, const Cat* const v2, const ListItem* const v3) : _v1(v1), _v2(v2), _v3(v3) {}

    void accept(DefVisitor* v) override;
};

struct DefLineComment : public Def {
    const char* const _v1;
    explicit DefLineComment (const char* const v1) : _v1(v1) {}

    void accept(DefVisitor* v) override;
};

struct DefBlockComment : public Def {
    const char* const _v1;
    const char* const _v2;
    DefBlockComment (const char* const v1, const char* const v2) : _v1(v1), _v2(v2) {}

    void accept(DefVisitor* v) override;
};

struct DefInternal : public Def {
    const Label* const _v1;
    const Cat* const _v2;
    const ListItem* const _v3;
    DefInternal (const Label* const v1, const Cat* const v2, const ListItem* const v3) : _v1(v1), _v2(v2), _v3(v3) {}

    void accept(DefVisitor* v) override;
};

struct DefToken : public Def {
    const char* const _v1;
    const Reg* const _v2;
    DefToken (const char* const v1, const Reg* const v2) : _v1(v1), _v2(v2) {}

    void accept(DefVisitor* v) override;
};

struct DefPositionToken : public Def {
    const char* const _v1;
    const Reg* const _v2;
    DefPositionToken (const char* const v1, const Reg* const v2) : _v1(v1), _v2(v2) {}

    void accept(DefVisitor* v) override;
};

struct DefEntryPoints : public Def {
    const ListIdent* const _v1;
    explicit DefEntryPoints (const ListIdent* const v1) : _v1(v1) {}

    void accept(DefVisitor* v) override;
};

struct DefSeparator : public Def {
    const MinimumSize* const _v1;
    const Cat* const _v2;
    const char* const _v3;
    DefSeparator (const MinimumSize* const v1, const Cat* const v2, const char* const v3) : _v1(v1), _v2(v2), _v3(v3) {}

    void accept(DefVisitor* v) override;
};

struct DefTerminator : public Def {
    const MinimumSize* const _v1;
    const Cat* const _v2;
    const char* const _v3;
    DefTerminator (const MinimumSize* const v1, const Cat* const v2, const char* const v3) : _v1(v1), _v2(v2), _v3(v3) {}

    void accept(DefVisitor* v) override;
};

struct DefCoercions : public Def {
    const char* const _v1;
    const int _v2;
    DefCoercions (const char* const v1, const int v2) : _v1(v1), _v2(v2) {}

    void accept(DefVisitor* v) override;
};

struct DefRules : public Def {
    const char* const _v1;
    const ListRHS* const _v2;
    DefRules (const char* const v1, const ListRHS* v2) : _v1(v1), _v2(v2) {}

    void accept(DefVisitor* v) override;
};

struct DefLayout : public Def {
    const ListString* const _v1;
    explicit DefLayout (const ListString* const v1) : _v1(v1) {}

    void accept(DefVisitor* v) override;
};

struct DefLayoutStop : public Def {
    const ListString* const _v1;
    explicit DefLayoutStop (const ListString* const v1) : _v1(v1) {}

    void accept(DefVisitor* v) override;
};

struct DefLayoutTopLevel : public Def {
    DefLayoutTopLevel () = default;

    void accept(DefVisitor* v) override;
};

struct DefVisitor {
    virtual void visit(DefLabel* v) = 0;
    virtual void visit(DefLineComment* v) = 0;
    virtual void visit(DefBlockComment* v) = 0;
    virtual void visit(DefInternal* v) = 0;
    virtual void visit(DefToken* v) = 0;
    virtual void visit(DefPositionToken* v) = 0;
    virtual void visit(DefEntryPoints* v) = 0;
    virtual void visit(DefSeparator* v) = 0;
    virtual void visit(DefTerminator* v) = 0;
    virtual void visit(DefCoercions* v) = 0;
    virtual void visit(DefRules* v) = 0;
    virtual void visit(DefLayout* v) = 0;
    virtual void visit(DefLayoutStop* v) = 0;
    virtual void visit(DefLayoutTopLevel* v) = 0;
};


#endif //BNFC_BNFC_ABSYN_DEF_H
