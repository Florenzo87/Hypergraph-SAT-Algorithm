#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <chrono>
#include "hypergraph.hpp"

bool DPL(HG H);
var Deduce(HG& H, var const & u, bel const & l);
bool Restriction(HG P);
bool Relaxation(HG H);
bel tvalue(var const & u, harc const & a);
void print(const std::vector<bel>& vec);
void print(const std::vector<bool>& vec);
void print(const std::vector<int>& vec);


int main(int argc, char** argv){
    HG H(argv[1]);
    H.print();
    //H.printFSBS();
    auto start = std::chrono::high_resolution_clock::now();
    bool loesbar = DPL(H);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << loesbar << std::endl;
    std::cout << "time: " << duration.count() << "ms" << std::endl;
    return 0;
}

//DPL ist ziemplich trivial, man versucht eine Relaxation dann eine Restriction und falls keiner von dieser den Subproblem als
//wahr oder falsch beweist, dann stellt man zwei neue Subproblems durch ein Branching an der Variable die die branch_var Funktion
//bestimmt, diese setzen wir sowohl als falsch als auch wahr und setzen sie in den Queue falls sie keine Contradiction erzeugen
bool DPL(HG H){
    std::stack<HG> Q;
    Q.push(H);
    HG P = H;
    while(true){
        if(Q.empty()) {
            return false;
        }
        else{
            P = Q.top();
            Q.pop();
        }
        //P.print();
        if(P.SimplifyUR() == true){
            //P.print();
            //P.printFSBS();
            if(P.empty()){
                //P.print();
                std::cout << "Solved through Unit Resolution" << std::endl;
                print(P.get_belegung());
                H.set_belegung(P.get_belegung());
                std::cout << H.verify_strict() << std::endl;
                return true;
            }
            if(Relaxation(P) == true){
                //std::cout << "Relaxation is true" << std::endl;
                if(Restriction(P) == true){
                    std::cout << "Restriction is true" << std::endl;
                    print(P.get_belegung());
                    H.set_belegung(P.get_belegung());
                    std::cout << H.verify() << std::endl;
                    return true;
                }
                //std::cout << "Restriction is false" << std::endl;
                int k = P.minimal_harc();
                //std::cout << "k=" << k << std::endl;
                std::vector<int> branch_var = P.branching_var(k);
                int p = branch_var[0];
                float Fk = branch_var[1];
                float Bk = branch_var[2];
                //std::cout << "p=" << p << std::endl;
                HG PT = P;
                HG PF = P;
                bool t = PT.Branching_True(p);
                bool f = PF.Branching_False(p);
                if(k>2){
                    if(Bk > Fk){
                        if(t == true){
                            Q.push(PT);
                        }
                        if(f == true){
                            Q.push(PF);
                        }
                    }
                    else{
                        if(f == true){
                            Q.push(PF);
                        }
                        if(t == true){
                            Q.push(PT);
                        }
                    }
                }
                else if(k == 2){
                    if(Bk > Fk){
                        if(f == true){
                            Q.push(PF);
                        }
                        if(t == true){
                            Q.push(PT);
                        }
                    }
                    else{
                        if(t == true){
                            Q.push(PT);
                        }
                        if(f == true){
                            Q.push(PF);
                        }
                    }
                }
                else{
                    std::cout << "error" << std::endl;
                }
            }
            else{
                //std::cout << "Relaxation failed" << std::endl;
            }
        }
        else{
            //std::cout << "Unit Resolution failed" << std::endl;
        }
    }
    return false;
}



var Deduce(HG& H, var const & u, bel const & l){
    H.set_visited(true, u.get_var());
    H.set_L(u.get_var(), l);
    if(l == wahr){
        H.set_AD(u.get_var(), H.get_FS()[u.get_var()]);
    }
    else{
        H.set_AD(u.get_var(), H.get_BS()[u.get_var()]);
    }
    for(harc h : H.get_AD(u.get_var())){
        int a;
        for(int i=0; i<H.get_hgraph().size(); i++){
            if(h.get_pos() == H.get_hgraph()[i].get_pos()){
                a = i;
            }
        }
        if (H.get_hgraph()[a].give_harc1().size()>2){                               //harcs mit mehr als zwei variablen werden 
            H.get_hgraph()[a].set_V(H.get_hgraph()[a].get_V()-1);                   //geschrumpft zu welche mit 2 Variablen
            if(H.get_hgraph()[a].get_V() == 1){
                var w = H.root(H.get_hgraph()[a].give_harc1());
                if(H.get_hgraph()[a].Last(H.get_visited()) == 0){
                    return w; 
                }
                var last = H.get_vars()[H.get_hgraph()[a].Last(H.get_visited())-1];
                std::vector<harc> ADw = H.get_AD(w.get_var());
                std::vector<int> Head = {last.get_var()};
                std::vector<int> Tail = {w.get_var()};
                harc shrink(Tail, Head, H.get_harcs());
                ADw.push_back(shrink);
                H.get_hgraph().push_back(shrink);
                H.get_FS()[w.get_var()].push_back(shrink);
                H.get_BS()[last.get_var()].push_back(shrink);
                H.set_AD(w.get_var(), ADw);
                H.increase_harcs();
            }
            else if(H.get_hgraph()[a].get_V() == 0){
                return H.root(H.get_hgraph()[a].give_harc1());
            }
        }
    }
    for(harc h : H.get_AD(u.get_var())){
        int a;
        for(int i=0; i<H.get_hgraph().size(); i++){
            if(h.get_pos() == H.get_hgraph()[i].get_pos()){
                a = i;
            }
        }
        if (H.get_hgraph()[a].give_harc1().size() == 2){
            var v;
            if(H.get_hgraph()[a].give_harc1()[0] == u.get_var()){               //mit harcs mit 2 Variablen ist es einfach zu sehen
                v = H.get_hgraph()[a].give_harc1()[0];                          //welche Konsequenzen das setzen einer Variable
            }                                                                   //auf einen bestimmten Wert hat
            else{
                v = H.get_hgraph()[a].give_harc1()[1];
            }
            if(H.get_L()[v.get_var()] == null){
                H.set_P(v.get_var(), u.get_var());
                Deduce(H, v, tvalue(v, h));
            }
            else if(H.get_L()[v.get_var()] != tvalue(v, h)){
                return v;
            }
        }
    }
    return var(0);
}


bool Relaxation(HG H){
    H.set_for_relaxation();
    std::vector<std::vector<bel>> B;
    std::vector<bel> bi = {wahr, falsch};
    std::vector<int> S = H.get_S();
    H.set_for_relaxation();
    for(int i=0; i<H.variables()+1; i++){
            B.push_back(bi);
        }
    while(S.size() != 0){                                       //Step 1 - Abbrechen der Relaxation fall es keine variablen mehr 
        int u = S[0];                                           //zu überprufen gibt, sonst die nächste Variable betrachten
        S.erase(S.begin());
        bool skip_step3 = false;
        int deduce_reps = 0;
        std::vector<std::vector<bel>> Deduceu;
        std::vector<bel> L_prev = H.get_L();
        for(bel l : B[u]){                                      //Step 2 - Deduce für jede noch nicht überprufte Belegung durchführen
            H.set_values_Deduce();                              //fallse Deduce ein Knoten zuruckwirft, dann gab es ein Fehler beim
            H.set_L(L_prev);                                    //setzen dieses Wertes und im Folge dessen soll Theorem 1 benutzt
            var v = Deduce(H, u, l);                            //werden und die Belegung aller Variablen in denen Predecessor Baum
            Deduceu.push_back(H.get_L());                       //umgekehrt werden
            deduce_reps += 1;   
            if (v.get_var() != 0){
                H.set_valuesT1(v);                              
                skip_step3 = true;
            }
            else{
                for(int i=0; i<H.get_visited().size(); i++){
                    if(H.get_visited()[i] == true){             //falls es kein Knoten zuruckgeworfen wird, darf man bei alle besuchte
                        for(int j=0; j<B[i].size(); j++){       //Knoten den gesetzten Wert von B entfernen, da dieser schon als gültig
                            if(B[i][j] == H.get_L()[u]){        //erwiesen wurde
                                B[i].erase(B[i].begin()+j);
                                j -= 1;
                            }
                        }
                    }
                }
            }
        }

        bool skip_step4 = false;                                //Step 3 - falls man Deduce auf u sowohl für wahr als für falsch 
        if(skip_step3 == false && deduce_reps == 2){            //gemacht hat, dann darf man Property 1 anwenden und
            for(int i=0; i<Deduceu[0].size(); i++){             //alle Variablen die in beide Deduce auf dasselbe gesetzt wurden 
                if(Deduceu[0][i] == Deduceu[1][i]){             //mit diesen Wert belegen
                    if(i == H.variables() && Deduceu[0][i] != true){
                        return false;
                    }
                    if(i == H.variables()-1 && Deduceu[0][i] != false){
                        return false;
                    }
                    L_prev[i] = Deduceu[0][i];
                }
            }
            H.set_L(L_prev);
        }
        else{
            skip_step4 == true;
        }

        if(skip_step4 == false){                                //Step 4 - Unit Resolution anwenden um zu überprufen das man keine
            bool contradiction = H.SimplifyUR();                //illegale Kanten hinzugefügt hat
            if(contradiction == false){                         //contradiction ist false falls es eine gibt  
                return false;
            }
            else{
                std::vector<int> S_after_deleting{};            //wenn kein Problem durch die Unit Resolution entstanden ist, 
                for (int const variable_index : S) {            //darf man alle Knoten die eine besetzung zugeteilt wurde 
                   if (H.get_L()[variable_index] == null) {     //diese behalten und von S entfernt werden
                      S_after_deleting.push_back(variable_index);
                   }
                }
                std::swap(S, S_after_deleting);
            }
        }
    }
    return true;
}

//Restriction bassiert sich an die Existenz von B-Wege in der B-Reduction, wenn man eine
//B-Reduction findet in dem F und T nicht durch eonem B-Weg verbunden sind,
//dann muss der ursprüngliche Problem eine gültige Belegung haben
bool Restriction(HG P){                                          
    std::queue<HG> problems;                                    
    std::vector<int> h = P.Restriction();                       
    if(h.size() != 0){                                      //Restriction wirft der letzte harc(bzw. deren Tail) zuruck wenn es ein
        for(int j=0; j<h.size(); j++){                      //gültigen B-Weg gibt, sonst eine leere Klausel
            HG Pi = P;
            bool push = true;
            for(int i=0; i<j; i++){
                if(Pi.Branching_False(h[i]) == false){
                    push = false;
                }
            }
            if(Pi.Branching_True(h[j]) == false){           //wir setzen dann die j. Variable auf wahr und alle davor auf wahr
                push = false;                               //für den subproblem Pj, da mindestenst eine variable im Tail wahr
            }                                               //sein muss
            if(push == true){
                problems.push(Pi);
            }
        }
    }
    else{
        return true;
    }
    while(problems.empty() == false){                       //hier könnte man die ganze Restriction wiedder anfangen mit die subproblems
        if(problems.front().Restriction().size() == 0){     //und deren subproblems bilden, Ich habe mich aber dafür entschieden nur den
            return true;                                    //B-Weg zu suchen, da es sonst die unsat Instanzen noch mehr belastet
        }
        problems.pop();
    }
    return false;
}

bel tvalue(var const & u, harc const & a){              //gibt an wie man die gegebene Variable besetzen muss damit der gegebene harc erfüllt wird
    bel val = null;
    for(var v : a.give_harc2()[0]){
        if(v.get_var() == u.get_var()){
            val = wahr;
        }
    }
    for(var v : a.give_harc2()[1]){
        if(v.get_var() == u.get_var()){
            val = falsch;
        }
    }
    return val;
}

void print(const std::vector<bel>& vec){                                    
        for(int i = 0; i < int(vec.size()); i++){
                std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
}

void print(const std::vector<bool>& vec){                               
        for(int i = 0; i < int(vec.size()); i++){
                std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
}

void print(const std::vector<int>& vec){                               
        for(int i = 0; i < int(vec.size()); i++){
                std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
}

