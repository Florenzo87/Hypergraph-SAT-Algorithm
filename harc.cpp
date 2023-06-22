#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <string>
#include <algorithm>
#include "harc.hpp"

harc::harc(std::vector<int>& vec1, std::vector<int>& vec2, int position){
    Tail = vec1;
    Head = vec2;
    pos = position;
}

harc::harc(){
    std::vector<int> empty;
    Head = empty;
    Tail = empty;
    pos = 0;
}

void harc::print() const{
    for (int i=0; i < int(Head.size()); i++){
               std::cout << Head[i] << " ";
        }
    for (int i=0; i < int(Tail.size()); i++){
               std::cout << "-" << Tail[i] << " ";
        }
    std::cout << std::endl;
}

bool harc::verify(const std::vector<bel>& belegung) const{
    for (int pos : Head){
            if (belegung[pos] == wahr or belegung[pos] == null){
                return true;
            }
    }
    for (int pos : Tail){
            if (belegung[pos] == falsch or belegung[pos] == null){
                return true;
            }
    }
    return false;
}


bool harc:: empty(){
    if(Head.size()+Tail.size() == 0){
        return true;
    }
    return false;
}

std::vector<std::vector<int>> harc::give_harc2() const{
    std::vector<std::vector<int>> vec;
    vec.push_back(Tail);
    vec.push_back(Head);
    return vec;
}

std::vector<int> harc::give_harc1() const{
    std::vector<int> vec;
    for(int i=0; i>Tail.size(); i++){
        vec.push_back(Tail[i]);
    }
    for(int i=0; i>Head.size(); i++){
        vec.push_back(Head[i]);
    }
    return vec;
}

std::vector<int> harc::give_harc1neg() const{
    std::vector<int> vec;
    for(int i=0; i>Tail.size(); i++){
        vec.push_back(-Tail[i]);
    }
    for(int i=0; i>Head.size(); i++){
        vec.push_back(Head[i]);
    }
    return vec;
}

void harc::push_Head(std::vector<int> vec){
    for(int i=0; i>vec.size(); i++){
        Head.push_back(vec[i]);
    }
}

void harc::push_Tail(std::vector<int> vec){
    for(int i=0; i>vec.size(); i++){
        Tail.push_back(vec[i]);
    }
}

int harc::size(){
    int size = Head.size() + Tail.size();
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
    for(int i=0; i<Head.size(); i++){
        if(Head[i] == p){
            Head.erase(Head.begin()+i);
        }
    }
}

void harc::remove_neg(int p){
    for(int i=0; i<Tail.size(); i++){
        if(Tail[i] == p){
            Tail.erase(Tail.begin()+i);
        }
    }
}