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
    // Arquivo de saída .pre

    bool isSectionText = false;
    bool isSectionData = false;
    int contadorPosicao = 0;
    ParseLib parseLib("lalala");
    TabelaLib tabelaLib;

    int fatorDeCorrecao = 0;

    for(int i=0; i < tokensDaLinha.size(); i++) {
        Montador::TokensDaLinha copiaLinha = tokensDaLinha[i];
        if(tokensDaLinha[i].operacao == "section"){
            // Checa se o operando é .text ou .data
                if(tokensDaLinha[i].operando[0] == "text"){
                    // Ativa o booleano que impede que diretivas como EQU estejam no text
                   isSectionText = true;
                } else if(tokensDaLinha[i].operando[0] == "data"){
                    if(!isSectionText){
                        //Erro: seção data veio antes da seção teste
                        ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Seção DATA veio antes da seção TEXT", "Sintático");
                    } else{
                        isSectionText = false;
                        isSectionData = true;
                    }
                }
        }

        if(tokensDaLinha[i].operacao == "equ" && (!isSectionText && !isSectionData)){
            // Adicionar a label à Tabela de Símbolos

            InfoDeDiretivas infoDeDiretivas = tabelaLib.getDiretiva(tokensDaLinha[i].operacao);
            int valor = parseLib.converteOperandoParaInteiro(tokensDaLinha[i].operando[0]);
            if(!tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operacao)) {
                tabelaLib.insereSimboloNaTabelaDeSimbolos(tokensDaLinha[i].label, InfoDeSimbolo(contadorPosicao, -1, true, valor));
                // Após a inserção, removemos essa linha, pois ela não entrará no arquivo de saída:
                tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin()+(i-fatorDeCorrecao));
                fatorDeCorrecao++;

            } else {
                ErrorLib elib(tokensDaLinha[i].numeroDaLinha, "Redeclaração de flag para EQU", "Léxico");
            }

        } else if(tokensDaLinha[i].operacao == "equ" && (isSectionText || isSectionData)){
            ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Operando equ dentro da section text ou data!", "Sintático");
        }

        if(tokensDaLinha[i].operacao == "if" && isSectionText) {
            tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin()+(i-fatorDeCorrecao));
            std::cout << "Linha: " << tokensDaLinha[i].numeroDaLinha << std::endl;
            // Procura na tabela de símbolos se o símbolo já foi definido anteriormente
            if(tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operando[0])){
                InfoDeSimbolo infoSimbolo = tabelaLib.obtemSimboloNaTabelaDeSimbolos(tokensDaLinha[i].operando[0]);
                if(infoSimbolo.valorConstante != 1) {
                    // Se a comparação for diferente de 1, a próxima linha não será executada, logo podemos removê-la
                    // dos tokens de linha do resultado
                    tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin()+(i-fatorDeCorrecao));
                    fatorDeCorrecao+=2;
                } else {
                    fatorDeCorrecao++;
                }
            }
        }
    }
    // Recalculamos o número de linhas:
    for(int it = 0; it < tokensDaLinhaSaida.size(); it++) {
        tokensDaLinhaSaida[it].numeroDaLinha = it+1;
    }
    setTokensDaLinhaList(tokensDaLinhaSaida);
    gerarCodigoDeSaidaDiretivas(nomeArquivoSaida);
}

void PreProcessamento::gerarCodigoDeSaidaDiretivas(std::string nomeArquivoDeSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    std::ofstream arquivoSaida(nomeArquivoDeSaida+".pre");
    for (const auto &e : tokensDaLinha){
        if(e.label!="") {
            arquivoSaida << e.label << ": ";
        }
        arquivoSaida << e.operacao << " ";
        if(e.operando.size() != 0){
            if(e.operando.size() == 1){
                arquivoSaida << e.operando[0] << "\n";
            } else { // tamanho 2(Copy)
                arquivoSaida << e.operando[1] << "," << e.operando[2] << "\n";
            }
        }
    }
    arquivoSaida.close();
}

void PreProcessamento::processarDiretivasEMacros(std::string nomeArquivoSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    // Cópia das linhas tokenizadas do código, como se o if for falso a linha seguinte não será processada,
    // ela será removida da saída
    std::vector<Montador::TokensDaLinha> tokensDaLinhaSaida = getTokensDaLinhaList();
    // Arquivo de saída .pre

    bool isSectionText = false;
    bool isSectionData = false;
    int contadorPosicao = 0;
    ParseLib parseLib("lalala");
    TabelaLib tabelaLib;

    int fatorDeCorrecao = 0;

    for(int i=0; i < tokensDaLinha.size(); i++) {
        Montador::TokensDaLinha copiaLinha = tokensDaLinha[i];
        if(tokensDaLinha[i].operacao == "section"){
            // Checa se o operando é .text ou .data
            if(tokensDaLinha[i].operando[0] == "text"){
                // Ativa o booleano que impede que diretivas como EQU estejam no text
                isSectionText = true;
            } else if(tokensDaLinha[i].operando[0] == "data"){
                if(!isSectionText){
                    //Erro: seção data veio antes da seção teste
                    ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Seção DATA veio antes da seção TEXT", "Sintático");
                } else{
                    isSectionText = false;
                    isSectionData = true;
                }
            }
        }

        if(tokensDaLinha[i].operacao == "equ" && (!isSectionText && !isSectionData)){
            // Adicionar a label à Tabela de Símbolos

            InfoDeDiretivas infoDeDiretivas = tabelaLib.getDiretiva(tokensDaLinha[i].operacao);
            int valor = parseLib.converteOperandoParaInteiro(tokensDaLinha[i].operando[0]);
            if(!tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operacao)) {
                tabelaLib.insereSimboloNaTabelaDeSimbolos(tokensDaLinha[i].label, InfoDeSimbolo(contadorPosicao, -1, true, valor));
                // Após a inserção, removemos essa linha, pois ela não entrará no arquivo de saída:
                tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin()+(i-fatorDeCorrecao));
                fatorDeCorrecao++;

            } else {
                ErrorLib elib(tokensDaLinha[i].numeroDaLinha, "Redeclaração de flag para EQU", "Léxico");
            }

        } else if(tokensDaLinha[i].operacao == "equ" && (isSectionText || isSectionData)){
            ErrorLib errorLib(tokensDaLinha[i].numeroDaLinha, "Operando equ dentro da section text ou data!", "Sintático");
        }

        if(tokensDaLinha[i].operacao == "if" && isSectionText) {
            tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin()+(i-fatorDeCorrecao));
            std::cout << "Linha: " << tokensDaLinha[i].numeroDaLinha << std::endl;
            // Procura na tabela de símbolos se o símbolo já foi definido anteriormente
            if(tabelaLib.rotuloJaExistenteNaTabelaDeSimbolos(tokensDaLinha[i].operando[0])){
                InfoDeSimbolo infoSimbolo = tabelaLib.obtemSimboloNaTabelaDeSimbolos(tokensDaLinha[i].operando[0]);
                if(infoSimbolo.valorConstante != 1) {
                    // Se a comparação for diferente de 1, a próxima linha não será executada, logo podemos removê-la
                    // dos tokens de linha do resultado
                    tokensDaLinhaSaida.erase(tokensDaLinhaSaida.begin()+(i-fatorDeCorrecao));
                    fatorDeCorrecao+=2;
                } else {
                    fatorDeCorrecao++;
                }
            }
        }
    }
    // Recalculamos o número de linhas:
    for(int it = 0; it < tokensDaLinhaSaida.size(); it++) {
        tokensDaLinhaSaida[it].numeroDaLinha = it+1;
    }
    setTokensDaLinhaList(tokensDaLinhaSaida);
    gerarCodigoDeSaidaMacros(nomeArquivoSaida);
}

void PreProcessamento::gerarCodigoDeSaidaMacros(std::string nomeArquivoDeSaida) {
    std::vector<Montador::TokensDaLinha> tokensDaLinha = getTokensDaLinhaList();
    std::ofstream arquivoSaida(nomeArquivoDeSaida+".mcr");
    for (const auto &e : tokensDaLinha){
        if(e.label!="") {
            arquivoSaida << e.label << ": ";
        }
        arquivoSaida << e.operacao << " ";
        if(e.operando.size() != 0){
            if(e.operando.size() == 1){
                arquivoSaida << e.operando[0] << "\n";
            } else { // tamanho 2(Copy)
                arquivoSaida << e.operando[1] << "," << e.operando[2] << "\n";
            }
        }
    }
    arquivoSaida.close();
}
