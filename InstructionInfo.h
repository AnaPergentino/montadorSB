//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_INSTRUCTIONINFO_H
#define T1_SB_INSTRUCTIONINFO_H

#include <string>

/**
 * Em uma instrução, temos as seguintes informações:
 *  - Nome simbólico
 *  - OPCODE
 *  - Tamanho
 *  - Número de operandos
 *  - Operação
 *  Para a operação, será passado um int com o nome da operação, que vai chamar um switch e escolherá a operação correta
 */
class InstructionInfo {
private:
    std::string nomeSimbolico;
    size_t tamanho;
    int numeroDeOperandos;

    enum opcode{
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

public:

    const std::string &getNomeSimbolico() const;

    void setNomeSimbolico(const std::string &nomeSimbolico);

    size_t getTamanho() const;

    void setTamanho(size_t tamanho);

    int getNumeroDeOperandos() const;

    void setNumeroDeOperandos(int numeroDeOperandos);

    void escolherOperacao(opcode);

};


#endif //T1_SB_INSTRUCTIONINFO_H
