#pragma once
#include "Component.h"
#include "Player.h"
#include "Components.h"
class PlayerMovmentComponent :
    public Component
{
private:
    std::shared_ptr<Player> player;
    int currentStandingSpace;
    Game* game;
public:
    PlayerMovmentComponent(std::shared_ptr<Player> players, Game* game);
    ~PlayerMovmentComponent();
    void update(float elapsedTime) override;
};

