//
// Created by HAL-9000 on 17/05/2018.
//

#ifndef T1_SB_LABELPARSE_H
#define T1_SB_LABELPARSE_H


#include <string>
#include "TabelaSimbolos.h"

class ParseLib{
protected:
    std::vector<std::string> linhasDoCodigo;
    std::string arquivo;
public:

    ParseLib(const std::string &arquivo);

    std::string parseLabel(std::string strLabel);
    std::string parseOperacao(std::string strOperacao);
    std::string parseOperando(std::string strOperando);

    void parseLinha(std::string linha);

    const std::vector<std::string> &getLinhasDoCodigo() const;

    void setLinhasDoCodigo(const std::vector<std::string> &linhasDoCodigo);

    void colocarLabelEmTabelaDeSimbolos(TabelaSimbolos tabelaSimbolos, std::string label);

    std::string removeEspacosEmBrancoExtras(const std::string &fileString);
    std::string removeTabulacoes(std::string linha);
    std::vector<std::string> separaEmLinhas(std::string fileString);
    std::string removeComentarios(std::string linha);
    void preparaCodigo();

    const std::string &getArquivo() const;

    void setArquivo(const std::string &arquivo);

    void printLinha(std::string linha);


};

#endif //T1_SB_LABELPARSE_H
