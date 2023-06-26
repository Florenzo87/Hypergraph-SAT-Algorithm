#pragma once 
#include <iostream>
#include <vector>

enum bel {falsch, wahr, null};

class harc{
    public:
        harc(std::vector<int>& Head, std::vector<int>& Tail, int pos);
        harc();         
        void print() const;                                                 //print
        bool verify(const std::vector<bel>& belegung) const;                //uberprüft anhand einer gegebene Belegung ob diese bei die Klausel gültig ist
        bool verify_strict(const std::vector<bel>& belegung);               //uberprüft anhand einer gegebene Belegung ob diese bei die Klausel gültig ist (erklärung zu strikt im HG)
        bool empty();
        std::vector<std::vector<int>> give_harc2() const;
        std::vector<int> give_harc1() const;
        std::vector<int> give_harc1neg() const;
        void push_Head(std::vector<int> const & vec);
        void push_Tail(std::vector<int> const & vec);
        void set_Head(std::vector<int> const & vec);
        void set_Tail(std::vector<int> const & vec);
        int size();
        void set_V(int i);
        int get_V();
        int get_pos();
        void set_pos(int i);
        void remove_nor(int p);
        void remove_neg(int p);
        int Last(std::vector<bool> const & visited);
    private:
        std::vector<int> Head;                                              //Head des harcs, durch den index im HG
        std::vector<int> Tail;                                              //Tail des harcs
        int V;                                                              //V Funktion aus der Relaxation
        int pos;                                                            //Identifier des harcs
};