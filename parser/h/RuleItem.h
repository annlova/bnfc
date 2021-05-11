//
// Created by Anton on 2021-05-07.
//

#ifndef BNFC_PARSER_H_RULEITEM_H
#define BNFC_PARSER_H_RULEITEM_H

#include "Rule.h"
#include "Token.h"

namespace Parser {
    struct RuleItem {
        const Rule* const rule;
        const int index;
        const Token* lookahead;

        bool actionAccept;
        bool actionReduce;
        int actionReduceRuleId;

        RuleItem(const std::vector<Rule>& rules, const int ruleId, const int index, const Token* lookahead);

        bool operator==(const RuleItem& other) const {
            return rule == other.rule && index == other.index && lookahead == other.lookahead;
        }
    };

    struct RuleItemHasher {
        std::size_t operator()(const RuleItem& i) const {
            TokenHasher tokenHasher;
            return 51 + 51 * std::hash<int>()(i.index) + 13 * tokenHasher(*i.lookahead) + std::hash<const Rule*>()(i.rule);
        }
    };
}
#endif //BNFC_PARSER_H_RULEITEM_H
