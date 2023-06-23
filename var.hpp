#pragma once
#include <iostream>
#include <vector>

class var{
    public:
    //var(var const &) = delete;
    var(int i);
    var();
    void set_value(int i);
    int get_value() const;
    int get_var() const;
    bool active() const;
    void deactivate();

    private:
    int variable;
    int value;
    bool activated;
};