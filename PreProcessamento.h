//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_PREPROCESSAMENTO_H
#define T1_SB_PREPROCESSAMENTO_H


#include <fstream>
#include "Montador.h"
#include "TabelaLib.h"
#include <unordered_map>

class PreProcessamento {
public:
    PreProcessamento(const std::vector<Montador::TokensDaLinha> &tokensDaLinhaList);

private:
    std::vector<Montador::TokensDaLinha> tokensDaLinhaList;
public:
    void processarDiretivasEMacros(std::string nomeArquivoDeSaida);
    void processarDiretivas(std::string nomeArquivoDeSaida);
    void gerarCodigoDeSaidaDiretivas(std::string nomeArquivoSaida);
    void gerarCodigoDeSaidaMacros(std::string nomeArquivoSaida);
    void gerarCodigoDeSaida(std::string nomeArquivoSaida);

    const std::vector<Montador::TokensDaLinha> &getTokensDaLinhaList() const;

    void setTokensDaLinhaList(const std::vector<Montador::TokensDaLinha> &tokensDaLinhaList);

    std::vector<Montador::TokensDaLinha>
    redefineVariaveisDeMacro(std::string nomeMacro,
                             std::vector<Montador::TokensDaLinha> macroLinhas,
                             std::vector<std::string> listaDeOperandosMacro, TabelaLib tabelaLib);



    // TODO: Se der tempo, mover isso para montador
    void montarCodigo();
    void primeiraPassagem(std::vector<Montador::TokensDaLinha> tokensDaLinha);
    void showTabelaDeSimbolos();
    void showCodigo();
    void segundaPassagem();
    void gerarCodigoObjeto();
    void analiseLexica();
    void analiseSintatica();

    bool isOperandoNumero(std::string operando);
    bool isOperandoHexa(std::string operando);

    int converteStringParaInt(std::string operando);
    int converteStringHexaParaInt(std::string operando);

    void segundaPassagem(std::string nomeArquivoSaida);

    void primeiraPassagem(std::vector<Montador::TokensDaLinha> tokensDaLinha, std::string nomeArquivoSaida);

    void montarCodigo(std::string nomeArquivoSaida);
};


#endif //T1_SB_PREPROCESSAMENTO_H
