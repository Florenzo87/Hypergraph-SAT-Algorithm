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

bool harc::verify_strict(const std::vector<bel>& belegung){
    for (int pos : Head){
            if (belegung[pos] == wahr){
                return true;
            }
    }
    for (int pos : Tail){
            if (belegung[pos] == falsch){
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

std::vector<std::vector<int>> harc::give_harc2() const{   // gives harc as the tail and head sepparately
    std::vector<std::vector<int>> vec;
    vec.push_back(Tail);
    vec.push_back(Head);
    return vec;
}

std::vector<int> harc::give_harc1() const{              //gives Tail and Head of harc together, in other words all variables in the harc
    std::vector<int> vec;
    for(int i=0; i<Tail.size(); i++){
        vec.push_back(Tail[i]);
    }
    for(int i=0; i<Head.size(); i++){
        vec.push_back(Head[i]);
    }
    return vec;
}

std::vector<int> harc::give_harc1neg() const{          //same as previous but with a minus for the tail, was necessary for a now removed function
    std::vector<int> vec;
    for(int i=0; i<Tail.size(); i++){
        vec.push_back(-Tail[i]);
    }
    for(int i=0; i<Head.size(); i++){
        vec.push_back(Head[i]);
    }
    return vec;
}

void harc::push_Head(std::vector<int> vec){
    for(int i=0; i<vec.size(); i++){
        Head.push_back(vec[i]);
    }
}

void harc::push_Tail(std::vector<int> vec){
    for(int i=0; i<vec.size(); i++){
        Tail.push_back(vec[i]);
    }
}

void harc::set_Head(std::vector<int> vec){
    Head = vec;
}

void harc::set_Tail(std::vector<int> vec){
    Tail = vec;
}

int harc::size(){
    int size = Head.size() + Tail.size();
    return size;
}

void harc::set_V(int i){
    V = i;
}

int harc::get_V(){
    return V;
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
            i -= 1;
        }
    }
}

void harc::remove_neg(int p){
    for(int i=0; i<Tail.size(); i++){
        if(Tail[i] == p){
            Tail.erase(Tail.begin()+i);
            i -= 1;
        }
    }
}

bool harc::active(){
    return activated;
}

void harc::deactivate(){
    activated = false;
}

void harc::set_unused(){
    unused = true;
}

void harc::set_used(){
    unused = false;
}

bool harc::get_unused(){
    return unused;
}

int harc::Last(std::vector<bool> visited){
    for(int i : Head){
        if(visited[i] == false){
            std::cout << i << std::endl;
            return i;
        }
    }
    for(int i : Tail){
        if(visited[i] == false){
            std::cout << i << std::endl;
            return i;
        }
    }
    std::cout << "none found" << std::endl;
    return 0;
}