//
// Created by HAL-9000 on 18/05/2018.
//

#include "InstructionInfo.h"

const std::string &InstructionInfo::getNomeSimbolico() const {
    return nomeSimbolico;
}

void InstructionInfo::setNomeSimbolico(const std::string &nomeSimbolico) {
    InstructionInfo::nomeSimbolico = nomeSimbolico;
}

size_t InstructionInfo::getTamanho() const {
    return tamanho;
}

void InstructionInfo::setTamanho(size_t tamanho) {
    InstructionInfo::tamanho = tamanho;
}

int InstructionInfo::getNumeroDeOperandos() const {
    return numeroDeOperandos;
}

void InstructionInfo::setNumeroDeOperandos(int numeroDeOperandos) {
    InstructionInfo::numeroDeOperandos = numeroDeOperandos;
}
