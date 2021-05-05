//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_REG_H
#define BNFC_BNFC_REG_H


struct Reg {
    virtual ~Reg() = default;

    virtual void accept(struct RegVisitor* v) = 0;
};

struct RegReg : public Reg {
    const Reg* const _v1;
    explicit RegReg (const Reg* const v1) : _v1(v1) {}

    void accept(RegVisitor* v) override;
};

struct RegChar : public Reg {
    RegChar () = default;

    void accept(RegVisitor* v) override;
};

struct RegLower : public Reg {
    RegLower () = default;

    void accept(RegVisitor* v) override;
};

struct RegUpper : public Reg {
    RegUpper () = default;

    void accept(RegVisitor* v) override;
};

struct RegLetter : public Reg {
    RegLetter () = default;

    void accept(RegVisitor* v) override;
};

struct RegDigit : public Reg {
    RegDigit () = default;

    void accept(RegVisitor* v) override;
};

struct RegSquareString : public Reg {
    const char* const _v1;
    explicit RegSquareString (const char* const v1) : _v1(v1) {}

    void accept(RegVisitor* v) override;
};

struct RegCurlyString : public Reg {
    const char* const _v1;
    explicit RegCurlyString (const char* const v1) : _v1(v1) {}

    void accept(RegVisitor* v) override;
};

struct RegEps : public Reg {
    RegEps () = default;

    void accept(RegVisitor* v) override;
};

struct RegRealChar : public Reg {
    const char _v1;
    explicit RegRealChar (const char v1) : _v1(v1) {}

    void accept(RegVisitor* v) override;
};

struct RegStar : public Reg {
    RegStar () = default;

    void accept(RegVisitor* v) override;
};

struct RegPlus : public Reg {
    RegPlus () = default;

    void accept(RegVisitor* v) override;
};

struct RegQuestion : public Reg {
    RegQuestion () = default;

    void accept(RegVisitor* v) override;
};

struct RegMinus : public Reg {
    const Reg* const _v1;
    const Reg* const _v2;
    RegMinus (const Reg* const v1, const Reg* const v2) : _v1(v1), _v2(v2) {}

    void accept(RegVisitor* v) override;
};

struct RegOr : public Reg {
    const Reg* const _v1;
    const Reg* const _v2;
    RegOr (const Reg* const v1, const Reg* const v2) : _v1(v1), _v2(v2) {}

    void accept(RegVisitor* v) override;
};

struct RegDoubleReg : public Reg {
    const Reg* const _v1;
    const Reg* const _v2;
    RegDoubleReg (const Reg* const v1, const Reg* const v2) : _v1(v1), _v2(v2) {}

    void accept(RegVisitor* v) override;
};

struct RegVisitor {
    virtual void visit(RegReg* token) = 0;
    virtual void visit(RegOr* token) = 0;
    virtual void visit(RegMinus* token) = 0;
    virtual void visit(RegDoubleReg* token) = 0;
    virtual void visit(RegStar* token) = 0;
    virtual void visit(RegPlus* token) = 0;
    virtual void visit(RegQuestion* token) = 0;
    virtual void visit(RegEps* token) = 0;
    virtual void visit(RegRealChar* token) = 0;
    virtual void visit(RegSquareString* token) = 0;
    virtual void visit(RegCurlyString* token) = 0;
    virtual void visit(RegDigit* token) = 0;
    virtual void visit(RegLetter* token) = 0;
    virtual void visit(RegUpper* token) = 0;
    virtual void visit(RegLower* token) = 0;
    virtual void visit(RegChar* token) = 0;
};


#endif //BNFC_BNFC_REG_H
