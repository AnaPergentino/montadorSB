#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <fstream>
#include <sstream>
#include "CommandOperations.h"
#include "PreProcessamento.h"
#include "ParseLib.h"

/**
 * Checa a extensão do arquivo digitado pelo usuário no segundo parâmetro
 * Se a extensão for .asm, retorna o nome original
 * Caso a extensão não tenha sido colocada, a função coloca o .asm no final do arquivo
 * @param fileName
 */

int main(int argc, char *argv[]) {

#ifdef DEBUG
    // Se estamos em modo de debug, o projeto receberá um arquivo teste para verificar seu funcionamento.
    argv[1] = const_cast<char *>("-p");
    argv[2] = const_cast<char *>("files/entrada_teste");
    argv[3] = const_cast<char *>("files/saida_teste");
    argc = 4;
    std::cout << "Numero de argumentos: " << argc << "\n";
    for (int i = 0; i < argc; ++i) {
        std::cout << "Argumentos: " << i << ":" << argv[i] << "\n";
    }
#else
#endif
    if (argc < 4) {
        std::cout << "Erro! Sao necessarios 3 argumentos na linha de comando na seguinte ordem: \n1.Tipo de operacao\n"
                     "2.Caminho do arquivo de entrada\n3.Caminho do arquivo de saida";
        return 0;
    }
    // Tratamento dos argumentos de operação:
    CommandOperations comando(argv[1], argv[2], argv[3]);
    if(comando.comandoEValido()){
        // DEBUG
//        std::cout << "Operacao: " << comando.getOperacao() << "\n";
//        std::cout << "Arquivo de entrada: " << comando.getArquivoEntrada() << "\n";
//        std::cout << "Arquivo de saida: " << comando.getArquivoSaida() << "\n";
        // END DEBUG

        // Abre o arquivo e lê linha a linha:
        std::fstream fileAssembly(comando.getArquivoEntrada());

        if  (fileAssembly.is_open()) {
        std::stringstream stringstream;
            stringstream << fileAssembly.rdbuf();
            ParseLib parseLib(stringstream.str());
            parseLib.preparaCodigo(comando.getOperacao(), comando.getArquivoEntrada(), comando.getArquivoSaida());
            fileAssembly.close();
        }
        else {
            std::cout << "Erro! Arquivo inexistente!";
            return -1;
        }
    };

    std::cout << "Execucao bem sucedida! Aperte qualquer botao para sair" << std::endl;
    getchar();
    return 0;
}

