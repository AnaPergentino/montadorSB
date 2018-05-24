//
// Created by HAL-9000 on 18/05/2018.
//

#include <algorithm>
#include "PreProcessamento.h"
#include "Montador.h"
#include "ParseLib.h"
#include "TabelaLib.h"
#include "ErrorLib.h"
#include <utility>
#include <iostream>
#include <unordered_map>
#include <map>
#include <sstream>
#include <cctype>

PreProcessamento::PreProcessamento(const std::vector<Montador::TokensDaLinha> &tokensDaLinhaList) : tokensDaLinhaList(
        tokensDaLinhaList) {}

const std::vector<Montador::TokensDaLinha> &PreProcessamento::getTokensDaLinhaList() const {
    return tokensDaLinhaList;
}

void PreProcessamento::setTokensDaLinhaList(const std::vector<Montador::TokensDaLinha> &tokensDaLinhaList) {
    PreProcessamento::tokensDaLinhaList = tokensDaLinhaList;
}

/**
 * Faz o pré-processamento indicado pela operação "-p"
 * @param nomeArquivoSaida
 */
void PreProcessamento::processarDiretivas(std::string nomeArquivoSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    // Cópia das linhas tokenizadas do código, como se o if for falso a linha seguinte não será processada,
    // ela será removida da saída
    std::vector<Montador::TokensDaLinha> tokensDaLinhaSaida = getTokensDaLinhaList();

    bool isSectionText = false;
    bool isSectionData = false;
    int contadorPosicao = 0;
    ParseLib parseLib("lalala");
    TabelaLib tabelaLib;

    int fatorDeCorrecao = 0;

    for (unsigned int i = 0; i < tokensDaLinha.size(); i++) {
        Montador::TokensDaLinha copiaLinha = tokensDaLinha[i];
        if (tokensDaLinha[i].operacao == "section") {
            // Checa se o operando é .text ou .data
            if (tokensDaLinha[i].operando[0] == "text") {
                // Ativa o booleano que impede que diretivas como EQU estejam no text
                isSectionText = true;
            } else if (tokensDaLinha[i].operando[0] == "data") {
                if (!isSectionText) {
                    //Erro: seção data veio antes da seção teste
                    ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Seção DATA veio antes da seção TEXT",
                                      "Sintático");
                } else {
                    isSectionText = false;
                    isSectionData = true;
                }
            }
        }

        if (tokensDaLinha[i].operacao == "equ" && (!isSectionText && !isSectionData)) {
            // Adicionar a label à Tabela de Símbolos

            InfoDeDiretivas infoDeDiretivas = tabelaLib.getDiretiva(tokensDaLinha[i].operacao);
            int valor = parseLib.converteOperandoParaInteiro(tokensDaLinha[i].operando[0]);
            if (!tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operacao)) {
                tabelaLib.insereSimboloNaTabelaDeSimbolos(tokensDaLinha[i].label,
                                                          InfoDeSimbolo(contadorPosicao, -1, true, valor));
                // Após a inserção, removemos essa linha, pois ela não entrará no arquivo de saída:
                tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin() + (i - fatorDeCorrecao));
                fatorDeCorrecao++;

            } else {
                ErrorLib elib(tokensDaLinha[i].numeroDaLinha, "Redeclaração de flag para EQU", "Léxico");
            }

        } else if (tokensDaLinha[i].operacao == "equ" && (isSectionText || isSectionData)) {
            ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Operando equ dentro da section text ou data!",
                              "Sintático");
        }

        if (tokensDaLinha[i].operacao == "if" && isSectionText) {
            tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin() + (i - fatorDeCorrecao));
//            std::cout << "Linha: " << tokensDaLinha[i].numeroDaLinha << std::endl;
            // Procura na tabela de símbolos se o símbolo já foi definido anteriormente
            if (tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operando[0])) {
                InfoDeSimbolo infoSimbolo = tabelaLib.obtemSimboloNaTabelaDeSimbolos(tokensDaLinha[i].operando[0]);
                if (infoSimbolo.valorConstante != 1) {
                    // Se a comparação for diferente de 1, a próxima linha não será executada, logo podemos removê-la
                    // dos tokens de linha do resultado
                    tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin() + (i - fatorDeCorrecao));
                    fatorDeCorrecao += 2;
                } else {
                    fatorDeCorrecao++;
                }
            }
        }
    }
    // Recalculamos o número de linhas:
    for (unsigned int it = 0; it < tokensDaLinhaSaida.size(); it++) {
        tokensDaLinhaSaida[it].numeroDaLinha = it + 1;
        // No mesmo loop, aproveitamos para fazer a substituição de valores caso as labels de diretivas estejam sendo
        // usadas como operandos
        for (unsigned int j = 0; j < tokensDaLinhaSaida[it].operando.size(); j++) {
            std::string operando = tokensDaLinhaSaida[it].operando[j];
            if (tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(operando)) {
                tokensDaLinha[it].operando[j] = std::to_string(
                        tabelaLib.obtemSimboloNaTabelaDeSimbolos(operando).valorConstante);
            }
        }
    }
    setTokensDaLinhaList(tokensDaLinhaSaida);
    gerarCodigoDeSaidaDiretivas(nomeArquivoSaida);
}

void PreProcessamento::gerarCodigoDeSaidaDiretivas(std::string nomeArquivoSaida) {
    gerarCodigoDeSaida(nomeArquivoSaida + ".pre");
}

void PreProcessamento::processarDiretivasEMacros(std::string nomeArquivoSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    // Cópia das linhas tokenizadas do código, como se o if for falso a linha seguinte não será processada,
    // ela será removida da saída
    std::vector<Montador::TokensDaLinha> tokensDaLinhaSaida = getTokensDaLinhaList();

    bool isSectionText = false;
    bool isSectionData = false;
    bool parsingMacro = false;
    std::string macroAtual;
    std::vector<std::string> operandosMacroAtual;
    std::vector<Montador::TokensDaLinha> macroParcial;
    int contadorPosicao = 0;
    ParseLib parseLib("lalala");
    TabelaLib tabelaLib;

    int fatorDeCorrecao = 0;
    int numDiretivasApagadas = 0;

    for (unsigned int i = 0; i < tokensDaLinha.size(); i++) {
        Montador::TokensDaLinha copiaLinha = tokensDaLinha[i];
        if (tokensDaLinha[i].operacao == "section") {
            // Checa se o operando é .text ou .data
            if (tokensDaLinha[i].operando[0] == "text") {
                // Ativa o booleano que impede que diretivas como EQU estejam no text
                isSectionText = true;
            } else if (tokensDaLinha[i].operando[0] == "data") {
                if (!isSectionText) {
                    //Erro: seção data veio antes da seção teste
                    ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Seção DATA veio antes da seção TEXT",
                                      "Sintático");
                } else {
                    isSectionText = false;
                    isSectionData = true;
                }
            }
        }

        if (parsingMacro) {
            macroParcial.push_back(tokensDaLinha[i]);
            if (tokensDaLinha[i].operacao == "endmacro") {
                macroParcial = redefineVariaveisDeMacro(macroAtual, macroParcial, operandosMacroAtual, tabelaLib);
                tabelaLib.insereMacroNaTabelaDeDefinicoes(macroAtual, InfoMacroDef(0, macroParcial));
                parsingMacro = false;
            }

        }

        if (tokensDaLinha[i].operacao == "macro") {
            if (!isSectionText) {
                ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "MACRO definida fora da section TEXT", "Sintático");
            } else {
                parsingMacro = true;
                // Aqui, faremos a referência da macro pelo seu nome, e não pela sua linha, pois essa parte ainda não se
                // preocupa com endereços absolutos, pois algumas linhas estão sendo eliminadas nessa seção do código, quando
                // tivermos o código por inteiro, podemos iterar com o contador de posição e o contador de linha corretamente
                macroAtual = tokensDaLinha[i].label;
                operandosMacroAtual = tokensDaLinha[i].operando;
                tabelaLib.insereNomeDaMacroNaTabelaDeNomes(
                        tokensDaLinha[i].label,
                        InfoMacroName(tokensDaLinha[i].operando.size(), 0));
            }
        }


        if (tokensDaLinha[i].operacao == "equ" && (!isSectionText && !isSectionData)) {
            // Adicionar a label à Tabela de Símbolos

            InfoDeDiretivas infoDeDiretivas = tabelaLib.getDiretiva(tokensDaLinha[i].operacao);
            int valor = parseLib.converteOperandoParaInteiro(tokensDaLinha[i].operando[0]);
            if (!tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operacao)) {
                tabelaLib.insereSimboloNaTabelaDeSimbolos(tokensDaLinha[i].label,
                                                          InfoDeSimbolo(valor, valor, true, valor));
                // Após a inserção, removemos essa linha, pois ela não entrará no arquivo de saída:
                tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin() + (i - fatorDeCorrecao));
                fatorDeCorrecao++;
                numDiretivasApagadas++;

            } else {
                ErrorLib elib(tokensDaLinha[i].numeroDaLinha, "Redeclaração de flag para EQU", "Léxico");
            }

        } else if (tokensDaLinha[i].operacao == "equ" && (isSectionText || isSectionData)) {
            ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Operando equ dentro da section text ou data!",
                              "Sintático");
        }

        if (tokensDaLinha[i].operacao == "if" && isSectionText) {
            tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin() + (i - fatorDeCorrecao));
            // Procura na tabela de símbolos se o símbolo já foi definido anteriormente
            if (tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operando[0])) {
                InfoDeSimbolo infoSimbolo = tabelaLib.obtemSimboloNaTabelaDeSimbolos(tokensDaLinha[i].operando[0]);
                if (infoSimbolo.valorConstante != 1) {
                    // Se a comparação for diferente de 1, a próxima linha não será executada, logo podemos removê-la
                    // dos tokens de linha do resultado
                    tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin() + (i - fatorDeCorrecao));
                    fatorDeCorrecao += 2;
                } else {
                    fatorDeCorrecao++;
                }
            }
        }

        if (tabelaLib.macroJaExistenteNaTabelaDeNomes(tokensDaLinha[i].operacao)) {
            // Expansão de macro
            // Verificamos na linha de chamada quais valores serão substituídos;
            InfoMacroName infoMacroName = tabelaLib.obtemInfoMacroNameNaTabelaDeNomes(tokensDaLinha[i].operacao);
            InfoMacroDef infoDefinicao = tabelaLib.obtemInfoMacroDefNaTabelaDeDefinicoes(tokensDaLinha[i].operacao);
            int numeroOperandos = infoMacroName.numeroDeArgumentos;
            // Criamos um map com o operando e seu #n respectivo
            std::map<std::string, std::string> mapVariaveis;
            if (tokensDaLinha[i].operando.size() == numeroOperandos) {
                for (int j = 0; j < numeroOperandos; j++) {
//                    std::cout << "Operandos: " << tokensDaLinha[i].operando[j] << std::endl;
                    mapVariaveis["#" + std::to_string(j)] = tokensDaLinha[i].operando[j];
                }
            }

            for (auto &k : macroParcial) {
                for (unsigned int m = 0; m < k.operando.size(); m++) {
//                    std::cout << "Macro parcial operando: " << k.operando[m] << std::endl;
                    for (int n = 0; n < numeroOperandos; n++) {
//                        std::cout << "Map variable: " << mapVariaveis.at("#" + std::to_string(n)) << std::endl;
                        if (k.operando[m] == "#" + std::to_string(n)) {
                            k.operando[m] = mapVariaveis.at("#" + std::to_string(m));
                        }
                    }
                }
            }

            // Depois, deletamos a linha atual e colocamos as linhas da MDT(exceto a última[endmacro])
            std::vector<Montador::TokensDaLinha> linhasParcial;
            linhasParcial.insert(linhasParcial.begin(), tokensDaLinhaSaida.begin(),
                                 tokensDaLinhaSaida.begin() + i - fatorDeCorrecao);
            // Insere a macro justamente na sua posição de declaração:
            linhasParcial.insert(linhasParcial.begin() + i - fatorDeCorrecao, macroParcial.begin(),
                                 macroParcial.end() - 1);
            linhasParcial.insert(linhasParcial.end(), tokensDaLinhaSaida.begin() + i - fatorDeCorrecao + 1,
                                 tokensDaLinhaSaida.end());
            tokensDaLinha.reserve(linhasParcial.size());
            fatorDeCorrecao = numDiretivasApagadas;
            tokensDaLinhaSaida = linhasParcial;
        }
    }

    // Recalculamos o número de linhas:
    for (unsigned int it = 0; it < tokensDaLinhaSaida.size(); it++) {
        tokensDaLinhaSaida[it].numeroDaLinha = it + 1;
        // No mesmo loop, aproveitamos para fazer a substituição de valores caso as labels de diretivas estejam sendo
        // usadas como operandos
        for (unsigned int j = 0; j < tokensDaLinhaSaida[it].operando.size(); j++) {
            std::string operando = tokensDaLinhaSaida[it].operando[j];
            if (!tabelaLib.isInstrucao(operando) && tabelaLib.isDiretiva(operando)) {
                if (tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(operando)) {
                    tokensDaLinha[it].operando[j] = std::to_string(
                            tabelaLib.obtemSimboloNaTabelaDeSimbolos(operando).valorConstante);
                }
            }
        }
    }
    setTokensDaLinhaList(tokensDaLinhaSaida);
    gerarCodigoDeSaidaMacros(nomeArquivoSaida);
}

void PreProcessamento::gerarCodigoDeSaidaMacros(std::string nomeArquivoSaida) {
    gerarCodigoDeSaida(nomeArquivoSaida + ".mcr");
}

void PreProcessamento::gerarCodigoDeSaida(std::string nomeArquivoSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    std::ofstream arquivoSaida(nomeArquivoSaida);
    for (const auto &e : tokensDaLinha) {
        if (e.label != "") {
            arquivoSaida << e.label << ": ";
        }
        arquivoSaida << e.operacao << " ";
        if (e.operando.size() != 0) {
            if (e.operando.size() == 1) {
                arquivoSaida << e.operando[0] << "\n";
            } else { // COPY E MACRO
                for (unsigned int j = 0; j < e.operando.size(); j++) {
                    if (j == e.operando.size() - 1) {
                        arquivoSaida << e.operando[j] << "\n";
                    } else {
                        arquivoSaida << e.operando[j] << ",";
                    }

                }
            }
        } else {
            arquivoSaida << "\n";
        }
    }
    arquivoSaida.close();
}

std::vector<Montador::TokensDaLinha> PreProcessamento::redefineVariaveisDeMacro(
        std::string nomeMacro,
        std::vector<Montador::TokensDaLinha> macroLinhas,
        std::vector<std::string> listaDeOperandosMacro,
        TabelaLib tabelaLib) {
    std::map<std::string, std::string> variaveis;
    for (unsigned int i = 0; i < listaDeOperandosMacro.size(); i++) {
        variaveis[listaDeOperandosMacro[i]] = "#" + std::to_string(i);
    }

    for (auto &macroLinha : macroLinhas) {
        if (!macroLinha.operando.empty()) {
            for (unsigned int k = 0; k < macroLinha.operando.size(); k++) {
                macroLinha.operando[k] = variaveis.at(macroLinha.operando[k]);
            }
        }
    }

    return macroLinhas;
}

/**
 * Aqui realizamos a montagem do código, que vai sofrer uma análise léxica
 */
void PreProcessamento::montarCodigo(std::string nomeArquivoSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    primeiraPassagem(tokensDaLinha, nomeArquivoSaida);
}

void PreProcessamento::primeiraPassagem(std::vector<Montador::TokensDaLinha> tokensDaLinha, std::string nomeArquivoSaida) {
    TabelaLib tabelaLib;
    int contadorLinha = 1;
    int contadorPosicao = 0;
    bool isSectionText = false;
    bool isSectionData = false;


    for (unsigned int i = 0; i < tokensDaLinha.size(); i++) {
        std::string label = tokensDaLinha[i].label;
        std::string operacao = tokensDaLinha[i].operacao;
        std::vector<std::string> operando = tokensDaLinha[i].operando;
        int numeroDaLinha = tokensDaLinha[i].numeroDaLinha;
        std::string::size_type numeroDeOperandos = operando.size();
        InfoDeInstrucoes infoDeInstrucoes;
        InfoDeDiretivas infoDeDiretivas;

        // Procura se label existe, caso exista, devemos procurar pelo label na tabela de simbolos
        // Rótulo:
        if (!label.empty()) {
            if (!tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(label)) {
                tabelaLib.insereSimboloNaTabelaDeSimbolos(label, InfoDeSimbolo(contadorPosicao, -1, false, -1));
            } else {
                ErrorLib errorLib(contadorLinha, "Redeclaração de rótulo " + label, "Léxico");
            }
        }

        // Operação
        // Se for instrução
        if (tabelaLib.isInstrucao(operacao)) {
            if (isSectionData) {
                ErrorLib errorLib(contadorLinha, "Instrucao em local incorreto!", "Léxico");
            }
            infoDeInstrucoes = tabelaLib.getInstrucao(operacao);
            // Checa número de operandos:
            if (numeroDeOperandos != infoDeInstrucoes.numeroDeOperandos) {
                ErrorLib errorLib(contadorLinha, "Número de operandos incorreto!", "Sintático");
            }
            contadorPosicao += infoDeInstrucoes.tamanho;
        } else if (tabelaLib.isDiretiva(operacao)) {
            infoDeDiretivas = tabelaLib.getDiretiva(operacao);
            // As diretivas IF e EQU já foram descartadas no processamento de diretivas
            // As macros já foram resolvidas no processamento de macros
            // Logo, só sobram 3 diretivas a serem resolvidas: SPACE CONST e SECTION
            if (operacao == "section") {
                contadorPosicao += infoDeDiretivas.tamanho;
                if (operando[0] == "text") {
                    isSectionText = true;
                } else if (operando[0] == "data") {
                    if (!isSectionText) {
                        ErrorLib errorLib(contadorLinha, "Section data declarada antes da seção text!", "Léxico");
                    } else {
                        isSectionText = false;
                        isSectionData = true;
                    }
                }
            }
            if (operacao == "space") {
                if (isSectionData) {
                    if (isOperandoNumero(operando[0])) {
                        int numeroOperando = converteStringParaInt(operando[0]);
                        tabelaLib.insereSimboloNaTabelaDeSimbolos(
                                label, InfoDeSimbolo(contadorPosicao, numeroOperando, false, 0));
                        contadorPosicao += numeroOperando;
                    } else {
                        ErrorLib errorLib(contadorLinha, "Operando incorreto para instrução SPACE", "Sintático");
                    }
                } else { ErrorLib errorLib(contadorLinha, "Instrução fora da section correta", "Léxico"); }
            }
            if (operacao == "const") {
                if (isSectionData) {
                    contadorPosicao += infoDeDiretivas.tamanho;
                    int constVal;
                    if (isOperandoNumero(operando[0])) {
                        constVal = converteStringParaInt(operando[0]);
                        tabelaLib.insereSimboloNaTabelaDeSimbolos(label,
                                                                  InfoDeSimbolo(contadorPosicao, 1, true, constVal));
                    } else if (isOperandoHexa(operando[0])) {
                        constVal = converteStringHexaParaInt(operando[0]);
                        tabelaLib.insereSimboloNaTabelaDeSimbolos(label,
                                                                  InfoDeSimbolo(contadorPosicao, 1, true, constVal));
                    }
                } else { ErrorLib errorLib(contadorLinha, "Instrução fora da section correta", "Léxico"); }
            }
        } else {
            ErrorLib errorLib(contadorLinha, "Operação não identificada!", "Léxico");
        }
        contadorLinha++;
    }

//    std::cout << "Fim da primeira passagem!" << std::endl;
//    showTabelaDeSimbolos();
    segundaPassagem(nomeArquivoSaida);
}

bool PreProcessamento::isOperandoNumero(std::string operando) {
    return !operando.empty() && std::find_if(operando.begin(),
                                             operando.end(), [](char c) { return !std::isdigit(c); }) == operando.end();
}

int PreProcessamento::converteStringParaInt(std::string operando) {
    return std::stoi(operando);
}

bool PreProcessamento::isOperandoHexa(std::string operando) {
    return operando.compare(0, 2, "0x") == 0
           && operando.size() > 2
           && operando.find_first_not_of("0123456789abcdefABCDEF", 2) == std::string::npos;
}

int PreProcessamento::converteStringHexaParaInt(std::string operando) {
    return std::stoi(operando, nullptr, 16);
}

void PreProcessamento::showTabelaDeSimbolos() {
    TabelaLib tabelaLib;
    std::map<std::string, InfoDeSimbolo> tabelaDeSimbolos = tabelaLib.getTabelaDeSimbolos();
    for (auto &tabelaDeSimbolo : tabelaDeSimbolos) {
//        std::cout << "Simbolo: " << tabelaDeSimbolo.first << std::endl;
//        std::cout << "Valor: " << tabelaDeSimbolo.second.endereco << std::endl;
    }

}


void PreProcessamento::segundaPassagem(std::string nomeArquivoSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    TabelaLib tabelaLib;
    int contadorLinha = 1;
    int contadorPosicao = 0;
    bool isSectionText = false;
    bool isSectionData = false;
    std::ofstream arquivoDeSaida(nomeArquivoSaida +".o");

    for (unsigned int i = 0; i < tokensDaLinha.size(); i++) {
        std::string label = tokensDaLinha[i].label;
        std::string operacao = tokensDaLinha[i].operacao;
        std::vector<std::string> operando = tokensDaLinha[i].operando;
        int numeroDaLinha = tokensDaLinha[i].numeroDaLinha;
        std::string::size_type numeroDeOperandos = operando.size();
        InfoDeInstrucoes infoDeInstrucoes;
        InfoDeDiretivas infoDeDiretivas;

        if (tabelaLib.isInstrucao(operacao)) {
            infoDeInstrucoes = tabelaLib.getInstrucao(operacao);
            for (int j = 0; j < numeroDeOperandos; j++) {
                int valor = 0;
                if (operando[j].find('+') != std::string::npos) {
                    std::string copia = operando[j];
                    operando[j] = operando[j].substr(0, operando[j].find('+'));
                    std::string temp = copia.substr(copia.find('+')+1, copia.size());
                    if(tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(temp)){
                        valor = tabelaLib.obtemSimboloNaTabelaDeSimbolos(temp).valorConstante;
                    } else {
                        valor = converteStringParaInt(temp);
//                        std::cout << "Valor: " << valor << std::endl;
                    }
                } else if (operando[j].find('-') != std::string::npos) {
                    std::string copia = operando[j];
                    operando[j] = operando[j].substr(0, operando[j].find('-'));
                    std::string temp = copia.substr(copia.find('-')+1, copia.size());
                    if(tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(temp)){
                        valor = tabelaLib.obtemSimboloNaTabelaDeSimbolos(temp).valorConstante;
                    } else {
                        valor = converteStringParaInt(temp);
//                        std::cout << "Valor: " << valor << std::endl;
                    }
                }
                if (!isOperandoNumero(operando[j])) {
                    if (tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(operando[j])) {
                        arquivoDeSaida << infoDeInstrucoes.opcodesInstrucoes << " ";
                        arquivoDeSaida << tabelaLib.obtemSimboloNaTabelaDeSimbolos(operando[j]).endereco+valor << " ";
                    }
                }
            }
        }
    }

    arquivoDeSaida.close();

}


