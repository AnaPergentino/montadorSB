//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_PREPROCESSAMENTO_H
#define T1_SB_PREPROCESSAMENTO_H


#include <fstream>

class PreProcessamento {
public:
    PreProcessamento(const std::string &fileString);

private:
    std::string fileString;
public:
    void processarMacros(std::ifstream arquivo);
    void processarDiretivas(std::string fileString);
    void montarCodigo(std::ifstream arquivo);

    void processarEQU();
    bool isEQU(std::string linha);
    void processarIF();
    bool isIF(std::string linha);


    const std::string &getFileString() const;

    void setFileString(const std::string &fileString);
};


#endif //T1_SB_PREPROCESSAMENTO_H
