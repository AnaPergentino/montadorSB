//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_PREPROCESSAMENTO_H
#define T1_SB_PREPROCESSAMENTO_H


#include <fstream>

class PreProcessamento {
    void processarMacros(std::ifstream arquivo);
    void processarDiretivas(std::ifstream arquivo);
    void montarCodigo(std::ifstream arquivo);

};


#endif //T1_SB_PREPROCESSAMENTO_H
