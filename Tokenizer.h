//
// Created by HAL-9000 on 16/05/2018.
//

#ifndef T1_SB_TOKENIZER_H
#define T1_SB_TOKENIZER_H

#include <array>

using namespace std;
class Tokenizer {
public:
    Tokenizer(istream& in);
    const Token& current() const;
    const Token& next() const;
    bool eof() const;
};

struct Token{
    bool operator == (const string& v) const;
    bool operator != (const string& v) const;

    unsigned line;
    string value;
};
#endif //T1_SB_TOKENIZER_H
