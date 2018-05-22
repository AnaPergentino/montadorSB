//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_TABELADIRETIVAS_H
#define T1_SB_TABELADIRETIVAS_H


#include <string>
#include <vector>
#include <array>
#include <algorithm>
#include <unordered_map>

// Para essa classe precisamos definir 2 tabelas estáticas e uma dinâmica:
// 1. Tabela de Diretivas
// 2. Tabela de Instruções
// 3. Tabela de Símbolos

// Para isso, precisamos de um map para associar as diretivas e outro para as instruções
// Após isso, precisaremos dizer, para cada instrução ou diretiva:
// Operandos
// Tamanho

enum opcodes {
    ADD = 1,
    SUB,
    MULT,
    DIV,
    JMP,
    JMPN,
    JMPP,
    JMPZ,
    COPY,
    LOAD,
    STORE,
    INPUT,
    OUTPUT,
    STOP
};

enum diretivas {
    SECTION,
    SPACE,
    CONST,
    EQU,
    IF,
    MACRO,
    ENDMACRO,
};

struct InfoDeInstrucoes {
    int numeroDeOperandos;
    int tamanho;
    opcodes opcodesInstrucoes;
};

struct InfoDeDiretivas {
    int numeroDeOperandos;
    int tamanho;
    diretivas diretivasDiretivas; // nome horrível, aceito sugestoes
    bool isPre;
};

struct InfoDeSimbolo {
    int endereco; // -1 significa endereço indefinido
    int espaco; // -1 significa espaço indefinido

    bool isConstante; // false funcionará para
    int valorConstante; // Talvez não seja necessário, já que o .o não faz cálculos com o valor absoluto da constante
                        // -1 significa valor indefinido
                        InfoDeSimbolo(int endereco, int espaco, bool isConstante, int valorConstante);
};

class TabelaLib {
public:
    bool isDiretiva(std::string operacao);
    InfoDeDiretivas getDiretiva(std::string operacao);
    bool isInstrucao(std::string operacao);
    InfoDeInstrucoes getInstrucao(std::string operacao);

    void insereSimboloNaTabelaDeSimbolos(std::string, InfoDeSimbolo);
    InfoDeSimbolo obtemSimboloNaTabelaDeSimbolos(std::string id);
    bool rotuloJaExistenteNaTabelaDeSimbolos(std::string);

    const std::unordered_map<std::string, InfoDeSimbolo> &getTabelaDeSimbolos() const;

    void setTabelaDeSimbolos(const std::unordered_map<std::string, InfoDeSimbolo> &TabelaDeSimbolos);


private:
    // Todas são statics pois usarei em várias partes do código
    static std::unordered_map<std::string, InfoDeInstrucoes> TabelaDeInstrucoes;
    static std::unordered_map<std::string, InfoDeDiretivas> TabelaDeDiretivas;
    static std::unordered_map<std::string, InfoDeSimbolo> TabelaDeSimbolos;
    std::unordered_map<std::string, int> MacroDefinitionTable;
    std::unordered_map<std::string, int> MacroNameTable;


};


#endif //T1_SB_TABELADIRETIVAS_H
