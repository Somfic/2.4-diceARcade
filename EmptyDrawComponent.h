#pragma once
#include "DrawComponent.h"
class EmptyDrawComponent :
    public DrawComponent
{
public:
    EmptyDrawComponent();
    ~EmptyDrawComponent();
    void draw() override;
};

