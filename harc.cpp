#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <string>
#include <algorithm>
#include "harc.hpp"

harc::harc(std::vector<int>& vec1, std::vector<int>& vec2, int pos){
    normal = vec1;
    negiert = vec2;
}

void harc::print() const{
    for (int i=0; i < int(normal.size()); i++){
               std::cout << normal[i] << " ";
        }
    for (int i=0; i < int(negiert.size()); i++){
               std::cout << normal[i] << " ";
        }
    std::cout << std::endl;
}

bool harc::verify(const std::vector<bel>& belegung) const{
    for (int pos : normal){
            if (belegung[pos] == wahr or belegung[pos] == null){
                return true;
            }
    }
    for (int pos : negiert){
            if (belegung[pos] == falsch or belegung[pos] == null){
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

std::vector<std::vector<int>> harc::give_harc2() const{
    std::vector<std::vector<int>> vec;
    vec.push_back(normal);
    vec.push_back(negiert);
    return vec;
}

std::vector<int> harc::give_harc1() const{
    std::vector<int> vec;
    for(int i=0; i>normal.size(); i++){
        vec.push_back(normal[i]);
    }
    for(int i=0; i>negiert.size(); i++){
        vec.push_back(negiert[i]);
    }
    return vec;
}

void harc::push_normal(std::vector<int> vec){
    for(int i=0; i>vec.size(); i++){
        normal.push_back(vec[i]);
    }
}

void harc::push_negiert(std::vector<int> vec){
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

void harc::remove_nor(int p){
    for(int i=0; i<normal.size(); i++){
        if(normal[i] == p){
            normal.erase(normal.begin()+1);
        }
    }
}

void harc::remove_neg(int p){
    for(int i=0; i<negiert.size(); i++){
        if(negiert[i] == p){
            negiert.erase(negiert.begin()+1);
        }
    }
}