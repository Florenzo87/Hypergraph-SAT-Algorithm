#pragma once 
#include <iostream>
#include <vector>

enum bel {falsch, wahr, null};

class harc{
    public:
        harc(std::vector<int>& Head, std::vector<int>& Tail, int pos);                                      //Baut ein harc element aus ein Vektor von ints
        harc();
        void print() const;                                                 //print
        bool verify(const std::vector<bel>& belegung) const;                //uberprüft anhand einer gegebene Belegung ob diese bei die Klausel gültig ist
        bool empty();
        std::vector<std::vector<int>> give_harc2() const;
        std::vector<int> give_harc1() const;
        std::vector<int> give_harc1neg() const;
        void push_Head(std::vector<int>);
        void push_Tail(std::vector<int>);
        void set_Head(std::vector<int>);
        void set_Tail(std::vector<int>);
        int size();
        void set_value(int i);
        int get_value();
        int get_pos();
        void set_pos(int i);
        void remove_nor(int p);
        void remove_neg(int p);
    private:
        std::vector<int> Head;                                               //Klausel ist ein Vektor aus ints
        std::vector<int> Tail;
        int Value;
        int pos;
};