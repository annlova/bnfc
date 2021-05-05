//
// Created by Anton on 2020-07-18.
//

#ifndef BNFC_BNFC_ABSYN_MINIMUMSIZE_H
#define BNFC_BNFC_ABSYN_MINIMUMSIZE_H

struct MinimumSize {
    virtual ~MinimumSize() = default;

    virtual void accept(struct MinimumSizeVisitor* v) = 0;
};

struct MinimumSizeNonempty : public MinimumSize {
    MinimumSizeNonempty () = default;

    void accept(MinimumSizeVisitor* v) override;
};

struct MinimumSizeEpsilon : public MinimumSize {
    MinimumSizeEpsilon () = default;

    void accept(MinimumSizeVisitor* v) override;
};

struct MinimumSizeVisitor {
    virtual void visit(MinimumSizeNonempty* token) = 0;
    virtual void visit(MinimumSizeEpsilon* token) = 0;
};


#endif //BNFC_BNFC_ABSYN_MINIMUMSIZE_H
