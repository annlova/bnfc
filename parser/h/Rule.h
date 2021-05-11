//
// Created by anton on 2021-05-07.
//

#ifndef BNFC_RULE_H
#define BNFC_RULE_H

#include <utility>
#include <vector>
#include "Token.h"

namespace Parser {
    struct Rule {
        const int id;
        const Token* const cat;
        const std::vector<Token*> prd;

        Rule(const int id, const Token* const cat, const std::vector<Token*>& prd) : id(id), cat(cat), prd(std::move(prd)) {}

        bool isReductionOf(const Rule* const rule, int index) const;
    };
}
#endif //BNFC_RULE_H
