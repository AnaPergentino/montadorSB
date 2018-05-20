//
// Created by HAL-9000 on 20/05/2018.
//

#ifndef T1_SB_MONTADOR_H
#define T1_SB_MONTADOR_H


#include <string>
#include <vector>

class Montador {
    struct TokensDaLinha {
        std::string label;
        std::string operacao;
        std::vector<std::string> operando;

        TokensDaLinha(const std::string &label, const std::string &operacao, const std::vector<std::string> &operando);
    };

    void primeiraPassagem();

};


#endif //T1_SB_MONTADOR_H
