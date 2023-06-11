#include <iostream>
#include <vector>
#include "var.hpp"x

var::var(int i){
    variable = i;
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
    for(harc a : FS){
        if (a.size() == k){
            FSk += 1;
        }
    }
    for(harc a : BS){
        if (a.size() == k){
            BSk += 1;
        }
    }
    int W = FSk + BSk + (1.5*std::min(BSk, FSk));
}