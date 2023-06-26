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
var Deduce(HG& H, var u, bel l);
bool Restriction(HG P);
bool Relaxation(HG H);
bel tvalue(var u, harc a);
void print(const std::vector<bel>& vec);
void print(const std::vector<bool>& vec);


int main(int argc, char** argv){
    HG H(argv[1]);
    //H.print();
    //H.printFSBS();
    auto start = std::chrono::high_resolution_clock::now();
    bool loesbar = DPL(H);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << loesbar << std::endl;
    std::cout << "time: " << duration.count() << "ms" << std::endl;
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
                //std::cout << "Relaxation is true" << std::endl;
                if(Restriction(P) == true){
                    std::cout << "Restriction is true" << std::endl;
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

var Deduce(HG& H, var u, bel l){
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
        if (H.get_hgraph()[a].give_harc1().size()>2){
            H.get_hgraph()[a].set_V(H.get_hgraph()[a].get_V()-1);
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
                //std::cout << w.get_var() << " " << last.get_var() << std::endl;
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
            if(H.get_hgraph()[a].give_harc1()[0] == u.get_var()){
                v = H.get_hgraph()[a].give_harc1()[0];
            }
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
    H.set_for_relaxation();
    std::vector<std::vector<bel>> B;
    std::vector<bel> bi = {wahr, falsch};
    std::vector<int> S = H.get_S();
    H.set_for_relaxation();
    for(int i=0; i<H.variables()+1; i++){
            B.push_back(bi);
        }
    while(S.size() != 0){                                       //Step 1
        int u = S[0];
        S.erase(S.begin());
        bool skip_step3 = false;
        int deduce_reps = 0;
        std::vector<std::vector<bel>> Deduceu;
        std::vector<bel> L_prev = H.get_L();
        for(bel l : B[u]){                                      //Step 2
            H.set_values_Deduce();
            H.set_L(L_prev);
            var v = Deduce(H, u, l);
            Deduceu.push_back(H.get_L());
            deduce_reps += 1;
            if (v.get_var() != 0){
                H.set_valuesT1(v);                              
                skip_step3 = true;
            }
            else{
                for(int i=0; i<H.get_visited().size(); i++){
                    if(H.get_visited()[i] == true){
                        for(int j=0; j<B[i].size(); j++){
                            if(B[i][j] == H.get_L()[u]){
                                B[i].erase(B[i].begin()+j);
                            }
                        }
                    }
                }
            }
        }

        bool skip_step4 = false;                                //Step 3
        if(skip_step3 == false && deduce_reps == 2){    
            for(int i=0; i<Deduceu[0].size(); i++){
                if(Deduceu[0][i] == Deduceu[1][i]){
                    L_prev[i] = Deduceu[0][i];
                }
            }
            H.set_L(L_prev);
        }
        else{
            skip_step4 == true;
        }

        if(skip_step4 == false){                               //Step 4
            //H.print();
            //std::cout << "Deduce Unit Resolution" << std::endl;
            bool contradiction = H.SimplifyUR();               //contradiction ist false falls es eine gibt  
            //std::cout << "a" << std::endl;
            if(contradiction == false){
                //H.print();
                return false;
            }
            else{
                for(int i=1; i<H.variables()+1; i++){
                    if(H.get_L()[i] != null){
                        for(int j=0; j<S.size(); j++){
                            if(S[j] == i){
                                S.erase(S.begin()+j);
                            }
                        }
                    }
                }
            }
        }
    }
    return true;
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

void print(const std::vector<bool>& vec){                               
        for(int i = 0; i < int(vec.size()); i++){
                std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
}

