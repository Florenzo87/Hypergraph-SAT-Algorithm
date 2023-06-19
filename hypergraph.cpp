#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>
#include <queue>
#include "hypergraph.hpp"


HG::HG(const std::string filename){
     std::string line;
     std::ifstream myfile (filename);
     if (myfile.is_open())
     {
          while ( getline (myfile,line) )
          {
                    std::istringstream is(line);
                    std::string c;
                    is >> c;
                    if (c == "p"){
                         std::string cnf;
                         is >> cnf >> var_num >> harcs;
                         break;
                    }
          }
          F = var(var_num+1); //sie werden zu zwei weniger da wir var_num vergrößern
          T = var(var_num+2);
          var_num += 2;
          std::vector<std::vector<harc>> FS;
          std::vector<std::vector<harc>> BS;
          for (int i=1; i < var_num+1; i++){
               vars.push_back(var(i));
               belegung.push_back(null);
               std::vector<harc> empty;
               harcs_der_variable.push_back(empty);
               FS.push_back(empty);
               BS.push_back(empty);  
               Predecessor.push_back(var(i));
          }
          //vars.push_back(F);
          //vars.push_back(T);
          int clausel = 0;
          while ( getline (myfile,line) )
          {
                    std::vector<int> head = {};
                    std::vector<int> tail = {};
                    std::vector<int> FSv = {};
                    std::vector<int> BSv = {};
                    std::vector<int> harcs_der_variable_v = {};
                    std::istringstream is(line);
                    int n;
                    while( is >> n ) {
                         if (n != 0){
                                if (n>0){
                                    head.push_back(n);
                                    BSv.push_back(n);
                                }
                                else{
                                    tail.push_back(-n);
                                    FSv.push_back(-n);
                                }   
                         }
                    }
                    if(head.size() == 0){
                         head.push_back(var_num-1);
                         BSv.push_back(var_num-1);
                    }
                    if(tail.size() == 0){
                         tail.push_back(var_num);
                         FSv.push_back(var_num);
                    }
                    harc cls = harc(tail, head, clausel);
                    for (int v : FSv){
                         FS[v].push_back(cls);
                    }
                    for (int v : BSv){
                         BS[v].push_back(cls);
                    }
                    hgraph.push_back(cls);
                    clausel += 1;                      
          }
          myfile.close();
          for(var v : vars){
               v.set_FS(FS[v.get_var()]);
               v.set_BS(BS[v.get_var()]);
          }
     }

     else std::cout << "Unable to open file"; 

}

HG::HG(std::vector<harc> vec, int i){
     hgraph = vec;
     var_num = i;
     harcs = hgraph.size();
}

void HG::print() const{
     std::cout << var_num << " " << harcs << std::endl;
     for (harc h : hgraph){
          h.print();
     }
}

bool HG::verify() const{                               
     for (int i=0; i < harcs; i++){
          if(hgraph[i].verify(belegung) == false){
               return false;
          }
     }
     return true;
}

void HG::set_belegung(const std::vector<bel>& neubelegung){       //ändert die Belegung zum gegebenen Vektor, Achtung muss die richtige Länge haben
     belegung = neubelegung;
}

void HG::set_belegung(const int pos, const bel val){                   //ändert der Wert der Belegung an der gegebene Stelle zum gegebenen Wert
     belegung[pos] = val;
}

void HG::set_belegung(const int pos){                            //ändert der Wert der Belegung an der gegebenen Stelle zum entgegengesetztes Wert, 0->1 1->0 2->2
     if(belegung[pos] == falsch){
               belegung[pos] = wahr;
     }
     if(belegung[pos] == wahr){
               belegung[pos] = falsch;
     }
}

std::vector<bel> HG::get_belegung() const{                       //wirft der aktuelle Belegung zurück
     return belegung;
}

int HG::variables() const{                                       //wirft der Anzahl an Variablen zurück
     return var_num;
}

int HG::num_harcs() const{
     return harcs;
}

bool HG::empty(){
    for (int i=0; i < harcs; i++){
          if(hgraph[i].empty() == false){
               return false;
          }
     }
    return true;
}

harc& HG::give_harc(int i) {
     return hgraph[i];
}

std::vector<var> HG::give_harc_as_vec(int i) const{
     std::vector<int> vec1 = hgraph[i].give_harc1();
     std::vector<var> vec2;
     for(int i = 0; i<vec1.size(); i++){
          vec2.push_back(var(vec1[i]));
     }
     return vec2;
}

var HG::root(std::vector<int> h){
     int limit = h.size();
     std::queue<var> q;
     std::vector<std::vector<int>> checked;
     for(int i : h){
          var a = vars[i];
          q.push(a);
          a.set_value(a.get_value()+1);
     }
     while(q.empty() == false){
          var a = q.front();
          a.set_value(a.get_value()+1);
          if(a.get_value() == limit){
               return a;
          }
          std::vector<harc> vec1 = a.get_BS();
          for(harc b : vec1){
               std::vector<int> vec2 = b.give_harc2()[0];
               for(var c : vec2){
                    bool add = true;
                    for(int i = 0; i<checked[a.get_var()].size() ; i++){
                         if(c.get_var() == checked[a.get_var()][i]){
                              add = false;
                         }
                    }
                    if(add == true){
                         q.push(c);
                    }
               }
          }
          q.pop();
     }
     return var(0);
}

void HG::set_values(){
     for (harc a : hgraph){
          a.set_value(a.give_harc1().size());
     }
}

HG HG::binary(){
     std::vector<harc> hgraphneu;
     for(harc a : hgraph){
          if(a.give_harc2()[0].size() == 1 && a.give_harc2()[1].size() == 1){
               hgraphneu.push_back(a);
          }
     }
     HG H(hgraphneu, var_num);
     return H;
}

var HG::shrink(std::vector<int> h){
     int limit = h.size();
     std::queue<var> q;
     std::vector<std::vector<int>> checked;
     for(int i : h){
          var a = vars[i];
          q.push(a);
          a.set_value(a.get_value()+1);
     }
     while(true){
          var a = q.front();
          a.set_value(a.get_value()+1);
          if(a.get_value() == limit){
               return a;
          }
          std::vector<harc> vec1 = a.get_FS();
          for(harc b : vec1){
               std::vector<int> vec2 = b.give_harc2()[1];
               for(int c : vec2){
                    bool add = true;
                    for(int i = 0; i<checked[a.get_var()].size() ; i++){
                         if(c == checked[a.get_var()][i]){
                              add = false;
                         }
                    }
                    if(add == true){
                         q.push(c);
                    }
               }
          }
          q.pop();
     }
     return var(0);
}

void HG::Branching_True(var p){
     for(int i=0; i<vars.size(); i++){
          if(vars[i].get_var() == p.get_var()){
               vars.erase(vars.begin()+i);
          }
     }
     for(harc h : p.get_FS()){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph.erase(hgraph.begin()+i);
                    break;
               }
          }
          harcs -=1;
     }
     for(harc h : p.get_BS()){
          h.remove_neg(p.get_var());
     }
}

void HG::Branching_False(var p){
     for(int i=0; i<vars.size(); i++){
          if(vars[i].get_var() == p.get_var()){
               vars.erase(vars.begin()+i);
          }
     }
     for(harc h : p.get_FS()){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph.erase(hgraph.begin()+i);
                    break;
               }
          }
          harcs -=1;
     }
     for(harc h : p.get_FS()){
          h.remove_nor(p.get_var());
     }
}

int HG::minimal_harc(){
     int size = var_num;
     for(harc a : hgraph){
          size = std::min(size, a.size());
     }
     return size;
}

var HG::branch_var(int k){
     int max = 0;
     var p(0);
     for(var u : vars){
          //std::cout << u.W(k) << std::endl;
          if(u.W(k) > max){
               max = u.W(k);
               p = u;
          }
     }
     return p;
}

bool HG::SimplifyUR(){
     for(harc h : hgraph){
          if (h.size() == 1){
               if(h.give_harc2()[0].size() == 1){
                    Branching_True(h.give_harc2()[0][0]);
               }
               else if(h.give_harc2()[1].size() == 1){
                    Branching_False(h.give_harc2()[1][0]);
               }
          }
     }
     if(hgraph.size() == 0){
          return false;
     }
     return true;
}

void HG::set_P(var v, var u){
     Predecessor[v.get_var()] = u;
}

var HG::get_P(var v){
     return Predecessor[v.get_var()];
}

std::vector<var> HG::get_vars(){
     return vars;
}

void HG::set_valuesT1(){
     
}
        
void HG::set_valuesP1(){

}

bool HG::Restriction(){
     std::vector<int> A = branching(var_num-1);
     for(int i: A){
          if(i == var_num){
               return false;
          }
     }
     return true;
}

std::vector<int> HG::branching(int i){
     std::queue<int> q;
     q.push(i);
     std::vector<int> besuche;
     while(q.empty() == false){
          for(harc a : vars[q.front()-1].get_FS()){
               for(int i : a.give_harc2()[0]){
                    bool besucht = false;
                    for(int j : besuche){
                         if(i == j){
                              besucht = true;
                         }
                    }
                    if(besucht == false){
                         besuche.push_back(i);
                         q.push(i);
                    }
               }
          }
          q.pop();
     }
     return besuche;
}