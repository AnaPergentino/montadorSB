//
// Created by HAL-9000 on 20/05/2018.
//

#ifndef T1_SB_ERRORLIB_H
#define T1_SB_ERRORLIB_H


#include <string>

class ErrorLib {
private:
    int linhaDoErro;
    std::string mensagemDeErro;
    std::string tipoErro; //Léxico, sintático ou semântico
public:
    ErrorLib(int linhaDoErro, const std::string &mensagemDeErro, const std::string &tipoErro);
};


#endif //T1_SB_ERRORLIB_H
