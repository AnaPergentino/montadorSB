#include <iostream>
#include <string>     // forget about char[] and char*: use string instead !  
#include <algorithm>  // for copy_if()
#include <iterator>   // for the back_inserter
#include <sstream>
#include <vector>

std::string remove_extra_whitespaces(const std::string &input)
{
    std::string output;
    output.clear();  // unless you want to add at the end of existing sring...
    unique_copy (input.begin(), input.end(), std::back_insert_iterator<std::string>(output), [](char a,char b){ return isspace(a) && isspace(b);});
    return output;
}

std::string removeComentarios(std::string linha){
    // Comentários são definidos por um ponto e vírgula antes do comentário em si, então
    // separaremos a linha pelo delimitador ;
    // Verificamos antes se a linha possui comentários, e então fazemos a remoção de string
    unsigned int posicaoComentario = linha.find(';');
    if(posicaoComentario != std::string::npos) {
        std::string linhaSemComentarios = linha.substr(0, posicaoComentario);
        return linhaSemComentarios;
    }
    else {
        // Linha não possui comentários
        return linha;
    }
}

std::string removeTabulacoes(std::string fileString){
    fileString.erase(std::remove(fileString.begin(), fileString.end(), '\t'), fileString.end());
    return fileString;
}

std::vector<std::string> split_string(const std::string& str,
                                      const std::string& delimiter)
{
    std::vector<std::string> strings;

    std::string::size_type pos = 0;
    std::string::size_type prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos)
    {
        strings.push_back(str.substr(prev, pos - prev));
        prev = pos + 1;
    }

    // To get the last substring (or only, if delimiter is not found)
    strings.push_back(str.substr(prev));

    return strings;
}



int main(int argc, char **argv)
{
    std::cout << "testing 2 ..\n";

    std::string input = "asfa sas    f f dgdgd  dg   ggg\n eiehosink \t ieieeifnirit tttoo \tto\tde\t  boas\n ttt t rr\n\n\n\n  "
                        "tt t\n LABEL:\n se deu mal;kkkkk\n kk\n";

    std::string saida = removeTabulacoes(input);
    std::cout << "Saida sem tabulacoes: " << saida << std::endl;

    saida = remove_extra_whitespaces(saida);
    std::cout << "Saida sem tabs: " << saida << std::endl;

    saida = removeComentarios(saida);
    std::cout << "Saida sem comentarios: " << saida << std::endl;

    std::vector<std::string> stringList = split_string(saida, "\n");
    for (std::vector<std::string>::const_iterator i = stringList.begin(); i != stringList.end(); ++i)
        std::cout << "Lista de strings: " << *i << ' ' << std::endl;



    return 0;
}
