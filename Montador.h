//
// Created by HAL-9000 on 20/05/2018.
//

#ifndef T1_SB_MONTADOR_H
#define T1_SB_MONTADOR_H


#include <string>
#include <vector>

class Montador {
public:
    struct TokensDaLinha {
        std::string label;
        std::string operacao;
        std::vector<std::string> operando;
        // Dados adicionais para facilitar
        int numeroDaLinha;

        TokensDaLinha(const std::string &label, const std::string &operacao, const std::vector<std::string> &operando,
                      int numeroDaLinha);
    };

    std::vector<TokensDaLinha> listaDeTokensDoArquivo;

    void montarCodigo();
    void analiseLexica(std::vector<TokensDaLinha> listaDeTokensDoArquivo);
    void primeiraPassagem();

    void adicionarTokenDaLinha(TokensDaLinha linha);
};


#endif //T1_SB_MONTADOR_H
