#pragma once 
#include <iostream>
#include <vector>
#include "harc.hpp"
#include "var.hpp"


class HG{
    public:
        HG(const std::string filename);                                     //baut HG element aus den gegebenen Textdokument
        void print() const;                                                 //druckt den HG auf die Konsole, jede Klausel auf einer Zeile
        void printFSBS();                                                   //druckt den HG auf die Konsole, jede Variable mit seinem FS und BS
        bool verify() const;                                                //uberprüft ob die Belegung richtig ist
        bool verify_strict();                                               //striktes verify, d.h. null/nicht gesetzt/2 zählt nicht als pauschal wahr sondern pauschal falsch
        void set_belegung(const std::vector<bel>& neubelegung);             //Belegung ändern zu den gegebenen Vektor
        std::vector<bel>const & get_belegung() const;                              //gibt Belegung zurück (die durch die Branchings definiert wurde)
        bool empty();                                                       //gibt an ob der HG leer ist               
        var root(std::vector<int> h);                                      //findet den root einer Gruppe an Variablen (durch deren indexes angegeben) durch benutzen der Predecessor Funktion
        bool Branching_True(int p);                                         //setzt die variable mit den gegebenen Index auf true und vereinfacht entsprechend den HG
        bool Branching_False(int p);                                        //setzt die variable mit den gegebenen Index auf false und vereinfacht entsprechend den HG
        bool SimplifyUR();                                                  //unit resolution, wirft false zurück falls diese zu eine contradiction führt
        bool Restriction();                                                 //Wrapper funktion für die restriction die den B-branchins aufruft
        std::vector<bool> Bbranching(int i);                                //macht einen B-Branching aus der Variable mit den gegeben index und gibt ein bool vektor der angibt welche variablenm besucht wurden zuruck
        std::vector<int> branchingFT();                                     //gibt den harc unmittelbar vor T bei einem B-branching aus F
        void set_valuesT1(var v);                                           //setzt die Werte der Predecessors der gegebene Variable entsprechend zum Theorem 1                             
        int minimal_harc();                                                 //findet die größe des kleinsten harcs, bzw. k für die branching Rule
        std::vector<int> branching_var(int k);                              //gibt die branching variable nach Rule 1
        std::vector<float> branching_var2(int k);                           //gibt die branching Variable nach Rule 2
        std::vector<var>& get_vars();                                        //get und set Funktion sollen hoffentlich offentsichtilich sein bei beachten der untere Beschreibung der Elemente
        std::vector<harc>& get_hgraph();
        int variables() const;  //bzw. get_var_num()
        int get_harcs();
        std::vector<std::vector<harc>>& get_FS();
        std::vector<std::vector<harc>>& get_BS();
        void set_P(int v, int u);
        void set_L(std::vector<bel> & vec);
        bool set_L(int u, bel l);
        std::vector<bel> const & get_L();
        void set_AD(int u, std::vector<harc>& h);
        std::vector<harc> const & get_AD(int u);
        std::vector<bool>const & get_visited();
        void set_visited(bool l, int u);
        std::vector<int> const & get_S();
        void set_V();
        void increase_harcs();
        void set_for_relaxation();
        void set_values_Deduce();
    private:
        std::vector<harc> hgraph;                                           //hgraph entaltet alle harcs im hypergraph
        std::vector<bel> belegung;                                          //Vektor der enum Klasse bel die angibt ob es ein Branching zu die entsprechende variable gemacht wurde(bzw. was für einen)
        std::vector<var> vars;                                              //vars enthaltet alle Variable die im URSPRÜNGLICHEN hypergraph waren, inklusiv F und T
        int var_num;                                                        //Anzahl an Variablen (des ursprünglichen HGs)
        int harcs;                                                          //Anzahl an harcs (die es insgesamt gegeben hat inlusiv gelöschte und geschrumpfte, da man es benutzen will damit keine zwei harcs diesselbe kennungsnummer bekommt)
        std::vector<std::vector<harc>> FS;                                  //Liste Forward Star der Variablen, muss beim löschen eines harcs updated werden da es nur Kopien sind
        std::vector<std::vector<harc>> BS;                                  //wie oben aber für den Backward Star
        std::vector<int> Predecessor;                                       //diese sind alle "Funktionen" für die Relaxation
        std::vector<bel> L;
        std::vector<std::vector<harc>> AD;
        std::vector<bool> visited;
        std::vector<int> S;
        std::vector<float> Sb;                                              //haben nichts mit S zu tun, sind für die Werte die für die branching variable benötigt sind
        std::vector<float> Sf;
        //Merke F-> var_num-1   T->var_num
};