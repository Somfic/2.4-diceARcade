#pragma once
#include "Space.h"
class NormalSpace :
    public Space
{
public:
    NormalSpace(int number);
    ~NormalSpace();
    void onLand(Player& p) override;
};

