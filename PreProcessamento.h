//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_PREPROCESSAMENTO_H
#define T1_SB_PREPROCESSAMENTO_H


#include <fstream>
#include "Montador.h"

class PreProcessamento {
public:
    PreProcessamento(const std::vector<Montador::TokensDaLinha> &tokensDaLinhaList);

private:
    std::vector<Montador::TokensDaLinha> tokensDaLinhaList;
public:
    void processarMacros(std::ifstream arquivo);
    void processarDiretivas(std::string nomeArquivoDeSaida);
    void gerarCodigoDeSaida(std::string nomeArquivoDeSaida);
    void montarCodigo(std::ifstream arquivo);

    const std::vector<Montador::TokensDaLinha> &getTokensDaLinhaList() const;

    void setTokensDaLinhaList(const std::vector<Montador::TokensDaLinha> &tokensDaLinhaList);

};


#endif //T1_SB_PREPROCESSAMENTO_H
