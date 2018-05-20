//
// Created by HAL-9000 on 20/05/2018.
//

#include <iostream>
#include "ErrorLib.h"

/**
 * Se esse construtor for chamado, o erro irá redirecionar o usuário para o fim do programa após uma mensagem de erro.
 * @param linhaDoErro
 * @param mensagemDeErro
 * @param tipoErro
 */
ErrorLib::ErrorLib(int linhaDoErro, const std::string &mensagemDeErro, const std::string &tipoErro) : linhaDoErro(
        linhaDoErro), mensagemDeErro(mensagemDeErro), tipoErro(tipoErro) {
    std::cout << "Erro!" << mensagemDeErro << " Na linha:" << linhaDoErro << " Tipo de Erro" << tipoErro << std::endl;
    exit(1);
}
