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
        P.SimplifyUR();
        if(P.empty()){
            return true;
        }
        if(Relaxation(P) == true){
            if(Restriction(P) == true){
                return true;
            }
            int k = H.minimal_harc();
            var p = P.branch_var(k);
            HG PT = P;
            HG PF = P;
            PT.Branching_True(p);
            PF.Branching_False(p);
            if(k>2){
                Q.push(PT);
                Q.push(PF);
            }
            else{
                Q.push(PF);
                Q.push(PT);
            }
        }
    }
}

var Deduce(HG& H, var u, bel l){
    std::vector<bel> L = H.get_belegung();
    std::vector<std::vector<harc>> AD;
    std::queue<harc> Last;
    H.set_values();
    for(int i = 0; i < H.variables(); i++){
        L.push_back(null);
        std::vector<harc> empty;
        AD.push_back(empty);
    }
    for(int i=0; i>H.num_harcs(); i++){
        Last.push(H.give_harc(i));
    }
    L[u.get_var()] = l;
    if(l == true){
        AD[u.get_var()] = u.get_FS();
    }
    else{
        AD[u.get_var()] = u.get_BS();
    }
    for(harc a : AD[u.get_var()]){
        if (a.give_harc1().size()>2){
            a.set_value(a.get_value()-1);
            if(a.get_value() == 1 && Last.front().empty() == false){
                var w = H.root(a.give_harc1());
                AD[w.get_var()].push_back(shrink(H , a, w));
            }
            else if(a.get_value() == 1){
                return H.root(a.give_harc1());
            }
        }
        Last.pop();
    }
    for(harc a : AD[u.get_var()]){
        if (a.give_harc1().size() == 2){
            var b = shrink(H, a, H.root(a.give_harc1())).give_harc2()[0][0];
            var v(0);
            if(a.give_harc2()[0][0].get_var() != b.get_var()){
                var v = a.give_harc2()[0][0];
            }
            else{
                var v = a.give_harc2()[1][0];
            }
            if(L[a.give_harc2()[0][0].get_var()] == null){
                    H.set_P(v, u);
                    Deduce(H, v, tvalue(v,a));
                }
            else if(L[a.give_harc2()[0][0].get_var()] != tvalue(v,a)){
                return v;
            }
        }
    }
    var empty(0);
    return empty;
}

bool Restriction(HG P){
    return false;
}

bool Relaxation(HG H){
    std::vector<bel> L;
    std::vector<std::vector<bel>> B;
    std::vector<bel> bi = {wahr, falsch};
    std::vector<var> S;
    for(int i=0; i<H.variables(); i++){
            B.push_back(bi);
            S.push_back(var(i));
        }
    while(true){
        if(S.size() == 0){
            break;
        };
        var u = S[0];
        S.erase(S.begin());
        bool skip = false;
        std::vector<bel> Deduceu;
        for(bel l : B[u.get_var()]){
            std::vector<bel> altbelegung;
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
                   B[i].erase(std::find(B[i].begin(),B[i].end(),L[i]));
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

}

harc shrink(HG H, harc a, var w){
    H = H.binary();
    var u = H.shrink(a.give_harc1());
    std::vector<var> normal = {u};
    std::vector<var> negiert = {w};
    harc as(normal, negiert, H.num_harcs());
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
}


