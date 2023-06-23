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

bool var::active() const{
    return activated;
}

void var::deactivate(){
    activated = false;
}