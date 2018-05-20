//
// Created by HAL-9000 on 18/05/2018.
//

#include "PreProcessamento.h"
#include <utility>

void PreProcessamento::processarMacros(std::ifstream arquivo) {
    if(arquivo.is_open()) {
        // Nessa etapa, precisamos processar apenas as macros
    }
}

void PreProcessamento::processarDiretivas(std::string fileString) {
}

const std::string &PreProcessamento::getFileString() const {
    return fileString;
}

void PreProcessamento::setFileString(const std::string &fileString) {
    PreProcessamento::fileString = fileString;
}

PreProcessamento::PreProcessamento(const std::string &fileString) : fileString(fileString) {}
