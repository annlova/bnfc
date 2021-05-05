//
// Created by Anton on 2021-05-04.
//
//
//#include <utility>
//#include <vector>
//#include <memory>
//#include <set>
//#include <unordered_map>
//#include <cassert>
//#include <iostream>
//
//struct Symbol {
//    const int id;
//    const bool terminal;
//
//    Symbol(const int id, const bool terminal) : id(id), terminal(terminal) {}
//
//    bool operator==(const Symbol& other) const {
//        bool equal = id == other.id;
//        assert(!equal || equal && terminal == other.terminal);
//        return equal;
//    }
//
//};
//
//struct SymbolHasher {
//    std::size_t operator()(const Symbol& s) const {
//        return s.id;
//    }
//};
//
//struct Rule {
//    const int id;
//    const Symbol category;
//    const std::vector<Symbol> production;
//
//    Rule(const int id, const Symbol& category, std::vector<Symbol> production)
//    : id(id), category(category), production(std::move(production)) {}
//};
//
//struct Item {
//    const std::shared_ptr<const Rule> rule;
//    const int index;
//
//    Item(std::shared_ptr<const Rule> rule, const int index) : rule(std::move(rule)), index(index) {}
//};
//
//struct State {
//    const int id;
//    std::set<Item> items{};
//
//    explicit State(const int id) : id(id) {}
//};
//
//std::vector<Rule> rules;
//std::unordered_map<Symbol, std::vector<int>, SymbolHasher> ruleCategoryMap;
//
//void closure(State& state) {
//    bool itemsAdded = false;
//
//    for (auto& item: state.items) {
//        if (item.index == item.rule->production.size()) {
//            // There is no next symbol - do nothing
//        } else if (!item.rule->production[item.index].terminal) {
//            // Next symbol is non-terminal - add new items to the new state
//            // for each rule the non-terminal is the category for
//            auto categoryRules = ruleCategoryMap[item.rule->production[item.index]];
//            for (auto ruleIndex: categoryRules) {
////                auto err = state.items.emplace(std::make_shared<Rule>(rules[ruleIndex]), 0);
////                if (err.second) {
////                    // New rule item added - keep track
////                    itemsAdded = true;
////                    // TODO: Fix inefficiency
////                }
//            }
//        }
//    }
//
//    if (itemsAdded) {
//        closure(state);
//    }
//}
//
///*
// * 0 - S
// * 1 - E
// * 2 - B
// * 3 - 0
// * 4 - 1
// * 5 - +
// * 6 - *
// * 7 - eof
// */
//void test() {
//    Symbol S(0, false);
//    Symbol E(1, false);
//    Symbol B(2, false);
//    Symbol NIL(3, true);
//    Symbol ONE(4, true);
//    Symbol ADD(5, true);
//    Symbol MUL(6, true);
//    Symbol END(7, true);
//
//    rules.emplace_back(0, S, std::initializer_list<Symbol>{E, END});
//    rules.emplace_back(1, E, std::initializer_list<Symbol>{E, MUL, B});
//    rules.emplace_back(2, E, std::initializer_list<Symbol>{E, ADD, B});
//    rules.emplace_back(3, E, std::initializer_list<Symbol>{B});
//    rules.emplace_back(4, B, std::initializer_list<Symbol>{NIL});
//    rules.emplace_back(5, B, std::initializer_list<Symbol>{ONE});
//
//    ruleCategoryMap[S].push_back(0);
//    ruleCategoryMap[E].push_back(1);
//    ruleCategoryMap[E].push_back(2);
//    ruleCategoryMap[E].push_back(3);
//    ruleCategoryMap[B].push_back(4);
//    ruleCategoryMap[B].push_back(5);
//
////    Item startRuleItem(std::make_shared<Rule>(rules[0]), 0);
////    State state(0);
////    state.items.insert(startRuleItem);
////
////    std::cout << "Before closure:" << std::endl;
////    for (auto& item: state.items) {
////        std::cout << item.rule->id << ", " << item.index << std::endl;
////    }
////
////    closure(state);
////
////    std::cout << "After closure:" << std::endl;
////    for (auto& item: state.items) {
////        std::cout << item.rule->id << ", " << item.index << std::endl;
////    }
//}