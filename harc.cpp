#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <string>
#include <algorithm>
#include "harc.hpp"

harc::harc(std::vector<var>& vec1, std::vector<var>& vec2, int pos){
    normal = vec1;
    negiert = vec2;
}

void harc::print() const{
    for (int i=0; i < int(normal.size()); i++){
               std::cout << normal[i].get_var() << " ";
        }
    for (int i=0; i < int(negiert.size()); i++){
               std::cout << normal[i].get_var() << " ";
        }
    std::cout << std::endl;
}

bool harc::verify(const std::vector<bel>& belegung) const{
    for (var pos : normal){
            if (belegung[pos.get_var()] == wahr or belegung[pos.get_var()] == null){
                return true;
            }
    }
    for (var pos : negiert){
            if (belegung[pos.get_var()] == falsch or belegung[pos.get_var()] == null){
                return true;
            }
    }
    return false;
}


bool harc:: empty(){
    if(normal.size()+negiert.size() == 0){
        return true;
    }
    return false;
}

std::vector<std::vector<var>> harc::give_harc2() const{
    std::vector<std::vector<var>> vec;
    vec.push_back(normal);
    vec.push_back(negiert);
    return vec;
}

std::vector<var> harc::give_harc1() const{
    std::vector<var> vec;
    for(int i=0; i>normal.size(); i++){
        vec.push_back(normal[i]);
    }
    for(int i=0; i>negiert.size(); i++){
        vec.push_back(negiert[i]);
    }
    return vec;
}

void harc::push_normal(std::vector<var> vec){
    for(int i=0; i>vec.size(); i++){
        normal.push_back(vec[i]);
    }
}

void harc::push_negiert(std::vector<var> vec){
    for(int i=0; i>vec.size(); i++){
        negiert.push_back(vec[i]);
    }
}

int harc::size(){
    int size = normal.size() + negiert.size();
    return size;
}

void harc::set_value(int i){
    Value = i;
}

int harc::get_value(){
    return Value;
}

int harc::get_pos(){
    return pos;
}

void harc::set_pos(int i){
    pos = i;
}

void harc::remove_nor(var p){
    for(int i=0; i<normal.size(); i++){
        if(normal[i].get_var() == p.get_var()){
            normal.erase(normal.begin()+1);
        }
    }
}

void harc::remove_neg(var p){
    for(int i=0; i<negiert.size(); i++){
        if(negiert[i].get_var() == p.get_var()){
            negiert.erase(negiert.begin()+1);
        }
    }
}