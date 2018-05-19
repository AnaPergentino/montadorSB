//
// Created by HAL-9000 on 17/05/2018.
//

#ifndef T1_SB_LABELPARSE_H
#define T1_SB_LABELPARSE_H


#include <string>
#include "TabelaSimbolos.h"

class ParseLib{
public:
    std::string parseLabel(std::string strLabel);
    std::string parseOperacao(std::string strOperacao);
    std::string parseOperando(std::string strOperando);

    void parseLinha(ssize_t linha);


    void colocarLabelEmTabelaDeSimbolos(TabelaSimbolos tabelaSimbolos, std::string label);

    std::string removeEspacosEmBrancoExtras(const std::string &fileString);
    std::string removeComentarios(std::string linha);
//    std::string remove
};


#endif //T1_SB_LABELPARSE_H
