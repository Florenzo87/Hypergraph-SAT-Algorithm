#pragma once 
#include <iostream>
#include <vector>
#include "harc.hpp"
#include "var.hpp"


class HG{
    public:
        bool verify() const;                                                //uberprüft ob die Belegung richtig ist
        bool verify(const int zuletz_geaenderte_variable) const;            //uberprüft ob die Belegung richtig ist aber nur an der Klauseln in der sich die geänderte Variable befindet
        bool verify(const int erste_geaenderte_variable, const int letzte_geaenderte_variable) const;   //uberprüft ob die Belegung richtig ist aber nur an der Klauseln in der sich die geänderte Variablen befindet
        bool verify_strict();
        int backtrack_until() const;                                        //findet die kleinste Variable die die grösste in ihrer (falsche) Klausel ist
        void print() const;                                                 //druckt den HG auf die Konsole, jede Klausel auf einer Zeile
        void printFSBS();
        HG(const std::string filename);                                    //baut HG element aus den gegebenen Textdokument
        HG(std::vector<std::vector<int>>);
        HG(std::vector<harc> vec, int i);
        void set_belegung(const std::vector<bel>& neubelegung);             //Belegung ändern zu den gegebenen Vektor, Achtung: überprüft nicht ob dieser die richtige Länge hat
        void set_belegung(const int pos, const bel val);                    //Belegung an der gegebene Position zum gegebenen Wert ändern
        void set_belegung(const int pos);                                   //Belegung an der gegebene Position ändern (0 ->1, 1 ->0, 2 ->2)
        std::vector<bel> get_belegung() const;                              //gibt Belegung zurück
        int variables() const;                                              //gibt Anzahl an Variablen zurück
        int num_harcs() const;
        bool empty();
        harc& give_harc(int i);  
        std::vector<var> give_harc_as_vec(int i) const; 
        var root(std::vector <int> h);   
        var shrink(std::vector <int> h); 
        void set_V();
        HG binary();
        bool Branching_True(int p);
        bool Branching_False(int p);
        bool Branching_True2(int p);
        bool Branching_False2(int p);
        int minimal_harc();
        var branch_var(int k);
        bool SimplifyUR();
        void set_P(var v, var u);
        var get_P(var v);
        void set_valuesT1();
        void set_valuesP1();
        std::vector<var> get_vars();
        std::vector<bool> Bbranching(int i);
        std::vector<int> branchingFT();
        bool Restriction();
        int branching_var(int k);
        std::vector<std::vector<harc>>& get_FS();
        std::vector<std::vector<harc>>& get_BS();
        void set_L(std::vector<bel> vec);
        void set_L(int u, bel l);
        std::vector<bel> get_L();
        void set_AD(int u, std::vector<harc>);
        std::vector<harc> get_AD(int u);
        std::vector<harc>& get_hgraph();
        std::vector<bool> get_visited();
        void set_visited(bool l, int u);
        void increase_harcs();
    private:
        std::vector<harc> hgraph;                                            //HG ist ein Vektor aus der klasse Clause
        std::vector<bel> belegung;                                         //als vector von int 0,1,2 mit 2 als noch nicht belegt/gleichzeitig wahr und falsch, die erste Stelle entspricht keiner Variable und wird evtl. für der depth der besetzte Variablen benutzt
        std::vector<var> vars;
        int var_num;                                                            //Anzahl an Variablen
        int harcs;                                                        //Anzahl an harcs
        std::vector<std::vector<harc>> harcs_der_variable;                //in welche arcs sich jede Variable befindet
        std::vector<var> Predecessor;
        var F;
        var T;
        //std::vector<int> index_harc;                                         gibt an wo sich die variablen des ursprünglichen Problem befinden
        std::vector<std::vector<harc>> FS;
        std::vector<std::vector<harc>> BS;
        std::vector<bel> L;
        std::vector<std::vector<harc>> AD;
        std::vector<bool> visited;
};