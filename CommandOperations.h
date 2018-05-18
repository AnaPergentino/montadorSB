//
// Created by HAL-9000 on 16/05/2018.
//

#ifndef T1_SB_FILEOPERATIONS_H
#define T1_SB_FILEOPERATIONS_H


#include <string>

class CommandOperations {
public:
    CommandOperations(std::string operacao, std::string arquivoEntrada, std::string arquivoSaida);

    bool verificaOperacao();
    int verificaExtensaodeArquivoDeEntrada();

    bool comandoEValido();
    bool arquivoEValido(std::string file);

    const std::string& getArquivoEntrada() const;
    std::ifstream getFile();

    void setArquivoEntrada(const std::string &fileName);

    const std::string &getOperacao() const;

    void setOperacao(const std::string &operacao);

    const std::string &getArquivoSaida() const;

    void setArquivoSaida(const std::string &arquivoSaida);

    //TODO: Definir função:
    std::string gerarArquivoDeSaida();

    const std::string &getFileNameApresentacao() const;

    void setFileNameApresentacao(const std::string &fileNameApresentacao);

private:
    std::string arquivoEntrada;
    std::string operacao;
    std::string arquivoSaida;
    std::string fileNameApresentacao;

    bool verificaArquivoDeSaida();
};


#endif //T1_SB_FILEOPERATIONS_H
