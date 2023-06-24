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
#include "hypergraph.hpp"

bool DPL(HG H);
var Deduce(HG& H, var u, bel l);
bool Restriction(HG P);
bool Relaxation(HG H);
harc shrink(HG H, harc a, var w);
bel tvalue(var u, harc a);
void print(const std::vector<bel>& vec);


int main(int argc, char** argv){
    HG H(argv[1]);
    print(H.get_belegung());
    //H.printFSBS();
    bool loesbar = DPL(H);
    std::cout << loesbar << std::endl;
    return 0;
}


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
                std::cout << "Relaxation is true" << std::endl;
                if(Restriction(P) == true){
                    std::cout << "Restriction is true" << std::endl;
                    return true;
                }
                std::cout << "Restriction is false" << std::endl;
                int k = P.minimal_harc();
                std::cout << "k=" << k << std::endl;
                int p = P.branching_var(k);
                std::cout << "p=" << p << std::endl;
                HG PT = P;
                HG PF = P;
                bool t = PT.Branching_True(p);
                bool f = PF.Branching_False(p);
                if(k>2){
                    if(t == true){
                        Q.push(PT);
                    }
                    else{
                        std::cout << "true branching failed" << std::endl;
                    }
                    if(f == true){
                        Q.push(PF);
                    }
                    else{
                        std::cout << "false branching failed" << std::endl;
                    }
                }
                else{
                    if(f == true){
                        Q.push(PF);
                    }
                    else{
                        std::cout << "false branching failed" << std::endl;
                    }
                    if(t == true){
                        Q.push(PT);
                    }
                    else{
                        std::cout << "true branching failed" << std::endl;
                    }
                }
            }
        }
        else{
            std::cout << "Unit Resolution failed" << std::endl;
        }
    }
    return false;
}

var Deduce(HG& H, var u, bel l){
    std::vector<bool> visited;
    //std::queue<harc> Last;
    for(int i = 0; i < H.variables()+1; i++){
        visited.push_back(false);
    }
    //for(int i=0; i < H.num_harcs(); i++){
    //    Last.push(H.give_harc(i));
    //}
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
        if (H.get_hgraph()[a].give_harc1().size()>2){
            H.get_hgraph()[a].set_V(H.get_hgraph()[a].get_V()-1);
            if(H.get_hgraph()[a].get_V() == 1 && H.get_hgraph()[a].Last(H.get_visited()) != 0){
                var last = H.get_vars()[H.get_hgraph()[a].Last(H.get_visited())];
                var w = H.root(H.get_hgraph()[a].give_harc1());
                std::vector<harc> ADw = H.get_AD(w.get_var());
                std::vector<int> Tail = {last.get_var()};
                std::vector<int> Head = {w.get_var()};
                harc shrink(Tail, Head, H.get_hgraph().size());
                ADw.push_back(shrink);
                H.get_hgraph().push_back(shrink);           //möglicherweise FS und BS aktualisieren?
                H.get_FS()[last.get_var()].push_back(shrink);
                H.get_BS()[w.get_var()].push_back(shrink);
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
            if(H.get_hgraph()[a].give_harc1()[0] == u.get_var()){
                v = H.get_hgraph()[a].give_harc1()[0];
            }
            else{
                v = H.get_hgraph()[a].give_harc1()[1];
            }
            if(H.get_L()[v.get_var()] == null){
                H.set_P(v.get_var(), u.get_var());
            }
            else if(H.get_L()[v.get_var()] != tvalue(v, h)){
                return v;
            }
        }
    }
    return var(0);
}

bool Restriction(HG P){
    std::queue<HG> problems;
    if(P.Restriction() == false){
        std::vector<int> h = P.branchingFT();
        for(int j=0; j<h.size(); j++){
            HG Pi = P;
            for(int i=0; i<j; i++){
                Pi.Branching_False(h[i]);
            }
            Pi.Branching_True(h[j]);
            problems.push(Pi);
        }
    }
    else{
        return true;
    }
    while(problems.empty() == false){
        if(problems.front().Restriction() == true){
            return true;
        }
        problems.pop();
    }
    return false;
}

bool Relaxation(HG H){
    return true;
    /*
    std::vector<bel> L = H.get_belegung();
    std::vector<std::vector<bel>> B;
    std::vector<bel> bi = {wahr, falsch};
    std::vector<var> S;
    H.set_V();
    for(int i=0; i<H.variables(); i++){
            B.push_back(bi);
            if(H.get_belegung()[i] == null && i > 0){
                S.push_back(H.get_vars()[i]);
            }
        }
    while(S.size() != 0){
        var u = S[0];
        S.erase(S.begin());
        bool skip = false;
        std::vector<bel> Deduceu;
        for(bel l : B[u.get_var()]){
            std::vector<bel> altbelegung = H.get_belegung();
            H.set_L(L);
            var v = Deduce(H, u, l);
            Deduceu.push_back(H.get_belegung()[u.get_var()]);
            if (v.get_var() != 0){
                H.set_valuesT1();                               //Theorem 1
                skip = true;
            }
            else{
                L = H.get_belegung();
                H.set_belegung(altbelegung);
                for(int i=0; i<B.size(); i++){
                    //std::cout << i << std::endl;
                    for(int j=0; j<B[i].size(); j++){
                        if(B[i][j] == L[i]){
                            B[i].erase(B[i].begin()+j);
                        }
                    }
                }
                if(B[u.get_var()].size() == 1){
                    skip = true;
                }
            }
        }
        if(skip == false){
            if(Deduceu[0] == Deduceu[1]){
                H.set_belegung(u.get_var(), Deduceu[0]);          //Property 1
            }
        }
        bool contradiction = H.SimplifyUR();           //contradiction == true heißt es gibt keine
        if(contradiction == false){
            return false;
        
        }
        std::vector<bel> belegung = H.get_belegung();
        for(int i=0; i<belegung.size(); i++){
            if(belegung[i] != null){
                for(int l=0; l<S.size(); l++){
                    if(S[l].get_var() == i){
                        S.erase(S.begin()+l);
                    }
                }
            }
        }
    }
    return true;
    //*/
}

harc shrink(HG H, int a, var w, int pos){
    H = H.binary();
    var u = H.shrink(H.get_hgraph()[a].give_harc1());
    std::vector<int> Head = {u.get_var()};
    std::vector<int> Tail = {w.get_var()};
    harc as(Head, Tail, pos);
    return as;
}

bel tvalue(var u, harc a){
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
