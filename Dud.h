//
// Created by Anton on 2020-07-21.
//

#ifndef BNFC__DUD_H
#define BNFC__DUD_H

struct Def {
    virtual ~Def() = default;

    virtual void accept(struct DefVisitor* v) = 0;
};

struct Def1 : Def {
    int a;
    void accept(struct DefVisitor* v) override;
};

struct Def2 : Def {
    int a;
    int b;
    void accept(struct DefVisitor* v) override;
};

struct DefVisitor {
    virtual void visit(Def1* token) = 0;
    virtual void visit(Def2* token) = 0;
};

#endif //BNFC__DUD_H
