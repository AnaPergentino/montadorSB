//
// Created by HAL-9000 on 20/05/2018.
//

#include "Montador.h"

Montador::TokensDaLinha::TokensDaLinha(const std::string &label, const std::string &operacao,
                                       const std::vector<std::string> &operando) : label(label), operacao(operacao),
                                                                                   operando(operando) {}
