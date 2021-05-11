//
// Created by Anton on 2021-05-07.
//

#include "../h/State.h"

bool Parser::State::operator==(const Parser::State& other) const {
    return items == other.items;
}

void Parser::State::closure(const std::vector<Rule>& rules, std::unordered_map<Token*, std::vector<int>>& rulesByCategory) {
    bool itemsAdded = false;

    for (auto& item: items) {
        if (item.index == item.rule->prd.size()) {
            // There is no next symbol - do nothing
        } else if (!item.rule->prd[item.index]->terminal) {
            // Next symbol is non-terminal - add new items to the new state
            // for each rule the non-terminal is the category for
            std::vector<const Token*> lookaheads;
            if (item.index + 1 == item.rule->prd.size()) {
                lookaheads.push_back(item.lookahead);
            } else {
                auto lookahead = item.rule->prd[item.index + 1];
                if (lookahead->terminal) {
                    lookaheads.push_back(lookahead);
                } else {
                    std::vector<int> search;
                    std::unordered_set<int> searchSet;
                    for (auto e: rulesByCategory[lookahead]) {
                        search.push_back(e);
                        searchSet.insert(e);
                    }
                    for (int i = 0; i < search.size(); i++) {
                        auto* token = rules[i].prd[0];
                        if (token->terminal) {
                            lookaheads.push_back(token);
                        } else {
                            for (auto e: rulesByCategory[token]) {
                                auto inserted = searchSet.insert(e);
                                if (inserted.second) {
                                    search.push_back(e);
                                }
                            }
                        }
                    }
                }
            }

            auto categoryRules = rulesByCategory[item.rule->prd[item.index]];
            for (auto ruleIndex: categoryRules) {
                for (auto lookahead: lookaheads) {
                    auto inserted = items.emplace(rules, ruleIndex, 0, lookahead);
                    if (inserted.second) {
                        // New rule item added - keep track
                        itemsAdded = true;
                        // TODO: Fix inefficiency
                    }
                }
            }
        }
    }

    if (itemsAdded) {
        closure(rules, rulesByCategory);
    }
}

void Parser::State::findNextStates(std::vector<State>& table, const std::vector<Rule>& rules) {
    // Sort the states existing rules by category. Each category will create a new state.
    std::unordered_map<Token*, std::vector<RuleItem>> itemsByCategory;
    for (auto& item: items) {
        auto rule = item.rule;
        auto index = item.index;
        auto symbol = rule->prd[index];
        auto lookahead = item.lookahead;

        if (index < rule->prd.size() &&
            symbol->id != eofTokenId) {
            itemsByCategory[symbol].emplace_back(rules, rule->id, index + 1, lookahead);
        }
    }

    // Go through each rule category and construct a new state
    for (auto& categoryItems: itemsByCategory) {
        auto stateId = table.size();

    }
}
