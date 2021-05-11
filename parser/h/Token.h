//
// Created by anton on 2021-05-07.
//

#ifndef BNFC_TOKEN_H
#define BNFC_TOKEN_H

#include <cstddef>
#include <memory>

namespace Parser {

    /// The end-of-file token, which is always present, always has the same id.
    constexpr int eofTokenId = 0;

     struct Token {
         const int id;
         const bool terminal;
         // TODO: Store actual data

         Token(const int id, const bool terminal) : id(id), terminal(terminal) {}

         bool operator==(const Token& other) const;
         bool operator!=(const Token& other) const;
     };

     struct TokenHasher {
         std::size_t operator()(const Token& token) const {
             return std::hash<int>()(token.id);
         }
     };
}
#endif //BNFC_TOKEN_H
