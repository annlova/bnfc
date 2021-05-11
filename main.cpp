
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "parser/h/State.h"

#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cassert>
#include <iostream>
#include <set>
#include <sstream>
#include <iomanip>

struct Symbol {
    const int id;
    const bool terminal;
    const std::string sym;

    Symbol(const int id, const bool terminal, std::string sym) : id(id), terminal(terminal), sym(std::move(sym)) {}

    bool operator==(const Symbol& other) const {
        bool equal = id == other.id;
        assert(!equal || equal && terminal == other.terminal);
        return equal;
    }

    bool operator!=(const Symbol& other) const {
        bool unequal = id != other.id;
        assert(unequal || !unequal && terminal == other.terminal);
        return unequal;
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

    Rule(const int id, Symbol category, std::vector<Symbol> production)
            : id(id), category(std::move(category)), production(std::move(production)) {}

    bool isReduction(const Rule& rule, int index) {
        if (production.size() == index) {
            for (int i = 0; i < production.size(); i++) {
                if (production[i] != rule.production[i]) {
                    return false;
                }
            }

            return true;
        }

        return false;
    }
};

std::vector<Rule> rules;
std::unordered_map<Symbol, std::vector<int>, SymbolHasher> ruleCategoryMap;

struct Item {
    const Rule* rule;
    const int index;
    bool accept;
    bool reduce;
    int reduceRule;
    const Symbol lookahead;

    Item(const Rule* rule, const int index, Symbol  lookahead) : rule(rule), index(index), accept(false), reduce(false), reduceRule(0),
                                                                      lookahead(std::move(lookahead)) {
        if (index < rule->production.size()) {
            if(rule->production[index] == Symbol(0, true, "$")) {
                accept = true;
            }
        }

        for (int i = 0; i < rules.size(); i++) {
            if(rules[i].isReduction(*rule, index)) {
                if (reduce) {
                    std::cout << "Error! Item can be reduced to multiple rules!" << std::endl;
                    break;
                }
                reduce = true;
                reduceRule = i;
            }
        }
    }

    bool operator==(const Item& other) const {
        return index == other.index && rule == other.rule && lookahead == other.lookahead;
    }
};

struct ItemHasher {
    std::size_t operator()(const Item& i) const {
        SymbolHasher hasher;
        return 51 + 51 * std::hash<int>()(i.index) + 13 * hasher(i.lookahead) + std::hash<const Rule*>()(i.rule);
    }
};

enum Action {
    REDUCE,
    SHIFT,
    ACCEPT,
    GOTO,
    NA
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

std::unordered_set<State, StateHasher> states;
std::vector<const State*> statesList;
std::vector<std::vector<std::pair<Action, int>>> table;
constexpr int numsymbols = 6;

void closure(State& state) {
    bool itemsAdded = false;

    for (auto& item: state.items) {
        if (item.index == item.rule->production.size()) {
            // There is no next symbol - do nothing
        } else if (!item.rule->production[item.index].terminal) {
            // Next symbol is non-terminal - add new items to the new state
            // for each rule the non-terminal is the category for
            std::vector<const Symbol*> lookaheads;
            if (item.index + 1 == item.rule->production.size()) {
                lookaheads.push_back(&item.lookahead);
            } else {
                 auto lookahead = &item.rule->production[item.index + 1];
                 if (lookahead->terminal) {
                     lookaheads.push_back(lookahead);
                 } else {
                     std::vector<int> search;
                     std::unordered_set<int> searchSet;
                     for (auto e: ruleCategoryMap[*lookahead]) {
                         search.push_back(e);
                         searchSet.insert(e);
                     }
                     for (int i = 0; i < search.size(); i++) {
                         auto* sym = &rules[i].production[0];
                         if (sym->terminal) {
                             lookaheads.push_back(sym);
                         } else {
                             for (auto e: ruleCategoryMap[*sym]) {
                                 auto inserted = searchSet.insert(e);
                                 if (inserted.second) {
                                     search.push_back(e);
                                 }
                             }
                         }
                     }
                 }
            }

            auto categoryRules = ruleCategoryMap[item.rule->production[item.index]];
            for (auto ruleIndex: categoryRules) {
                for (auto lookahead: lookaheads) {
                    auto inserted = state.items.emplace(&rules[ruleIndex], 0, *lookahead);
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
        closure(state);
    }
}

void writeRuleItem(Item item, std::stringstream& ss) {
    int id = item.rule->id;
    int index = item.index;
    auto lookahead = item.lookahead;

    std::stringstream sst;
    sst << item.rule->category.sym << " -> ";
    for (auto& e: item.rule->production) {
        sst << e.sym << " ";
    }

    sst << ", " << lookahead.sym;

    auto offset = 4 + index * 2;
    std::stringstream temp;
    temp << sst.str().substr(0, offset);
    temp << " .";
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
            if (item.rule->production[item.index] == Symbol(0, true, "$")) {
                continue;
            }
            itemsCategoryMap[item.rule->production[item.index]].emplace_back(item.rule, item.index + 1, item.lookahead);
        }
    }

    for (auto& items: itemsCategoryMap) {
        State s(table.size());

        bool accept = false;
        bool reduce = false;
        int reduceRule = 0;
        for (auto& item : items.second) {
            s.items.insert(item);
            if (item.accept) {
                accept = true;
            }
            if (item.reduce) {
                reduce = true;
                reduceRule = item.reduceRule;
            }
        }

        std::stringstream ss;
        ss << std::endl << "Before closure:" << std::endl;
        for (auto& item: s.items) {
            writeRuleItem(item, ss);
        }

        closure(s);

        ss << "After closure:" << std::endl;
        for (auto& item: s.items) {
            writeRuleItem(item, ss);
        }

        auto inserted = states.insert(std::move(s));

        {
            auto& associatedRow = table[state.id];
            auto& sym = items.first;
            if (sym.terminal) {
                associatedRow[sym.id] = std::pair<Action, int>(Action::SHIFT, inserted.first->id);
            } else {
                associatedRow[sym.id] = std::pair<Action, int>(Action::GOTO, inserted.first->id);
            }
        }

        if (inserted.second) {
            std::cout << ss.str();
            statesList.push_back(&(*inserted.first));

            table.emplace_back(numsymbols);

            auto& row = table[table.size() - 1];
            for (auto& e: row) {
                e = std::pair<Action, int>(Action::NA, -1);
            }

            if (reduce) {
                for (auto& i : row) {
                    if (i.first != Action::GOTO) {
                        i.first = Action::REDUCE;
                        i.second = reduceRule;
                    }
                }
            }

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
//    Symbol END(0, true, "$");
//    Symbol S(1, false, "S");
//    Symbol E(2, false, "E");
//    Symbol B(3, false, "B");
//    Symbol NIL(4, true, "0");
//    Symbol ONE(5, true, "1");
//    Symbol ADD(6, true, "+");
//    Symbol MUL(7, true, "*");

    Symbol END(0, true, "$");
    Symbol S(4, false, "S");
    Symbol C(5, false, "C");
    Symbol c(1, true, "c");
    Symbol d(2, true, "d");
    Symbol SPRIME(3, false, "S'");

//    rules.emplace_back(0, S, std::initializer_list<Symbol>{E, END});
//    rules.emplace_back(1, E, std::initializer_list<Symbol>{E, MUL, B});
//    rules.emplace_back(2, E, std::initializer_list<Symbol>{E, ADD, B});
//    rules.emplace_back(3, E, std::initializer_list<Symbol>{B});
//    rules.emplace_back(4, B, std::initializer_list<Symbol>{NIL});
//    rules.emplace_back(5, B, std::initializer_list<Symbol>{ONE});

    rules.emplace_back(0, SPRIME, std::initializer_list<Symbol>{S, END});
    rules.emplace_back(1, S, std::initializer_list<Symbol>{C, C});
    rules.emplace_back(2, C, std::initializer_list<Symbol>{c, C});
    rules.emplace_back(3, C, std::initializer_list<Symbol>{d});

//    ruleCategoryMap[S].push_back(0);
//    ruleCategoryMap[E].push_back(1);
//    ruleCategoryMap[E].push_back(2);
//    ruleCategoryMap[E].push_back(3);
//    ruleCategoryMap[B].push_back(4);
//    ruleCategoryMap[B].push_back(5);

    ruleCategoryMap[S].push_back(0);
    ruleCategoryMap[S].push_back(1);
    ruleCategoryMap[C].push_back(2);
    ruleCategoryMap[C].push_back(3);

    Item startRuleItem(&rules[0], 0, END);
    State state(0);
    state.items.insert(startRuleItem);

    table.emplace_back(numsymbols);
    for (auto& e: table[0]) {
        e = std::pair<Action, int>(Action::NA, -1);
    }

    std::stringstream ss;
    ss << "Before closure:" << std::endl;
    for (auto& item: state.items) {
        writeRuleItem(item, ss);
    }

    closure(state);

    ss << "After closure:" << std::endl;
    for (auto& item: state.items) {
        writeRuleItem(item, ss);
    }

    std::cout << ss.str();

    auto inserted = states.insert(std::move(state));
    createNextStates(*inserted.first);

    std::cout << std::endl << "Done! " << states.size() << " rules created." << std::endl;

    std::cout << std::endl << "   $   c   d   S'  S   C" << std::endl;
    std::cout << std::left;
    int rc = 0;
    for(auto& row: table) {
        std::cout << std::setw(2) << rc++ << " ";
        for (auto& e: row) {
            switch (e.first) {
                case REDUCE:
                    std::cout << "r" << std::setw(2) << e.second << " ";
                    break;
                case SHIFT:
                    std::cout << "s" << std::setw(2) << e.second << " ";
                    break;
                case ACCEPT:
                    std::cout << "acc ";
                    break;
                case GOTO:
                    std::cout << "g" << std::setw(2) << e.second << " ";
                    break;
                case NA:
                    std::cout << "... ";
                    break;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::right;

    int count = 0;
    for (auto& r: rules) {
        std::cout << count++ << ") ";
        std::cout << r.category.sym << " -> ";
        for (auto& p: r.production) {
            std::cout << p.sym << " ";
        }
        std::cout << std::endl;
    }

    std::vector<Symbol> tokens;
    std::vector<int> stack;
    stack.push_back(0);

    std::string syntax = "$ d d c";
    std::stringstream file(syntax);
    std::string token;
    while (file >> token) {
        if (token == "c") {
            tokens.push_back(c);
        } else if (token == "d") {
            tokens.push_back(d);
        } else if (token == "$") {
            tokens.push_back(END);
        }
    }

    std::vector<int> out;

    bool accepted = false;
    while (!accepted) {
        auto& curr = tokens.back();
        auto& act = table[stack.back()][curr.id];
        switch (act.first) {
            case REDUCE:
                for (int i = 0; i < rules[act.second].production.size(); i++) {
                    stack.pop_back();
                }
                stack.push_back(table[stack.back()][rules[act.second].category.id].second);
                out.push_back(act.second);
                break;
            case SHIFT:
                tokens.pop_back();
                stack.push_back(act.second);
                break;
            case ACCEPT:
                accepted = true;
                break;
            case GOTO:
                std::cout << "err (goto)\n";
                break;
            case NA:
                std::cout << "err (na)\n";
                break;
        }
    }

    std::cout << std::endl;
    for (auto& e: out) {
        std::cout << e << " ";
    }
}

struct Del {
    void operator()(GLFWwindow* window) {
        std::cout << "Destroying window!" << std::endl;
        glfwDestroyWindow(window);
    }
};

void glfwErrorCallback(int code, const char* string) {
    std::cout << "Error " << std::hex << code << ": " << string << std::endl;
}

int main(const int argCount, const char* argVector[]) {
    glfwSetErrorCallback(glfwErrorCallback);

    glfwInit();

    std::unique_ptr<GLFWwindow, Del> window(glfwCreateWindow(480, 480, "", nullptr, nullptr));
    if (!window) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window.get());

    auto gladLoaderLoadSuccessful = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    if (!gladLoaderLoadSuccessful) {
        std::cout << "Failed to initialize OpenGL context." << std::endl;
        return -1;
    }

    glViewport(0, 0, 480, 480);

    test();

    while (!glfwWindowShouldClose(window.get())) {
        glfwPollEvents();

        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window.get());
    }
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