//
// Created by HAL-9000 on 17/05/2018.
//

#include <iostream>
#include "ParseLib.h"
#include "TabelaLib.h"

std::string ParseLib::parseLabel(std::string strLabel) {
    std::string label;
    // Provavelmente a label virá no seguinte formato:
    // LABEL:
    // Então o programa precisa separar a string dos dois pontos e retornar a label original, a ser inserida na
    // tabela de símbolos.
    label = strLabel.substr(0, strLabel.find(':'));
    std::cout << "parseLabel label: " << label;
    return label;
}

std::string ParseLib::parseOperacao(std::string strOperacao) {
    return std::__cxx11::string();
}

std::string ParseLib::removeComentarios(std::string linha){
    // Comentários são definidos por um ponto e vírgula antes do comentário em si, então
    // separaremos a linha pelo delimitador ;
    // Verificamos antes se a linha possui comentários, e então fazemos a remoção de string
    unsigned int posicaoComentario = linha.find(';');
    if(posicaoComentario != std::string::npos) {
        std::string linhaSemComentarios = linha.substr(0, posicaoComentario);
        return linhaSemComentarios;
    }
    else {
        // Linha não possui comentários
        return linha;
    }
}

void ParseLib::parseLinha(ssize_t linha) {
    // Supondo que já foram removidos os espaços, linhas em branco e tabulações desnecessárias
    // Primeiramente, a linha vai ser separada por espaços, então f
}


std::string ParseLib::removeEspacosEmBrancoExtras(const std::string &fileString) {
    // Solução adaptada de https://stackoverflow.com/a/35302029
        std::string output;
        output.clear();  // unless you want to add at the end of existing sring...
        unique_copy (fileString.begin(),
                     fileString.end(),
                     std::back_insert_iterator<std::string>(output),
                     [](char a,char b){ return isspace(a) && isspace(b);});
        return output;
}

