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
               Predecessor.push_back(i);
               AD.push_back(empty);
               L.push_back(null);
          }
          belegung[var_num] = wahr;
          belegung[var_num-1] = falsch;
          for (int i=1; i < var_num+1; i++){
               vars.push_back(var(i));
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
     }

     else std::cout << "Unable to open file"; 

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
     for (int i=0; i < hgraph.size(); i++){
          if(hgraph[i].verify(belegung) == false){
               return false;
          }
     }
     return true;
}

bool HG::verify_strict(){                               
     for (int i=0; i < hgraph.size(); i++){
          if(hgraph[i].verify_strict(belegung) == false){
               std::cout << hgraph[i].get_pos() << std::endl;
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
    if(hgraph.size() == 0){
          return true;
    }
    return false;
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
     std::queue<int> q;
     for(var & v : vars){
          v.set_value(0);
     }
     for(int i : h){
          q.push(i);
          vars[i].set_value(vars[i].get_value()+1);
     }
     while(q.empty() == false){
          int a = q.front();
          q.push(Predecessor[a]);
          vars[a].set_value(vars[a].get_value()+1);
          if(vars[a].get_value() == limit){
               return vars[a];
          }
          q.pop();
     }
     return var(0);
}

void HG::set_V(){
     for (harc & a : hgraph){
          a.set_V(a.give_harc1().size());
     }
}

bool HG::Branching_True(int p){
     belegung[p] = wahr;
     if(verify() == false){
          return false;
     }
     int var_index = p;
     for(harc & h : BS[var_index]){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph.erase(hgraph.begin()+i);         //erfüllte clauseln entfernen 
                    break;
               }
          }
     }
     for(harc & h : FS[var_index]){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph[i].remove_neg(var_index);        //gelöschte variable aus unerfüllte Klauseln entfernen
                    break;
               }
          }
     }
     for(int i=0; i<hgraph.size(); i++){
          if(hgraph[i].size() == 0){
               return false;                           // Die Idee bei die return false ist, das falls dieser Fall vorkommt dann ist ein solcher Branching illegal, da keine Variablen bleiben die die Klausel erfüllen könnten
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
                    return false;
               }
          }
     }
     for(int i=0; i<FS.size(); i++){              //hier werden die FS und BS updated da sie sonst nicht übereinstimmen
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
     return true;
}

bool HG::Branching_False(int p){
     belegung[p] = falsch;
     if(verify() == false){
          return false;
     }
     int var_index = p;
     for(harc & h : FS[var_index]){
          for(int i=0; i<hgraph.size(); i++){
               if(hgraph[i].get_pos() == h.get_pos()){
                    hgraph.erase(hgraph.begin()+i);
                    break;
               }
          }
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
               return false;
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
                    return false;
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
     return true;
}


int HG::minimal_harc(){
     int size = var_num;
     for(harc a : hgraph){
          size = std::min(size, a.size());
     }
     return size;
}


bool HG::SimplifyUR(){
     for(int i=0; i<hgraph.size(); i++){
          if (hgraph[i].size() == 2){
               if(hgraph[i].give_harc2()[0][0] == var_num){
                    //std::cout << "Unit Resolution True - " << hgraph[i].give_harc2()[1][0] << std::endl;
                    bool branch = Branching_True(hgraph[i].give_harc2()[1][0]);
                    if(branch == false){
                         return false;
                    }
                    i=0;
               }
               else if(hgraph[i].give_harc2()[1][0] == var_num-1){
                    //std::cout << "Unit Resolution False - " << hgraph[i].give_harc2()[0][0] << std::endl;
                    bool branch = Branching_False(hgraph[i].give_harc2()[0][0]);
                    if(branch == false){
                         return false;
                    }
                    i=0;
               }
          }
     }
     return true;
}

void HG::set_P(var v, var u){
     Predecessor[v.get_var()] = u.get_var();
}

var HG::get_P(var v){
     return Predecessor[v.get_var()];
}

std::vector<var> HG::get_vars(){
     return vars;
}

void HG::set_valuesT1(var v){
     int i = v.get_var();
     while(Predecessor[i] != i){
          if(L[i] == wahr){
               L[i] = falsch;
          }
          else if(L[i] == falsch){
               L[i] = wahr;
          }
          i = Predecessor[i];
     }
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
          for(harc a : FS[q.front()]){
               if(besuchtharc[a.get_pos()] == false){
                    bool tailbesucht = true;
                    for(var v : a.give_harc2()[0]){
                         if(besucht[v.get_var()] == false){
                              tailbesucht = false;
                         }
                    }
                    if(tailbesucht == true){
                         besuchtharc[a.get_pos()] == true;
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
     for(harc & h : hgraph){
          h.set_unused();
     }
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
          for(harc a : FS[q.front()]){
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

std::vector<std::vector<harc>>& HG::get_FS(){
     return FS;
}
        
        
std::vector<std::vector<harc>>& HG::get_BS(){
     return BS;
}

void HG::set_L(std::vector<bel> vec){
     L = vec;
}

bool HG::set_L(int u, bel l){
     if(u == var_num or u == var_num-1){
          return false;
     }
     L[u] = l;
     return true;
}

std::vector<bel> HG::get_L(){
     return L;
}

void HG::set_AD(int u, std::vector<harc> vec){
     AD[u] = vec;
}
        
std::vector<harc> HG::get_AD(int u){
     return AD[u];
}

std::vector<harc>& HG::get_hgraph(){
     return hgraph;
} 

std::vector<bool> HG::get_visited(){
     return visited;
}
        
void HG::set_visited(bool l, int u){
     visited[u] = l;
}

void HG::increase_harcs(){
     harcs += 1;
}

void HG::set_for_relaxation(){
     Predecessor = {};
     AD = {};
     L = {};
     visited = {};
     for(int i=0; i<var_num+1; i++){
          Predecessor.push_back(i);
          std::vector<harc> empty;
          AD.push_back(empty);
          L.push_back(null);
          visited.push_back(false);
     }
     L[var_num] = wahr;
     L[var_num-1] = falsch;
     S = {};
     std::vector<bool> Sbool;
     for(int i=0; i<var_num+1; i++){
          Sbool.push_back(false);
     }
     for(int i=0; i<hgraph.size(); i++){
          for(int v: hgraph[i].give_harc1()){
               Sbool[v] = true;
          }
     }
     for(int i=0; i<Sbool.size(); i++){
          if(Sbool[i] == true){
               S.push_back(i);
          } 
     }
     set_V();
}

std::vector<int> HG::get_S(){
     return S;
}

int HG::get_harcs(){
     return harcs;
}

void HG::set_values_Deduce(){
     Predecessor = {};
     AD = {};
     for(int i=0; i<var_num+1; i++){
          Predecessor.push_back(i);
          std::vector<harc> empty;
          AD.push_back(empty);
     }
     set_V();
}