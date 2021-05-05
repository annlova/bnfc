
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <set>
#include <sstream>

struct Symbol {
    const int id;
    const bool terminal;

    Symbol(const int id, const bool terminal) : id(id), terminal(terminal) {}

    bool operator==(const Symbol& other) const {
        bool equal = id == other.id;
        assert(!equal || equal && terminal == other.terminal);
        return equal;
    }
};

struct SymbolHasher {
    std::size_t operator()(const Symbol& s) const {
        return std::hash<int>()(s.id);
    }
};

struct Rule {
    const int id;
    const Symbol category;
    const std::vector<Symbol> production;

    Rule(const int id, const Symbol& category, std::vector<Symbol> production)
            : id(id), category(category), production(std::move(production)) {}
};

struct Item {
    const Rule* rule;
    const int index;
    bool accept;

    Item(const Rule* rule, const int index) : rule(rule), index(index), accept(false) {
        if(rule->production[index] == Symbol(0, false)) {
            accept = true;
        }
    }

    bool operator==(const Item& other) const {
        return index == other.index && rule == other.rule;
    }
};

struct ItemHasher {
    std::size_t operator()(const Item& i) const {
        return 51 + 51 * std::hash<int>()(i.index) + std::hash<const Rule*>()(i.rule);
    }
};

enum Action {
    REDUCE,
    SHIFT,
    ACCEPT,
    GOTO
};
struct State {
    const int id;
    std::unordered_set<Item, ItemHasher> items{};

    explicit State(const int id) : id(id) {}

    bool operator==(const State& other) const {
        return items == other.items;
    }
};

struct StateHasher {
    std::size_t operator()(const State& s) const {
        std::size_t sum = 0;
        ItemHasher ih;
        for (auto e: s.items) {
            sum += ih(e);
        }
        return sum;
    }
};

std::vector<Rule> rules;
std::unordered_map<Symbol, std::vector<int>, SymbolHasher> ruleCategoryMap;
std::unordered_set<State, StateHasher> states;
std::vector<const State*> statesList;
std::vector<std::vector<std::pair<Action, int>>> table;
constexpr int numsymbols = 8;


void closure(State& state) {
    bool itemsAdded = false;

    for (auto& item: state.items) {
        if (item.index == item.rule->production.size()) {
            // There is no next symbol - do nothing
        } else if (!item.rule->production[item.index].terminal) {
            // Next symbol is non-terminal - add new items to the new state
            // for each rule the non-terminal is the category for
            auto categoryRules = ruleCategoryMap[item.rule->production[item.index]];
            for (auto ruleIndex: categoryRules) {
                auto err = state.items.emplace(&rules[ruleIndex], 0);
                if (err.second) {
                    // New rule item added - keep track
                    itemsAdded = true;
                    // TODO: Fix inefficiency
                }
            }
        }
    }

    if (itemsAdded) {
        closure(state);
    }
}

void writeRuleItem(int id, int index, std::stringstream& ss) {
    std::stringstream sst;
    switch (id) {
        case 0:
            sst << "S -> E $";
            break;
        case 1:
            sst << "E -> E * B";
            break;
        case 2:
            sst << "E -> E + B";
            break;
        case 3:
            sst << "E -> B";
            break;
        case 4:
            sst << "B -> 0";
            break;
        case 5:
            sst << "B -> 1";
            break;
        default:;
    };


    auto offset = 4 + index * 2;
    std::stringstream temp;
    temp << sst.str().substr(0, offset);
    temp << " ¤ ";
    if (offset < sst.str().length()) {
        temp << sst.str().substr(offset + 1);
    }
    temp << std::endl;
    ss << temp.rdbuf();
}

void createNextStates(const State& state) {
    std::unordered_map<Symbol, std::vector<Item>, SymbolHasher> itemsCategoryMap;
    for (auto& item: state.items) {
        if (item.index < item.rule->production.size()) {
            itemsCategoryMap[item.rule->production[item.index]].emplace_back(item.rule, item.index + 1);
        }
    }

    for (auto& items: itemsCategoryMap) {
        State s(table.size());

        bool accept = false;
        for (auto& item : items.second) {
            s.items.insert(item);
            if (item.accept) {
                accept = true;
            }
        }

        std::stringstream ss;
        ss << std::endl << "Before closure:" << std::endl;
        for (auto& item: s.items) {
            writeRuleItem(item.rule->id, item.index, ss);
        }

        closure(s);

        ss << "After closure:" << std::endl;
        for (auto& item: s.items) {
            writeRuleItem(item.rule->id, item.index, ss);
        }

        auto inserted = states.insert(std::move(s));

        if (inserted.second) {
            std::cout << ss.str();
            statesList.push_back(&(*inserted.first));

            table.emplace_back(numsymbols);

            auto& row = table[table.size() - 1];
            if (accept) {
                row[0] = std::pair<Action, int>(Action::ACCEPT, 0); // EOF column
            }

            createNextStates(*inserted.first);
        }
    }
}

/*
 * 0 - S
 * 1 - E
 * 2 - B
 * 3 - 0
 * 4 - 1
 * 5 - +
 * 6 - *
 * 7 - eof
 */
void test() {
    Symbol END(0, true);
    Symbol S(1, false);
    Symbol E(2, false);
    Symbol B(3, false);
    Symbol NIL(4, true);
    Symbol ONE(5, true);
    Symbol ADD(6, true);
    Symbol MUL(7, true);

    rules.emplace_back(0, S, std::initializer_list<Symbol>{E, END});
    rules.emplace_back(1, E, std::initializer_list<Symbol>{E, MUL, B});
    rules.emplace_back(2, E, std::initializer_list<Symbol>{E, ADD, B});
    rules.emplace_back(3, E, std::initializer_list<Symbol>{B});
    rules.emplace_back(4, B, std::initializer_list<Symbol>{NIL});
    rules.emplace_back(5, B, std::initializer_list<Symbol>{ONE});

    ruleCategoryMap[S].push_back(0);
    ruleCategoryMap[E].push_back(1);
    ruleCategoryMap[E].push_back(2);
    ruleCategoryMap[E].push_back(3);
    ruleCategoryMap[B].push_back(4);
    ruleCategoryMap[B].push_back(5);

    Item startRuleItem(&rules[0], 0);
    State state(0);
    state.items.insert(startRuleItem);

    std::stringstream ss;
    ss << "Before closure:" << std::endl;
    for (auto& item: state.items) {
        writeRuleItem(item.rule->id, item.index, ss);
    }

    closure(state);

    ss << "After closure:" << std::endl;
    for (auto& item: state.items) {
        writeRuleItem(item.rule->id, item.index, ss);
    }

    std::cout << ss.str();

    auto inserted = states.insert(std::move(state));
    createNextStates(*inserted.first);

    std::cout << std::endl << "Done! " << states.size() << " rules created." << std::endl;
}

int main(const int argCount, const char* argVector[]) {
    test();
    return 0;
}

//
//#include "bnfc/absyn/LabelId.h"
//#include "bnfc/absyn/ListInteger.h"
//
//#include <assert.h>
//#include <iostream>
//#include <windows.h>
//#include <fstream>
//#include <vector>
//
//void parse (std::string&, std::string&);
//
//void setConsoleColor(WORD);
//void resetConsoleColor();
//
//// Console text color variables
//HANDLE hstdout;
//WORD defaultConsoleAttribute;
//
//// Token colors
//constexpr WORD lineCommentColor = 0x09;
//constexpr WORD terminalColor = 0x02;
//constexpr WORD literalColor = 0x04;
//constexpr WORD keywordColor = 0x05;
//constexpr WORD labelColor = 0x03;
//
//#include <regex>
//
//#include "Dud.h"
//
//struct Abc {
//    Def* def1;
//    Def* def2;
//};
//
//struct Visitor : DefVisitor {
//    void visit(Def1* token) override {
//        std::cout << "a: " << token->a << std::endl;
//    }
//
//    void visit(Def2* token) override {
//        std::cout << "a: " << token->a << " b: " << token->b << std::endl;
//    }
//};
//#include <memory>
//#include <unordered_map>
//#include <set>
//
//#include "LR0.cpp"
//
//int main(const int argCount, const char* argVector[]) {
//    test();
//    return 0;
////    int a1 = 5;
////
////    int a2 = 7;
////    int b2 = 10;
////
////    auto ddd = new Def1();
////    auto vptr = *(void**) ddd;
////
////    auto def1 = malloc(sizeof(Def1));
////    auto def1Bytes = static_cast<char*>(def1);
////    new (def1Bytes) void*(vptr);
////    new (def1Bytes + sizeof(Def)) int(a1);
////
////    delete ddd;
////
////    auto ddd2 = new Def2();
////    auto vptr2 = *(void**) ddd2;
////
////    auto def2 = malloc(sizeof(Def2));
////    auto def2Bytes = static_cast<char*>(def2);
////    new (def2Bytes) void*(vptr2);
////    new (def2Bytes + sizeof(Def)) int(a2);
////    new (def2Bytes + sizeof(Def) + sizeof(int)) int(b2);
////
////    delete ddd2;
////
////    auto abc = malloc(sizeof(void*) * 2);
////    auto abcBytes = static_cast<char*>(abc);
////    new (abcBytes) void*(def1);
////    new (abcBytes + sizeof(void*)) void*(def2);
////
////    auto test = reinterpret_cast<Abc*>(abc);
////
////    Visitor visitor{};
////    test->def1->accept(&visitor);
////    test->def2->accept(&visitor);
////
////    free(abc);
////    free(def2);
////    free(def1);
////    return 0;
//
////    std::regex_match(, std::regex(R"()"));
//    assert(argCount == 3); // Make sure a single path to a file containing the BNF grammar.
//
//    hstdout = GetStdHandle(STD_OUTPUT_HANDLE);
//    CONSOLE_SCREEN_BUFFER_INFO info{};
//    GetConsoleScreenBufferInfo(hstdout, &info);
//    defaultConsoleAttribute = info.wAttributes;
//
//    std::string bnfFilePath(argVector[1]);
//    std::string parserInfoPath(argVector[2]);
//    parse(bnfFilePath, parserInfoPath);
//
//    return 0;
//}
//
//struct RuleItem;
//
//std::unordered_map<int, std::vector<RuleItem*>> ruleItems;
//
//struct RuleItem {
//    int id;
//    int cat;
//    std::vector<int> prod;
//
//    void closure(std::set<std::pair<int, int>>* const items, int pos) {
//        items->insert({id, pos});
//        if (pos >= prod.size() || !ruleItems.count(prod[pos])) {
//            return;
//        }
//
//        for (auto* item: ruleItems[prod[pos]]) {
//            if (item )
//            item->closure(items, 0);
//        }
//    }
//};
//
//struct RuleSet {
//    std::set<int> items;
//};
//
//auto ruleSetComp = [](const RuleSet* setA, const RuleSet* setB) { return setA->items == setB->items; };
//std::set<RuleSet*, decltype(ruleSetComp)> ruleSets(ruleSetComp);
//
//void parse (std::string& bnfFilePath, std::string& parserInfoPath) {
//    std::ifstream parserInfo(parserInfoPath);
//    if (!parserInfo.is_open()) {
//        std::cout << "Failed to load parser info file: \"\n" << parserInfoPath << "\".";
//        return;
//    }
//
//    std::vector<std::regex> tokens;
//    std::vector<std::regex> tokensPartial;
//
//    std::unordered_map<std::string, int> tokenNameMap;
//    int nextTokenNameAssociation = 0;
//
//    // Read in the parser info
//    {
//        // Read token regex
//        std::string line;
//        while (std::getline(parserInfo, line)) {
//            std::istringstream iss(line);
//            if (line[0] == '0') {
//                std::string key;
//                std::string completeRegex;
//                std::string partialRegex;
//                if (!(iss >> key >> completeRegex >> partialRegex)) {
//                    std::cout << "Error when reading in token regex." << std::endl;
//                    parserInfo.close();
//                    return;
//                }
//
//                if (!tokenNameMap.count(key)) {
//                    tokenNameMap[key] = nextTokenNameAssociation;
//                    nextTokenNameAssociation++;
//                } else {
//                    std::cout << "Error when filling tokenNameMap: Duplicate entries." << std::endl;
//                    parserInfo.close();
//                    return;
//                }
//
//                tokens.emplace_back(completeRegex, std::regex_constants::optimize | std::regex_constants::ECMAScript);
//                tokensPartial.emplace_back(partialRegex, std::regex_constants::optimize | std::regex_constants::ECMAScript);
//
//            } else if (line[0] == '#') {
//                char throwaway;
//                std::string item;
//                iss >> throwaway;
//                iss >> item;
//                if (!tokenNameMap.count(item)) {
//                    tokenNameMap[item] = nextTokenNameAssociation;
//                    nextTokenNameAssociation++;
//                } else {
//                    std::cout << "Error when filling tokenNameMap: Duplicate entries (\"" << item << "\")." << std::endl;
//                    parserInfo.close();
//                    return;
//                }
//            } else {
//                std::string item;
//                int rule = 0;
//
//                iss >> item;
//                iss >> rule;
//
//                const int catId = tokenNameMap[item];
//
//                int id = 0;
//                auto ruleItem = new RuleItem;
//
//                ruleItem->id = id;
//                id++;
//
//                ruleItem->cat = catId;
//
//                std::string prod;
//                for (iss >> prod; prod != "1"; ) {
//                    ruleItem->prod.emplace_back(tokenNameMap[prod]);
//                }
//
//                ruleItem->pos = 0;
//
//                ruleItems[catId].push_back(ruleItem);
//            }
//        }
//    }
//
//
//    std::ifstream bnfFile(bnfFilePath);
//    if (!bnfFile.is_open()) {
//        std::cout << "Failed to load BNF grammar file: \"\n" << bnfFilePath << "\".";
//        return;
//    }
//
//    // Read in the BNF grammar file
//    bnfFile.seekg(0, std::ios::end);
//    auto length = bnfFile.tellg();
//    bnfFile.seekg(0, std::ios::beg);
//    auto data = new char[length];
//    bnfFile.read(data, length);
//    bnfFile.close();
//
//    // Parse the source
//    auto* const dataStart = data;
//    std::vector<int> integerTokens;
//
//    char token[50];
//    for (char& i : token) {
//        i = 0;
//    }
//
//    bool tokFlag[tokens.size()];
//    for (bool& i : tokFlag) {
//        i = true;
//    }
//
//    for (int tokenSize = 0; *data != 0; data++) {
//        if (tokenSize == 0 && (*data == ' ' || *data == '\t' || *data == '\n' || *data == '\r')) {
//            std::cout << *data;
//            continue;
//        }
//        token[tokenSize] = *data;
//        tokenSize++;
//        if (tokenSize == 1) {
//            continue;
//        }
//
//        int singleMatch = -2;
//        for (int i = 0; i < tokens.size(); i++) {
//            if (tokFlag[i]) {
//                bool match = std::regex_match(token, tokensPartial[i]);
//                tokFlag[i] = match;
//                if (match && singleMatch >= 0) {
//                    singleMatch = -1;
//                    break;
//                } else if (match && std::regex_match(token, tokens[i]) && singleMatch != -3) {
//                    singleMatch = i;
//                } else if (match) {
//                    singleMatch = -3;
//                }
//            }
//        }
//
//        if (singleMatch >= 0) {
//            WORD col = 0;
//            if (singleMatch == 0) {
//                col = labelColor;
//            } else if (singleMatch < 3) {
//                col = terminalColor;
//            } else if (singleMatch < 4) {
//                col = literalColor;
//            } else if (singleMatch < 23) {
//                col = keywordColor;
//            } else if (singleMatch < 39) {
//                col = defaultConsoleAttribute;
//            } else if (singleMatch < 41) {
//                col = lineCommentColor;
//            } else {
//                col = literalColor;
//            }
//
//            char lookahead = token[tokenSize - 1];
//            token[tokenSize - 1] = 0;
//
//            setConsoleColor(col);
//            std::cout << token;
//            resetConsoleColor();
//
//            integerTokens.emplace_back(singleMatch);
//
//            tokenSize = 0;
//            for (char& i : token) {
//                i = 0;
//            }
//            if (!(lookahead == ' ' || lookahead == '\t' || lookahead == '\n' || lookahead == '\r')) {
//                token[0] = lookahead;
//                tokenSize = 1;
//            } else {
//                std::cout << lookahead;
//            }
//
//            for (bool& i : tokFlag) {
//                i = true;
//            }
//        } else if (singleMatch == -2) {
//            std::cout << std::endl << "ERROR! Could not lex :(" << std::endl;
//            break;
//        }
//    }
//
//    delete[] dataStart;
//
//    std::cout << std::endl;
//    for (int& i: integerTokens) {
//        std::cout << i << " ";
//    }
//}
//
//// Only works for ASCII characters A-Z
//bool isCapitalLetter(const char c) {
//    return (c >= 65 && c <= 90);
//}
//
//// Only works for ASCII characters A-Za-z
//bool isLetter(const char c) {
//    return (c >= 65 && c <= 90) || (c >= 97 && c <= 122);
//}
//
//void setConsoleColor(WORD attr) {
//    SetConsoleTextAttribute(hstdout, attr);
//}
//
//void resetConsoleColor() {
//    SetConsoleTextAttribute(hstdout, defaultConsoleAttribute);
//}