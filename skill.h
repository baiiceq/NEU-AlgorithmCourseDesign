#pragma once
#include <string>
#include <functional>

class Player;
class Boss;

class Skill
{
private:
    std::string name;
    int cooldown;
    int remainingCooldown; // 当前剩余冷却轮数
    std::function<void(Player&, Boss&)> effect;

public:
    Skill(const std::string& name, int damage, int cooldown);

    std::string getName() const;
    int getDamage() const;
    int getCooldown() const;
    int getRemainingCooldown() const;

    void resetCooldown();           // 冷却设置为 cooldown
    void reduceCooldown();          // 每回合-1
    bool isAvailable() const;       // 当前是否可用
};

