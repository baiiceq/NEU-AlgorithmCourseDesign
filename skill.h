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
    int remainingCooldown; // ��ǰʣ����ȴ����
    std::function<void(Player&, Boss&)> effect;

public:
    Skill(const std::string& name, int damage, int cooldown);

    std::string getName() const;
    int getDamage() const;
    int getCooldown() const;
    int getRemainingCooldown() const;

    void resetCooldown();           // ��ȴ����Ϊ cooldown
    void reduceCooldown();          // ÿ�غ�-1
    bool isAvailable() const;       // ��ǰ�Ƿ����
};

