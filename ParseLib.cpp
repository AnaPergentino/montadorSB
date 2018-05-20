//
// Created by HAL-9000 on 17/05/2018.
//

#include <iostream>
#include <regex>
#include "ParseLib.h"
#include "TabelaLib.h"

std::string ParseLib::parseLabel(std::string strLabel) {
    std::string label;
    // Provavelmente a label virá no seguinte formato:
    // LABEL:
    // Então o programa precisa separar a string dos dois pontos e retornar a label original, a ser inserida na
    // tabela de símbolos.
    std::string::size_type posicaoLabel = strLabel.find(':');
    if(posicaoLabel != std::string::npos){
        label = strLabel.substr(0, posicaoLabel);
        // DEBUG
        std::cout << "parseLabel label: " << label << std::endl;
        return label;
    } else {
        std::cout << "Nao existem labels nessa linha!" << std::endl;
        return "";
    }
}

std::string ParseLib::parseOperacao(std::string strOperacao) {
    return std::__cxx11::string();
}

std::string ParseLib::removeComentarios(std::string linha) {
    // Comentários são definidos por um ponto e vírgula antes do comentário em si, então
    // separaremos a linha pelo delimitador ;
    // Verificamos antes se a linha possui comentários, e então fazemos a remoção de string
    unsigned int posicaoComentario = linha.find(';');
    if (posicaoComentario != std::string::npos) {
        std::string linhaSemComentarios = linha.substr(0, posicaoComentario);
        return linhaSemComentarios;
    } else {
        // Linha não possui comentários
        return linha;
    }
}

void ParseLib::parseLinha(std::string linha) {
    // Faz a separação dos tokens de uma linha, podemos ter 3 opções:
    // Linhas com um token(END, STOP)
    // Linhas com 3 tokens(COPY command ou LABEL + Operação)
    // Linhas com 4 tokens(LABEL+COPY)
    printLinha(linha);
    // Separa os elementos da linha, para isso, usamos

    std::string labelLinha = parseLabel(linha);

}


std::string ParseLib::removeEspacosEmBrancoExtras(const std::string &fileString) {
    // Solução adaptada de https://stackoverflow.com/a/35302029
    std::string output;
    output.clear();  // unless you want to add at the end of existing sring...
    unique_copy(fileString.begin(),
                fileString.end(),
                std::back_insert_iterator<std::string>(output),
                [](char a, char b) { return isspace(a) && isspace(b); });

    std::regex labelComPuloDeLinha("(.+:)(\n)");
    output = std::regex_replace(output, labelComPuloDeLinha, "$1 ");
    std::regex labelComLetraLogoAposDoisPontos("(.+:)(\\w)");
    output = std::regex_replace(output, labelComLetraLogoAposDoisPontos, "$1 $2");

    return output;
}

std::string ParseLib::removeTabulacoes(std::string fileString){
    fileString.erase(std::remove(fileString.begin(), fileString.end(), '\t'), fileString.end());
    return fileString;
}

std::vector<std::string> ParseLib::separaEmLinhas(std::string fileString)
{
    std::vector<std::string> strings;
    const std::string delimiter = "\n";

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = fileString.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(fileString.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(fileString.substr(prev));

    return strings;
}

void ParseLib::preparaCodigo() {
    std::string arquivo = this->getArquivo();
    arquivo = this->removeTabulacoes(arquivo);
    arquivo = this->removeEspacosEmBrancoExtras(arquivo);
    setLinhasDoCodigo(this->separaEmLinhas(arquivo));
    std::vector<std::string> codeLines = getLinhasDoCodigo();

    for(auto i = codeLines.begin(); i != codeLines.end(); ++i){
        parseLinha(*i);
    }
}

const std::vector<std::string> &ParseLib::getLinhasDoCodigo() const {
    return linhasDoCodigo;
}

void ParseLib::setLinhasDoCodigo(const std::vector<std::string> &linhasDoCodigo) {
    ParseLib::linhasDoCodigo = linhasDoCodigo;
}

ParseLib::ParseLib(const std::string &arquivo) : arquivo(arquivo) {
    this->arquivo = arquivo;
}

const std::string &ParseLib::getArquivo() const {
    return arquivo;
}

void ParseLib::setArquivo(const std::string &arquivo) {
    ParseLib::arquivo = arquivo;
}

void ParseLib::printLinha(std::string linha) {
    std::cout << "Linha atual: " << linha << std::endl;

}
