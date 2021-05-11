//
// Created by Anton on 2021-05-07.
//

#include "../h/Rule.h"

bool Parser::Rule::isReductionOf(const Parser::Rule* const rule, int index) const {
    if (rule->prd.size() == index) {
        for (int i = 0; i < prd.size(); i++) {
            if (prd[i] != rule->prd[i]) {
                return false;
            }
        }

        return true;
    }

    return false;
}
