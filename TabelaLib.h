//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_TABELADIRETIVAS_H
#define T1_SB_TABELADIRETIVAS_H


#include <string>
#include <vector>
#include <array>
#include <algorithm>

// Para essa classe precisamos definir 2 tabelas estáticas e uma dinâmica:
// 1. Tabela de Diretivas
// 2. Tabela de Instruções
// 3. Tabela de Símbolos

// Para isso, precisamos de um map para associar as diretivas e outro para as instruções
// Após isso, precisaremos dizer, para cada instrução ou diretiva:
// Operandos
// Tamanho
// AÇão
class TabelaLib {
public:
    enum opcodes{
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

    enum diretivas{
        SECTION,
        SPACE,
        CONST,
        EQU,
        IF,
        MACRO,
        ENDMACRO,
    };

    // TODO: Também é necessário inserir o tipo de operação a ser realizado, mas não sei como fazer isso agora
    struct infoDeInstrucoes {
        int numeroDeOperandos;
        int tamanho;
        opcodes opcodesInstrucoes; // O código de enum já associa ao código da tabela
        // ? operacaoARealizar;
    };

    struct infoDeDiretivas {
        int numeroDeOperandos;
        int tamanho;
        diretivas diretivasDiretivas; // nome horrível, aceito sugestoes
        bool isPre;
    };

//    void montarTabelaDeDiretivas(std::vector tabelaDeSimbolos);
    std::string converterOpcodesEmString(int opcode);



};


#endif //T1_SB_TABELADIRETIVAS_H
