//
// Created by anton on 2021-05-07.
//

#ifndef BNFC_STATE_H
#define BNFC_STATE_H

#include <unordered_set>
#include <unordered_map>

#include "RuleItem.h"

namespace Parser {

    struct State {
        const int id;
        std::unordered_set<RuleItem, RuleItemHasher> items;

        explicit State(const int id) : id(id), items{} {}

        bool operator==(const State& other) const;

        void closure(const std::vector<Rule>& rules, std::unordered_map<Token*, std::vector<int>>& rulesByCategory);

        void findNextStates(std::vector<State>& table, const std::vector<Rule>& rule);
    };

    struct StateHasher {
        std::size_t operator()(const State& state) {
            std::size_t sum = 0;
            RuleItemHasher hasher;
            for (auto e: state.items) {
                sum += hasher(e);
            }
            return std::hash<std::size_t>()(sum);
        }
    };
}
#endif //BNFC_STATE_H
