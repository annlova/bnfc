//
// Created by Anton on 2021-05-07.
//

#include "../h/RuleItem.h"

#include <iostream>


Parser::RuleItem::RuleItem(const std::vector<Rule>& rules, const int ruleId, const int index, const Parser::Token* lookahead)
    : rule(&rules[ruleId]), index(index), lookahead(lookahead), actionAccept(false), actionReduce(false) {

    // Check if this rule item will incur an accept action
    if (index < rule->prd.size() &&
        rule->prd[index]->id == eofTokenId) {
        actionAccept = true;
    }

    // Check if this rule item will incur a reduce action
    // TODO: possible improvement, make rules list indexed for faster lookup
    for (int i = 0; i < rules.size(); i++) {
        if(rules[i].isReductionOf(rule, index)) {
            if (actionReduce) {
                // TODO: Change logging method, also remove if optimization is more important
                std::cout << "Error! Item can be reduced to multiple rules!" << std::endl;
                break;
            }
            actionReduce = true;
            actionReduceRuleId = i;
        }
    }
}
