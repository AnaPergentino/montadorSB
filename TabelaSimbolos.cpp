//
// Created by HAL-9000 on 18/05/2018.
//

#include "TabelaSimbolos.h"

TabelaSimbolos::TabelaSimbolos(std::string label, int endereco) {
    this->labelNome = label;
    this->labelEndereco = endereco;

}

const std::string &TabelaSimbolos::getLabelNome() const {
    return labelNome;
}

void TabelaSimbolos::setLabelNome(const std::string &labelNome) {
    TabelaSimbolos::labelNome = labelNome;
}

int TabelaSimbolos::getLabelEndereco() const {
    return labelEndereco;
}

void TabelaSimbolos::setLabelEndereco(int labelEndereco) {
    TabelaSimbolos::labelEndereco = labelEndereco;
}
