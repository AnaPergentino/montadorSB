//
// Created by HAL-9000 on 17/05/2018.
//

#include <iostream>
#include <regex>
#include <sstream>
#include "ParseLib.h"
#include "TabelaLib.h"
#include "ErrorLib.h"
#include "Montador.h"
#include "PreProcessamento.h"

std::string ParseLib::parseLabel(std::string linha) {
    std::string label;
    // Provavelmente a label virá no seguinte formato:
    // LABEL:
    // Então o programa precisa separar a string dos dois pontos e retornar a label original, a ser inserida na
    // tabela de símbolos.
    std::string::size_type posicaoLabel = linha.find(':');
    if (posicaoLabel != std::string::npos) {
        label = linha.substr(0, posicaoLabel);
        // DEBUG
//        std::cout << "parseLabel label: " << label << std::endl;
        return label;
    } else {
//        std::cout << "Nao existem labels nessa linha!" << std::endl;
        return "";
    }
}

std::string ParseLib::parseOperacao(std::string linha, bool hasLabel) {
    // Uma operacao é uma palavra após uma label(caso a linha tenha label)
    // ou a primeira palavra caso a linha não tenha label
    // Em caso de label, separamos a linha por espaços em brancos (o código garante que, nesse ponto, a label e a
    // operação são separados por pelo menos um espaço) e obtemos a primeira palavra:
    std::vector<std::string> tokensLinhas;
    std::istringstream iss(linha);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(tokensLinhas));

    if (hasLabel) {
//        std::cout << "Linha Operacao/Diretiva:" << tokensLinhas[1] << std::endl;
        return tokensLinhas[1];
    } else {
//        std::cout << "Linha Operacao/Diretiva:" << tokensLinhas[0] << std::endl;
        return tokensLinhas[0];
    }
}

std::vector<std::string> ParseLib::parseOperando(std::string linha, int numeroDeOperandos, bool hasLabel) {
    // Um operando vem logo após a operação, mas se a linha tiver label, a posição do operando pode mudar, logo passamos
    // isso como parâmetro também
    // Obtém os tokens da linha separadamente
    std::vector<std::string> tokensLinhas;
    std::istringstream iss(linha);
    std::copy(std::istream_iterator<std::string>(iss),
              std::istream_iterator<std::string>(),
              std::back_inserter(tokensLinhas));
    // se tiver label, a token[0] é o label e token[1] é a operação:
    std::vector<std::string> operandosString;
    std::vector<int> operandos;

    if (hasLabel) {
        if (numeroDeOperandos == 0) { // Caso de algumas diretivas
            if (operandosString.size() > 2) {
                ErrorLib errorLib(contadorLinha, "Número incorreto de operandos!", "Sintático");
            }
        } else if (numeroDeOperandos == 1) { // Todos os outros
            operandosString.push_back(tokensLinhas[2]);
        } else if (numeroDeOperandos == 2) { // Caso do COPY
            //Os dois estão juntos e separados por uma vírgula, logo é só fazer uma substr para cada operando
            std::string op1, op2;
            op1 = tokensLinhas[2].substr(0, tokensLinhas[2].find(','));
            op2 = tokensLinhas[2].substr(tokensLinhas[2].find(','), tokensLinhas[2].size());
            operandosString.push_back(op1);
            operandosString.push_back(op2);
        } else if (numeroDeOperandos == -1) { // Caso do SPACE e MACRO
            // Macro obrigatoriamente tem uma label antes, para identificá-la
            // Faremos uma iteração pela string que pegará o número de parâmetros separados por vírgula
            // Para diferenciar, precisaremos pegar o nome da operação, que aqui está, invariavelmente,
            // em tokensLinhas[1].
            std::string operacao = tokensLinhas[1];
            if (operacao == "macro") {
                // Os operandos começam(se existirem) a partir do tokensLinhas[2], e estão separados por vírgula e espaço
                std::string::size_type tamanho = tokensLinhas.size();
                if (tamanho > 2) {
                    for (unsigned int it = 2; it < tamanho; it++) {
                        std::string::size_type posicaoVirgula = tokensLinhas[it].find(',');
                        if (posicaoVirgula != std::string::npos) {
                            std::string operandoSemVirgula = tokensLinhas[it].substr(0, posicaoVirgula);
                            operandosString.push_back(operandoSemVirgula);
                        } else {
                            operandosString.push_back(tokensLinhas[it]);
                        }
                    }
                }
            }
            if (operacao == "space") {
                if (tokensLinhas.size() == 3) {
                    operandosString.push_back(tokensLinhas[2]);
                } else { operandosString.push_back("1"); }
            }
        }
        // Caso sem label, tokens relativos diminuem em uma posição no vetor
    } else {
        if (numeroDeOperandos == 0) { // Caso de algumas diretivas

        } else if (numeroDeOperandos == 1) { // Todos os outros
            operandosString.push_back(tokensLinhas[1]);
        } else if (numeroDeOperandos == 2) { // Caso do COPY
            //Os dois estão juntos e separados por uma vírgula, logo é só fazer uma substr para cada operando
            std::string op1, op2;
            std::string::size_type virgulaPos = tokensLinhas[1].find(',');
            if (tokensLinhas[virgulaPos + 1] == " ") {
                ErrorLib errorLib(contadorLinha, "Operandos de COPY estão separados com uma vírgula com espaço!",
                                  "Léxico");
            }
            op1 = tokensLinhas[1].substr(0, tokensLinhas[1].find(','));
            op2 = tokensLinhas[1].substr(tokensLinhas[1].find(','), tokensLinhas[1].size());
            operandosString.push_back(op1);
            operandosString.push_back(op2);
        } else if (numeroDeOperandos == -1) { // Caso do SPACE e chamada de macro
            std::string operacao = tokensLinhas[0];
            int tamanho = tokensLinhas.size();
            if (operacao == "space") {
                if (tokensLinhas.size() == 2) {
                    operandosString.push_back(tokensLinhas[1]);
                } else { operandosString.push_back("1"); }
            } else { // Chamada de macros
                if (tamanho > 1) {
                    for (int it = 1; it < tamanho; it++) {
                        std::string::size_type posicaoVirgula = tokensLinhas[it].find(',');
                        if (posicaoVirgula != std::string::npos) {
                            std::string operandoSemVirgula = tokensLinhas[it].substr(0, posicaoVirgula);
                            operandosString.push_back(operandoSemVirgula);
                        } else {
                            operandosString.push_back(tokensLinhas[it]);
                        }
                    }
                }
            }
        }
    }
// TODO: Realocar essa parte do método em outro lugar
/*    std::regex operandosHexa("/(0x([0-9]+))/g");

    // Verifica nessa sessão se o operando em questão é um número(decimal ou hexadecimal), ou um label
    for (auto &i : operandosString) {
        if (is_number(i)) { // Caso de operando ser inteiro:
            // Converte para inteiro
            operandos.push_back(std::stoi(i));
        } else if (std::regex_match(i, operandosHexa)) { // Caso de operando ser hexadecimal:
            operandos.push_back(converteOperandoHexaParaInteiro(i));
        }
        //TODO:: Implementar essa condição
//        } else if (//Operando is label){ // Caso de operando ser label
    }*/

    return operandosString;
}

std::string ParseLib::removeComentarios(std::string linha) {
    // Comentários são definidos por um ponto e vírgula antes do comentário em si, então
    // separaremos a linha pelo delimitador ;
    // Verificamos antes se a linha possui comentários, e então fazemos a remoção de string
    std::string::size_type posicaoComentario = linha.find(';');
    if (posicaoComentario != std::string::npos) {
        std::string linhaSemComentarios = linha.substr(0, posicaoComentario);
        return linhaSemComentarios;
    } else {
        // Linha não possui comentários
        return linha;
    }
}

Montador::TokensDaLinha ParseLib::parseLinha(std::string linha, int linhaContador, int contadorPosicao) {
    TabelaLib tabelaLib;

    // Faz a separação dos tokens de uma linha, podemos ter 3 opções:
    // Linhas com um token(END, STOP)
    // Linhas com 3 tokens(COPY command ou LABEL + Operação)
    // Linhas com 4 tokens(LABEL+COPY)
    std::transform(linha.begin(), linha.end(), linha.begin(), ::tolower);
    printLinha(linha);
    linha = removeComentarios(linha);
    // Separa os elementos da linha, para isso, usamos um parse para obter cada token da linha
    // <Rótulo>: <Operação> <Operando>; <Comentários>
    // <Rótulo>
    // Para obtermos a label, executamos o método parseLabel, que retorna uma string com a label ou uma string vazia
    // quando não temos a label na linha
    std::string labelLinha = parseLabel(linha);
    //TODO: Realocar essa parte do método em outro lugar
//    if (!labelLinha.empty()) {
//        // Se a label não for vazia, precisamos adiciona-la na tabela de símbolos
//        // Mas antes, devemos checar se o simbolo já não existe
//        if (tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(labelLinha)) {
//            ErrorLib errorLib(linhaContador, "Redefinição: Rótulo já inexistente na tabela de símbolos", "Léxico");
//        } else {
//            tabelaLib.insereSimboloNaTabelaDeSimbolos(labelLinha, InfoDeSimbolo(-1,-1,false,-1));
//        }
//    }
    // <Operação>
    // Para obtermos a operação, simplesmente precisamos verificar se uma label foi declarada, caso não tenha sido, o
    // primeiro elemento daquela linha será a operação
    std::string labelOperacao = parseOperacao(linha, !labelLinha.empty());
    std::vector<std::string> labelOperandos;

    if (tabelaLib.isDiretiva(labelOperacao)) {
        InfoDeDiretivas infoDeDiretivas = tabelaLib.getDiretiva(labelOperacao);
        // DEBUG Diretiva
//        std::cout << "infos de diretiva: " << std::endl;
//        std::cout << "diretiva: " << infoDeDiretivas.diretivasDiretivas << std::endl;
//        std::cout << "/numero de operandos: " << infoDeDiretivas.numeroDeOperandos << std::endl;
//        std::cout << "tamanho: " << infoDeDiretivas.tamanho << std::endl ;
//        std::cout << "isPre: " << infoDeDiretivas.isPre << std::endl << std::endl;

        labelOperandos = parseOperando(linha, infoDeDiretivas.numeroDeOperandos, !labelLinha.empty());
        // DEBUG Operando
        for (auto &labelOperando : labelOperandos) {
//            std::cout << "Operando: " << labelOperando << std::endl;
        }
        if (infoDeDiretivas.tamanho == -1) {
        }
        //  TODO: Realocar essa parte do método em outro lugar
//        setContadorPosicao(contadorPosicao + infoDeDiretivas.tamanho);
    } else if (tabelaLib.isInstrucao(labelOperacao)) {
        InfoDeInstrucoes infoDeInstrucoes = tabelaLib.getInstrucao(labelOperacao);
        // DEBUG Instrução
//        std::cout << "Infos de instrucao: " << std::endl;
//        std::cout << "Instrucao: " << infoDeInstrucoes.opcodesInstrucoes << std::endl;
//        std::cout << "Numero de operandos: " << infoDeInstrucoes.numeroDeOperandos << std::endl;
//        std::cout << "Tamanho: " << infoDeInstrucoes.tamanho << std::endl << std::endl;
        labelOperandos = parseOperando(linha, infoDeInstrucoes.numeroDeOperandos, !labelLinha.empty());
        // DEBUG Operando
        for (auto &labelOperando : labelOperandos) {
//            std::cout << "Operando: " << labelOperando << std::endl;
        }
//        setContadorPosicao(contadorPosicao + infoDeInstrucoes.tamanho);
    } else {
        // Caso que o operando é um label declarado anteriormente, como aqui só fazemos a referência linha a linha, não
        // temos como guardar as labels já declaradas, então vamos assumir apenas que é uma macro e que tem operandos
        labelOperandos = parseOperando(linha, -1, !labelLinha.empty());
    }
//    TODO: Fazer checagem de operação em outro momento
//    else {
//        ErrorLib errorLib(linhaContador, "Operação inexistente", "Léxico");
//    }
    return Montador::TokensDaLinha(labelLinha, labelOperacao, labelOperandos, linhaContador);
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

std::string ParseLib::removeTabulacoes(std::string fileString) {
    fileString.erase(std::remove(fileString.begin(), fileString.end(), '\t'), fileString.end());
    return fileString;
}

std::vector<std::string> ParseLib::separaEmLinhas(std::string fileString) {
    std::vector<std::string> strings;
    const std::string delimiter = "\n";

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = fileString.find(delimiter, prev)) != std::string::npos) {
        strings.push_back(fileString.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(fileString.substr(prev));

    return strings;
}

void ParseLib::preparaCodigo(std::string operacao, const std::string &arquivoEntradaNome, const std::string &arquivoSaidaNome) {
    std::string arquivo = this->getArquivo();
    arquivo = this->removeTabulacoes(arquivo);
    arquivo = this->removeEspacosEmBrancoExtras(arquivo);
    setLinhasDoCodigo(this->separaEmLinhas(arquivo));
    std::vector<std::string> codeLines = getLinhasDoCodigo();
    int contadorLinha = 1;
    setContadorLinha(contadorLinha);
    int contadorPosicao = 0;
    std::vector<Montador::TokensDaLinha> listTokensDaLinha;
    setContadorPosicao(contadorPosicao);
    for (auto &codeLine : codeLines) {
        Montador::TokensDaLinha tokensDaLinha = parseLinha(codeLine, contadorLinha, contadorPosicao);
        listTokensDaLinha.push_back(tokensDaLinha);
        contadorLinha++;
    }
//    DEBUG
//    for (auto &i : listTokensDaLinha) {
//        std::cout << "Label: " << i.label << std::endl;
//        std::cout << "Operacao: " << i.operacao << std::endl;
//        for (auto it = i.operando.begin(); it != i.operando.end(); ++it) {
//            std::cout << "Operando: " << *it << std::endl;
//        }
//        std::cout << "Numero da Linha: " << i.numeroDaLinha << std::endl << std::endl;
//    }

    PreProcessamento preProcessamento(listTokensDaLinha);
    if(operacao == "-p") {
        preProcessamento.processarDiretivas(arquivoSaidaNome);
    } else if(operacao == "-m"){
        preProcessamento.processarDiretivasEMacros(arquivoSaidaNome);
    } else if(operacao == "-o"){
        preProcessamento.processarDiretivasEMacros(arquivoSaidaNome);
        preProcessamento.montarCodigo(arquivoSaidaNome);
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
//    std::cout << "Linha atual: " << linha << std::endl;

}

bool ParseLib::isParsingMacro() const {
    return parsingMacro;
}

void ParseLib::setParsingMacro(bool parsingMacro) {
    ParseLib::parsingMacro = parsingMacro;
}

int ParseLib::converteOperandoHexaParaInteiro(std::string operando) {
    return std::stoi(operando, nullptr, 16);
}

int ParseLib::converteOperandoParaInteiro(std::string operando) {
    return std::stoi(operando);
}

bool ParseLib::isSectionText() const {
    return sectionText;
}

void ParseLib::setSectionText(bool sectionText) {
    ParseLib::sectionText = sectionText;
}

bool ParseLib::isSectionData() const {
    return sectionData;
}

void ParseLib::setSectionData(bool sectionData) {
    ParseLib::sectionData = sectionData;
}

int ParseLib::getContadorPosicao() const {
    return contadorPosicao;
}

void ParseLib::setContadorPosicao(int contadorPosicao) {
    ParseLib::contadorPosicao = contadorPosicao;
}

int ParseLib::getContadorLinha() const {
    return contadorLinha;
}

void ParseLib::setContadorLinha(int contadorLinha) {
    ParseLib::contadorLinha = contadorLinha;
}

bool ParseLib::is_number(const std::string &s) {
    return false;
}




