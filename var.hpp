#pragma once
#include <iostream>
#include <vector>
#include "harc.hpp"

class var{
    public:
    //var(var const &) = delete;
    var(int i);
    var();
    void set_value(int i);
    int get_value() const;
    int get_var() const;
    std::vector<harc>& get_FS();
    std::vector<harc>& get_BS();
    void set_FS(std::vector<harc> h);
    void set_BS(std::vector<harc> h);
    int W(int k);

    private:
    int variable;
    int value;
    std::vector<harc> FS;
    std::vector<harc> BS;
};