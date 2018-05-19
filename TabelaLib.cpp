//
// Created by HAL-9000 on 18/05/2018.
//

#include "TabelaLib.h"

//void TabelaLib::montarTabelaDeDiretivas(std::vector tabelaDeSimbolos) {
//
//}

std::string TabelaLib::converterOpcodesEmString(int opcode) {
    switch (opcode){
        case ADD:
            return "add";
        case SUB:
            return "sub";
        case MULT:
            return "mult";
        case DIV:
            return "div";
        case JMP:
            return "jmp";
        case JMPN:
            return "jmpn";
        case JMPP:
            return "jmpp";
        case JMPZ:
            return "jmpz";
        case COPY:
            return "copy";
        case LOAD:
            return "load";
        case STORE:
            return "store";
        case INPUT:
            return "input";
        case OUTPUT:
            return "output";
        case STOP:
            return "stop";
    }

}
