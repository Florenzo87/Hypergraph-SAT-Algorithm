#include <iostream>
#include <vector>
#include "var.hpp"

var::var(int i){
    variable = i;
}

var::var(){
    variable = 0;
}

int var::get_var() const{
    return variable;
}

void var::set_value(int i){
    value = i;
}

int var::get_value() const{
    return value;
}

std::vector<harc> var::get_FS(){
    return FS;
}

std::vector<harc> var::get_BS(){
    return BS;
}

void var::set_FS(std::vector<harc> h){
    FS = h;
}
    
void var::set_BS(std::vector<harc> h){
    BS = h;
}

int var::W(int k){
    int FSk = 0;
    int BSk = 0;
    std::cout << FS.size() <<std::endl;
    for(harc a : FS){
        if (a.size() == k){
            FSk += 1;
        }
    }
    std::cout << BS.size() <<std::endl;
    for(harc a : BS){
        if (a.size() == k){
            BSk += 1;
        }
    }
    std::cout << FSk << BSk << std::endl;
    int W = FSk + BSk + (1.5*std::min(BSk, FSk));
    return W;
}