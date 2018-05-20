//
// Created by HAL-9000 on 18/05/2018.
//

#include "TabelaLib.h"
#include <algorithm>
#include <unordered_map>
#include <iostream>

//void TabelaLib::montarTabelaDeDiretivas(std::vector tabelaDeSimbolos) {
//
//}

std::unordered_map <std::string, InfoDeInstrucoes> TabelaLib::TabelaDeInstrucoes{
        {"add", {1,2, opcodes::ADD}},
        {"sub", {1,2, opcodes::SUB}},
        {"mult",{1,2, opcodes::MULT}},
        {"div", {1,2, opcodes::DIV}},
        {"jmp", {1,2, opcodes::JMP}},
        {"jmpn", {1,2,opcodes::JMPN}},
        {"jmpp", {1,2, opcodes::JMPP}},
        {"jmpz", {1,2, opcodes::JMPZ}},
        {"copy", {2,3, opcodes::COPY}},
        {"load", {1,2, opcodes::LOAD}},
        {"store", {1,2,opcodes::STORE}},
        {"input", {1,2, opcodes::INPUT}},
        {"output", {1,2,opcodes::OUTPUT}},
        {"stop", {0,1, opcodes::STOP}}
};

std::unordered_map<std::string, InfoDeDiretivas> TabelaLib::TabelaDeDiretivas{
        {"section", {1,0, SECTION, bool(false)}},
        {"space", {-1,-1, SPACE, bool(false)}},
        {"const" , {1,1, CONST, bool(false)}},
        {"equ", {1,0,EQU, bool(true)}},
        {"if", {1,0, IF, bool(true)}},
        {"macro", {0,0, MACRO, bool(false)}},
        {"endmacro", {0,0, ENDMACRO, bool(false)}}
};

bool TabelaLib::isDiretiva(std::string operacao) {
    // Obtém os valores da lista de diretiva e verifica pelo valor da key se é uma diretiva
    for(auto i : TabelaLib::TabelaDeDiretivas) {
        if (i.first == operacao){
            return i.first == operacao;
        };
    }
}

InfoDeDiretivas TabelaLib::getDiretiva(std::string operacao) {
    return TabelaLib::TabelaDeDiretivas.at(operacao);
}

InfoDeInstrucoes TabelaLib::getInstrucao(std::string operacao) {
    return TabelaLib::TabelaDeInstrucoes.at(operacao);
}

bool TabelaLib::isInstrucao(std::string operacao) {
    for(auto i: TabelaLib::TabelaDeInstrucoes){
        if(operacao == i.first){
            return i.first == operacao;
        }
    }
}

//void TabelaLib::insereSimboloNaTabelaDeSimbolos(std::string key, InfoDeSimbolo infoDeSimbolo) {
//    TabelaLib::TabelaDeSimbolos.insert(std::make_pair(key, infoDeSimbolo));
//}

//bool TabelaLib::rotuloJaExistenteNaTabelaDeSimbolos(std::string rotulo) {
//    return !(TabelaDeSimbolos.find(rotulo) == TabelaDeSimbolos.end());
//}
