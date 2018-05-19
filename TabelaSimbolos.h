//
// Created by HAL-9000 on 18/05/2018.
//

#ifndef T1_SB_TABELASIMBOLOS_H
#define T1_SB_TABELASIMBOLOS_H


#include <string>

class TabelaSimbolos {
private:
    std::string labelNome;
    int labelEndereco;

public:
    TabelaSimbolos(std::string label, int endereco);

    const std::string &getLabelNome() const;

    void setLabelNome(const std::string &labelNome);

    int getLabelEndereco() const;

    void setLabelEndereco(int labelEndereco);

private:
    int contadorDeLinhas;
    int locationCounter;

};


#endif //T1_SB_TABELASIMBOLOS_H
