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
          var_num += 2;
          for (int i=0; i < var_num+1; i++){
               FS.push_back({});
               BS.push_back({});
               belegung.push_back(null);
               Predecessor.push_back(i);
               AD.push_back({});
               L.push_back(null);
               Sb.push_back(0);
               Sf.push_back(0);
          }
          belegung[var_num] = wahr;
          belegung[var_num-1] = falsch;
          for (int i=1; i < var_num+1; i++){
               vars.push_back(var(i));
          }
          int clausel = 0;
          std::vector<int> head = {};
          std::vector<int> tail = {};
          std::vector<int> FSv = {};
          std::vector<int> BSv = {};
          while ( getline (myfile,line) ){
               std::istringstream is(line);
               int n;
               //std::cout << clausel << std::endl;
               while( is >> n ) {
                    if (n != 0){
                         //std::cout << n << std::endl;
                         if (n>0){
                         bool add = true;
                         for(int i=0; i<head.size(); i++){
                              if(head[i] == n){
                                   add = false;
                              }
                         }
                         if(add == true){
                              head.push_back(n);
                              BSv.push_back(n);
                         }
                         }
                         else{
                         bool add = true;
                         for(int i=0; i<tail.size(); i++){
                              if(tail[i] == n){
                                   add = false;
                              }
                         }
                         if(add == true){
                              tail.push_back(-n);
                              FSv.push_back(-n);
                         }
                         }   
                    }
                    if(n == 0){
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
                         head = {};
                         tail = {};
                         FSv = {};
                         BSv = {};
                    }
               }    
          }
          myfile.close();
          for(int u=1; u<Sb.size(); u++){
               float beta = 6;
               float mu = 0;
               for(harc h : hgraph){
                    if(h.size() > 2){
                         mu += h.size()*std::pow(2,-h.size());
                    }
               }
               float Sbu = 0;
               for(harc h : BS[u]){
                    if(h.size() > 2){
                         Sbu += std::pow(2, -h.size());
                    }
               }
               Sb[u] = (beta/mu)*Sbu;
               float Sfu = 0;
               for(harc h : FS[u]){
                    if(h.size() > 2){
                         Sfu += std::pow(2, -h.size());
                    }
               }
               Sf[u] = (beta/mu)*Sfu;
          }
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

std::vector<bel>const & HG::get_belegung() const{                       //wirft der aktuelle Belegung zurück
     return belegung;
}

int HG::variables() const{                                       //wirft der Anzahl an Variablen zurück
     return var_num;
}

bool HG::empty(){
    if(hgraph.size() == 0){
          return true;
    }
    return false;
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
          FS[i] = {};
     }
     for(int i=0; i<BS.size(); i++){
          BS[i] = {};
     }
     for(harc & h : hgraph){
          //h.print();
          for(int i=0; i < h.give_harc2()[0].size(); i++) {
               //std::cout << "Tail" << h.get_pos() << " " << h.give_harc2()[0][i] << std::endl;
               FS[h.give_harc2()[0][i]].push_back(h);
          }
          for(int i=0; i < h.give_harc2()[1].size(); i++) {
               //std::cout << "Head" << h.get_pos() << " " << h.give_harc2()[1][i] << std::endl; 
               BS[h.give_harc2()[1][i]].push_back(h);
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
          FS[i] = {};
     }
     for(int i=0; i<BS.size(); i++){
          BS[i] = {};
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

void HG::set_P(int v, int u){
     Predecessor[v] = u;
}

std::vector<var>& HG::get_vars(){
     return vars;
}

void HG::set_valuesT1(var v){
     int i = v.get_var();
     std::vector<bool> besuche;
     for(int v=0; v<var_num+1; v++){
          besuche.push_back(false);
     }
     while(Predecessor[i] != i){
          if(L[i] == wahr){
               L[i] = falsch;
               besuche[i] = true;
          }
          else if(L[i] == falsch){
               L[i] = wahr;
               besuche[i] = true;
          }
          i = Predecessor[i];
     }
     for(int v=0; v<var_num+1; v++){
          if(besuche[v] == false){
               L[v] == null;
          }
     }
}

void HG::print(const std::vector<int>& vec){                               
        for(int i = 0; i < int(vec.size()); i++){
                std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
}

void HG::print(const std::vector<bool>& vec){                               
        for(int i = 0; i < int(vec.size()); i++){
                std::cout << vec[i] << " ";
        }
        std::cout << std::endl;
}

std::vector<int> HG::Restriction(){
     std::vector<int> A = Bbranching(var_num);
     if(A.size() != 0){
          return A;
     }
     return {};
}



std::vector<int> HG::Bbranching(int i){
     std::queue<int> q;
     q.push(i);
     std::vector<bool> besucht;
     for(int i=0; i<var_num+1; i++){
          besucht.push_back(false);
     }
     besucht[i] = true;
     std::vector<bool> besuchtharc;
     for(int i=0; i<harcs; i++){
          besuchtharc.push_back(false);
     }
     while(q.empty() == false){
          //std::cout <<"FS size:" << FS[q.front()].size() << std::endl;
          for(int h=0; h<FS[q.front()].size(); h++){
               harc a = FS[q.front()][h];
               //std::cout << "hyperarch " << a.get_pos() << " " << besuchtharc[a.get_pos()] << std::endl;
               if(besuchtharc[a.get_pos()] == false){
                    bool tailbesucht = true;
                    for(int i=0; i<a.give_harc2()[0].size(); i++){
                         //std::cout << "variable " << a.give_harc2()[0][i] << std::endl;
                         if(besucht[a.give_harc2()[0][i]] == false){
                              tailbesucht = false;
                         }
                    }
                    //std::cout << "tailbesucht " << tailbesucht << std::endl;
                    if(tailbesucht == true){
                         besuchtharc[a.get_pos()] = true;
                         for(int i=0; i<a.give_harc2()[1].size(); i++){
                              if(a.give_harc2()[1][i] == var_num-1){
                                   //std::cout << "abesuchte Knoten: ";
                                   //print(besucht);
                                   return a.give_harc2()[0];
                              }
                              besucht[a.give_harc2()[1][i]] = true;
                              //std::cout << "push " << a.give_harc2()[1][i] << std::endl;
                              q.push(a.give_harc2()[1][i]);
                         }
                         h = 0;
                    }
               }
          }
          //std::cout << "qpop" << q.front() << std::endl;
          q.pop();
     }
     //std::cout << "besuchte Knoten: ";
     //print(besucht);
     return {};
}

/*
std::vector<int> HG::branchingFT(){
     int i = var_num;
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
               std::cout << a.get_pos() << std::endl;
               if(besuchtharc[a.get_pos()] == false){
                    bool tailbesucht = true;
                    for(int i=0; i<a.give_harc2()[0].size(); i++){
                         std::cout << a.give_harc2()[0][i] << std::endl;
                         if(besucht[a.give_harc2()[0][i]] == false){
                              tailbesucht = false;
                         }
                    }
                    if(tailbesucht == true){
                         besuchtharc[a.get_pos()] == true;
                         for(int i=0; i<a.give_harc2()[1].size(); i++){
                              if(besucht[a.give_harc2()[1][i]] == false){
                                   if(a.give_harc2()[1][i] == var_num-1){
                                        return a.give_harc2()[0];
                                   }
                                   besucht[a.give_harc2()[1][i]] = true;
                                   q.push(besucht[a.give_harc2()[1][i]]);
                              }
                         }
                    }
               }
          }
          q.pop();
     }
     return {};
}
*/

std::vector<int> HG::branching_var(int k){
     int max = 0;
     int p = 0;
     int FSkp = 0;
     int BSkp = 0;
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
               FSkp = FSk;
               BSkp = BSk;
          }
     }
     std::vector<int> vec;
     vec.push_back(p);
     vec.push_back(FSkp);
     vec.push_back(BSkp);
     return vec;
}

std::vector<float> HG::branching_var2(int k){
     float max = 0;
     int p = 0;
     float Bkp;
     float Fkp;
     std::vector<float> fk;
     std::vector<float> f2;
     std::vector<float> bk;
     std::vector<float> b2;
     fk.push_back(0);
     f2.push_back(0);
     bk.push_back(0);
     b2.push_back(0);
     for(int i=1; i<var_num-1; i++){
          fk.push_back(Sf[i]);
          f2.push_back(Sf[i]);
          bk.push_back(Sf[i]);
          b2.push_back(Sf[i]);
     }
     for(int i=1; i<var_num-1; i++){
          for(harc a : FS[i]){
               if (a.size() == k){
                    fk[i] += fk[i]+1;
               }
               if (a.size() == 2){
                    f2[i] += f2[i]+1;
               }
          }
          for(harc a : BS[i]){
               if (a.size() == k){
                    bk[i] += bk[i]+1;
               }
               if (a.size() == 2){
                    b2[i] += b2[i]+1;
               }
          }
     }
     for(int i=1; i<var_num-1; i++){
          float Fku = fk[i];
          float Bku = bk[i];
          for(harc h : FS[i]){
               if(h.size() == 2 && h.give_harc2()[1][0] != var_num){
                    Fku += f2[h.give_harc2()[1][0]];
               }
               if(h.size() == 3 && h.give_harc2()[1][0] == var_num){
                    Fku += b2[h.give_harc2()[0][1]];
               }
          }
          for(harc h : FS[i]){
               if(h.size() == 2 && h.give_harc2()[0][0] != var_num-1){
                    Bku += b2[h.give_harc2()[0][0]];
               }
               if(h.size() == 3 && h.give_harc2()[0][0] == var_num-1){
                    Bku += f2[h.give_harc2()[1][1]];
               }
          }
          float alpha = 1.5;
          float W = Fku + Bku + alpha*(std::min(Fku, Bku));
          if(W > max){
               p = i;
               Bkp = Bku;
               Fkp = Fku;
          }
     }
     std::vector<float> vec;
     vec.push_back(p);
     vec.push_back(Fkp);
     vec.push_back(Bkp);
     return vec;
}

std::vector<std::vector<harc>>& HG::get_FS(){
     return FS;
}
        
        
std::vector<std::vector<harc>>& HG::get_BS(){
     return BS;
}

void HG::set_L(std::vector<bel> & vec){
     L = vec;
}

bool HG::set_L(int u, bel l){
     if(u == var_num or u == var_num-1){
          return false;
     }
     L[u] = l;
     return true;
}

std::vector<bel> const & HG::get_L(){
     return L;
}

void HG::set_AD(int u, std::vector<harc>& vec){
     AD[u] = vec;
}
        
std::vector<harc> const & HG::get_AD(int u){
     return AD[u];
}

std::vector<harc>& HG::get_hgraph(){
     return hgraph;
} 

std::vector<bool>const & HG::get_visited(){
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
          AD.push_back({});
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

std::vector<int> const & HG::get_S(){
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
          AD.push_back({});
     }
     set_V();
}