//
// Created by HAL-9000 on 16/05/2018.
//

#include <iostream>
#include <fstream>
#include "CommandOperations.h"

/**
 * Verifica se a operação é alguma das 3, caso contrário, retorna uma mensagem de erro e encerra o programa
 * @param operacao
 * @return 0 se operação não existir
 */

bool CommandOperations::verificaOperacao() {
    if (operacao == "-p") {
        //TODO: Operação de preprocessamento
    } else if (operacao == "-m") {
        //TODO: Operação de preprocessamento de macros
    } else if (operacao == "-o") {
        //TODO: Operação de montagem
    } else {
        std::cout << "Erro! Operacao nao identificada, por favor, use -p, -m ou -o\n";
        return false;
    }
}
/**
 * Verifica se extensão de arquivo é válida
 * Se for, retorna o parâmetro enviado, sem modificações
 * Caso contrário, retorna o nome de arquivo com ".asm" ao seu final
 * @param fileName
 * @return
 */
int CommandOperations::verificaExtensaodeArquivoDeEntrada() {
    // Se a string for maior que 0 e se ela não conter o ponto(indicador da extensão, então ele está sem extensão
    size_t extensaoPosicao = getArquivoEntrada().find_last_of('.');
    if (getArquivoEntrada().length() > 0) {
        if (extensaoPosicao != std::string::npos) {
            std::string extensao = getArquivoEntrada().substr(extensaoPosicao, getArquivoEntrada().length());
            if(extensao != ".asm") {
                std::cout << "Erro! apenas arquivos do tipo .asm são aceitos, insira outro arquivo e tente novamente\n";
                return -1;
            } else {
                // Caso não tenha extensão, é necessário criar um arquivo .pre com o mesmo conteúdo
                // O motivo disso é que a leitura de um arquivo sem extensão gera um erro de arquivo inexistente
                // Então precisamos criar um arquivo com o mesmo conteúdo, mas com extensão definida, para o caso do
                // projeto, usaremos a extensão .pre, que não altera o conteúdo do arquivo
                setFileNameApresentacao(getArquivoEntrada()+".asm");
            }
        } else {
            std::string filenameApresentacao = getArquivoEntrada();
            setFileNameApresentacao(filenameApresentacao+".asm");
        }
    }
}



/**
 * A princípio, o arquivo de saída deve ter um nome válido para arquivo, essa função verifica se o nome é válido
 */
bool CommandOperations::verificaArquivoDeSaida(){
    return arquivoSaida.length() > 0;

}

CommandOperations::CommandOperations(std::string operacao, std::string arquivoEntrada, std::string arquivoSaida){
    this->operacao = operacao;
    this->arquivoEntrada = arquivoEntrada;
    this->arquivoSaida = arquivoSaida;
    verificaExtensaodeArquivoDeEntrada();
}

bool CommandOperations::comandoEValido() {
    bool retorno(verificaOperacao() && verificaArquivoDeSaida());
    std::cout << "comandoEValido() retorno: " << retorno << "\n";
    return retorno;
}

bool CommandOperations::arquivoEValido(std::string file) {
    std::ifstream arquivo(file);
    std::cout << "arquivoEValido() retorno: " << file << ":\n" << arquivo.good() << "\n";
    return arquivo.good();
}

const std::string &CommandOperations::getArquivoEntrada() const {
    return arquivoEntrada;
}

void CommandOperations::setArquivoEntrada(const std::string &fileName) {
    CommandOperations::arquivoEntrada = fileName;
}

const std::string &CommandOperations::getOperacao() const {
    return operacao;
}

void CommandOperations::setOperacao(const std::string &operacao) {
    CommandOperations::operacao = operacao;
}

const std::string &CommandOperations::getArquivoSaida() const {
    return arquivoSaida;
}

void CommandOperations::setArquivoSaida(const std::string &arquivoSaida) {
    CommandOperations::arquivoSaida = arquivoSaida;
}

const std::string &CommandOperations::getFileNameApresentacao() const {
    return fileNameApresentacao;
}

void CommandOperations::setFileNameApresentacao(const std::string &fileNameApresentacao) {
    CommandOperations::fileNameApresentacao = fileNameApresentacao;
}
