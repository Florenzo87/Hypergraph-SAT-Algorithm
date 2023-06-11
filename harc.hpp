#pragma once 
#include <iostream>
#include <vector>
#include "var.hpp"

enum bel {falsch, wahr, null};

class harc{
    public:
        harc(std::vector<var>& normal, std::vector<var>& negiert, int pos);                                      //Baut ein harc element aus ein Vektor von ints
        void print() const;                                                 //print
        bool verify(const std::vector<bel>& belegung) const;                //uberprüft anhand einer gegebene Belegung ob diese bei die Klausel gültig ist
        bool empty();
        std::vector<std::vector<var>> give_harc2() const;
        std::vector<var> give_harc1() const;
        void push_normal(std::vector<var>);
        void push_negiert(std::vector<var>);
        int size();
        void set_value(int i);
        int get_value();
        int get_pos();
        void set_pos(int i);
        void remove_nor(var p);
        void remove_neg(var p);
    private:
        std::vector<var> normal;                                               //Klausel ist ein Vektor aus ints
        std::vector<var> negiert;
        int Value;
        int pos;
};