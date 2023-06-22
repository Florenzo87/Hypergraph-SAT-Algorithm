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
          for (int i=0; i < var_num+1; i++){
               std::vector<harc> empty;
               FS.push_back(empty);
               BS.push_back(empty);
               belegung.push_back(null);
          }
          for (int i=1; i < var_num+1; i++){
               vars.push_back(var(i));
               std::vector<harc> empty;
               harcs_der_variable.push_back(empty);
               Predecessor.push_back(var(i));
               //index_harc.push_back(i-1);
          }
          //vars.push_back(F); eigentlich schon addiert oben
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
          for(var& v : vars){
               v.set_FS(FS[v.get_var()]);
               v.set_BS(BS[v.get_var()]);
          }
     }

     else std::cout << "Unable to open file"; 

}

HG::HG(std::vector<std::vector<int>> vec){
          harcs = vec.size();
          std::vector<int> besuche;
          for(std::vector<int> h : vec){
               for(int i : h){
                    bool besucht = false;
                    for(int j : besuche){
                         if(i == j){
                              besucht = true;
                         }
                    }
                    if(besucht == false){
                         besuche.push_back(i);
                    }
               }
          }
          var_num = besuche.size();
          F = var(var_num+1); //sie werden zu zwei weniger da wir var_num vergrößern
          T = var(var_num+2);
          var_num += 2;
          std::vector<std::vector<harc>> FS(var_num+1);
          std::vector<std::vector<harc>> BS(var_num+1);
          for (int i=0; i < var_num+1; i++){
               vars.push_back(var(besuche[i]));
               belegung.push_back(null);
               std::vector<harc> empty;
               harcs_der_variable.push_back(empty);
               Predecessor.push_back(var(besuche[i]));
               //index_harc.push_back(i-1);
          }
          //vars.push_back(F); eigentlich schon addiert oben
          //vars.push_back(T);
          for(int clausel = 0; clausel < harcs; clausel++){
                    std::vector<int> head = {};
                    std::vector<int> tail = {};
                    std::vector<int> FSv = {};
                    std::vector<int> BSv = {};
                    std::vector<int> harcs_der_variable_v = {};
                    for(int n : vec[clausel]) {
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
          }                 
          for(var& v : vars){
               v.set_FS(FS[v.get_var()]);
               v.set_BS(BS[v.get_var()]);
          }
}
          


HG::HG(std::vector<harc> vec, int i){
     hgraph = vec;
     var_num = i;
     harcs = hgraph.size();
}

void HG::print() const{
     std::cout << var_num << " " << harcs << std::endl;
     for (harc h : hgraph){
          std::cout << h.get_pos() << " - ";
          h.print();
     }
}

void HG::printFSBS(){
     for(var & v : vars){
          std::cout << v.get_var() << std::endl;
          std::cout << "FS: ";
          std::cout << FS[v.get_var()].size() << " ";
          for(harc h : FS[v.get_var()]){
               std::cout << h.get_pos() << " ";
          }
          std::cout << std::endl;
          std::cout << "BS: ";
          std::cout << BS[v.get_var()].size() << " ";
          for(harc h : BS[v.get_var()]){
               std::cout << h.get_pos() << " ";
          }
          std::cout << std::endl;
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

void HG::Branching_True(int p){
     int var_index = p;
     for(harc & h : BS[var_index]){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph.erase(hgraph.begin()+i);
                    break;
               }
          }
          harcs -=1;
     }
     for(harc & h : FS[var_index]){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph[i].remove_neg(var_index);
                    break;
               }
          }
     }
     for(int i=0; i<hgraph.size(); i++){
          if(hgraph[i].size() == 0){
               hgraph.erase(hgraph.begin()+i);
               harcs -= 1;
               i -= 1;
          }
          else{
               if(hgraph[i].give_harc2()[0].size() == 0){
                    std::vector<int> T;
                    T.push_back(var_num);
                    hgraph[i].set_Tail(T);
               }
               if(hgraph[i].give_harc2()[1].size() == 0){
                    std::vector<int> F;
                    F.push_back(var_num-1);
                    hgraph[i].set_Head(F);
               }
               if(hgraph[i].give_harc2()[0].size() == 1 && hgraph[i].give_harc2()[1].size() == 1 && hgraph[i].give_harc2()[0][0] == var_num && hgraph[i].give_harc2()[1][0] == var_num-1){
                    std::cout << i << std::endl;
                    hgraph.erase(hgraph.begin()+i);
                    harcs -= 1;
                    i -= 1;
               }
          }
     }
     for(int i=0; i<FS.size(); i++){
          std::vector<harc> empty;
          FS[i] = empty;
     }
     for(int i=0; i<BS.size(); i++){
          std::vector<harc> empty;
          BS[i] = empty;
     }
     for(harc & h : hgraph){
          for(int n : h.give_harc2()[0]) { 
               FS[n].push_back(h);
          }
          for(int n : h.give_harc2()[1]) { 
               BS[n].push_back(h);
          }  
     }
     for(var& v : vars){
          v.set_FS(FS[v.get_var()]);
          v.set_BS(BS[v.get_var()]);
     }
}

void HG::Branching_False(int p){
     int var_index = p;
     for(harc & h : FS[var_index]){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph.erase(hgraph.begin()+i);
                    break;
               }
          }
          harcs -=1;
     }
     for(harc & h : BS[var_index]){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph[i].remove_nor(var_index);
                    break;
               }
          }
     }
     for(int i=0; i<hgraph.size(); i++){
          if(hgraph[i].size() == 0){
               hgraph.erase(hgraph.begin()+i);
               harcs -= 1;
               i -= 1;
          }
          else{
               if(hgraph[i].give_harc2()[0].size() == 0){
               std::vector<int> T;
               T.push_back(var_num);
               hgraph[i].set_Tail(T);
               }
               if(hgraph[i].give_harc2()[1].size() == 0){
                    std::vector<int> F;
                    F.push_back(var_num-1);
                    hgraph[i].set_Head(F);
               }
               if(hgraph[i].give_harc2()[0].size() == 1 && hgraph[i].give_harc2()[1].size() == 1 && hgraph[i].give_harc2()[0][0] == var_num && hgraph[i].give_harc2()[1][0] == var_num-1){
                    hgraph.erase(hgraph.begin()+i);
                    harcs -= 1;
                    i -= 1;
               }
          }
     }
     
     for(int i=0; i<FS.size(); i++){
          std::vector<harc> empty;
          FS[i] = empty;
     }
     for(int i=0; i<BS.size(); i++){
          std::vector<harc> empty;
          BS[i] = empty;
     }
     for(harc & h : hgraph){
          for(int n : h.give_harc2()[0]) { 
               FS[n].push_back(h);
          }
          for(int n : h.give_harc2()[1]) { 
               BS[n].push_back(h);
          }  
     }
     for(var& v : vars){
          v.set_FS(FS[v.get_var()]);
          v.set_BS(BS[v.get_var()]);
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
          if(u.W(k) > max){
               max = u.W(k);
               p = u;
          }
     }
     return p;
}

bool HG::SimplifyUR(){
     for(harc & h : hgraph){
          if (h.size() == 2){
               std::cout << h.get_pos() << std::endl;
               if(h.give_harc2()[0][0] == var_num){
                    std::cout << h.give_harc2()[1][0] << std::endl;
                    Branching_True(h.give_harc2()[1][0]);
               }
               else if(h.give_harc2()[1][0] == var_num-1){
                    std::cout << h.give_harc2()[0][0] << std::endl;
                    Branching_False(h.give_harc2()[0][0]);
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
     std::vector<bool> A = Bbranching(var_num-1);
     if(A[var_num] == true){
          return true;
     }
     return false;
}

std::vector<bool> HG::Bbranching(int i){
     std::queue<int> q;
     q.push(i);
     std::vector<bool> besucht;
     for(int i=0; i<var_num+1; i++){
          besucht.push_back(false);
     }
     std::vector<bool> besuchtharc;
     for(int i=0; i<harcs; i++){
          besuchtharc.push_back(false);
     }
     while(q.empty() == false){
          for(harc a : vars[q.front()-1].get_FS()){
               if(besuchtharc[a.get_pos()] == false){
                    besuchtharc[a.get_pos()] == true;
                    bool tailbesucht = true;
                    for(var v : a.give_harc2()[0]){
                         if(besucht[v.get_var()] == false){
                              tailbesucht = false;
                         }
                    }
                    if(tailbesucht == true){
                         for(int i : a.give_harc2()[1]){
                              if(besucht[i] == false){
                                   besucht[i] = true;
                                   q.push(i);
                              }
                         }
                    }
               }
          }
          q.pop();
     }
     return besucht;
}

std::vector<int> HG::branchingFT(){
     int i = var_num-1;
     std::queue<int> q;
     q.push(i);
     std::vector<bool> besucht;
     for(int i=0; i<var_num+1; i++){
          besucht.push_back(false);
     }
     std::vector<bool> besuchtharc;
     for(int i=0; i<harcs; i++){
          besuchtharc.push_back(false);
     }
     while(q.empty() == false){
          for(harc a : vars[q.front()-1].get_FS()){
               if(besuchtharc[a.get_pos()] == false){
                    besuchtharc[a.get_pos()] == true;
                    bool tailbesucht = true;
                    for(var v : a.give_harc2()[0]){
                         if(besucht[v.get_var()] == false){
                              tailbesucht = false;
                         }
                    }
                    if(tailbesucht == true){
                         for(int i : a.give_harc2()[1]){
                              if(besucht[i] == false){
                                   if(i == var_num){
                                        return a.give_harc2()[0];
                                   }
                                   besucht[i] = true;
                                   q.push(i);
                              }
                         }
                    }
               }
          }
          q.pop();
     }
     std::vector<int> empty;
     return empty;
}

int HG::branching_var(int k){
     int max = 0;
     int p = 0;
     for(int i=1; i<var_num-1; i++){
          int FSk = 0;
          int BSk = 0;
          for(harc a : FS[i]){
               if (a.size() == k){
                    FSk += 1;
               }
          }
          for(harc a : BS[i]){
               if (a.size() == k){
                    BSk += 1;
               }
          }
          //std::cout << FSk << BSk << std::endl;
          int W = FSk + BSk + (1.5*std::min(BSk, FSk));
          if(W >= max){
               max = W;
               p = i;
          }
     }
     return p;
}