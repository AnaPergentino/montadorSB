//
// Created by HAL-9000 on 17/05/2018.
//

#include "LabelParse.h"

std::string LabelParse::parseLabel(std::string strLabel) {
    std::string label;
    // Provavelmente a label virá no seguinte formato:
    // LABEL:
    // Então o programa precisa separar a string dos dois pontos e retornar a label original, a ser inserida na
    // tabela de símbolos.
    label = strLabel.substr(0, strLabel.find(":"));
    return label;
}
