#pragma once
#include "Component.h"
#include "Player.h"
#include "Components.h"
class PlayerMovmentComponent :
    public Component
{
private:
    std::shared_ptr<Player> player;
    std::shared_ptr <Space> currentStandingSpace;
public:
    PlayerMovmentComponent(std::shared_ptr<Player> players);
    ~PlayerMovmentComponent();
    void update(float elapsedTime) override;
};

