#pragma once
#include "animation.h"
#include "player.h"

enum class TileType
{
    Wall,
    Path,
    Start,
    End,
    Gold,
    Trap,
    Locker,
    Boss,
    Empty = -1,
    DOOR = -2
};

class Tile
{
public:
    Tile(TileType type = TileType::Path) : type(type) {}
    
    Tile(const Tile& t);

    virtual Tile* clone() const
    {
        return new Tile(*this);
    }

    virtual ~Tile() {}

    TileType get_type() const { return type; }
    void set_type(TileType new_type) { type = new_type; }
    
    void set_pos(Vector2 pos)
    {
        this->pos = pos;
    }

    virtual void on_render(bool is_show_resource);

    virtual void on_update(int delta);

    virtual void on_enter(Player& player);

    bool is_walkable() const
    {
        return type != TileType::Wall;
    }

protected:
    TileType type;
    Vector2 pos;
};


class Gold : public Tile
{
public:
    Gold(int value = 5);

    Gold(const Gold& g);

    Tile* clone() const override 
    {
        return new Gold(*this);  
    }

	int get_value() const { return value; }
	void set_value(int new_value) { value = new_value; }

	void set_triggered(bool triggered) { is_triggered = triggered; }

    void on_render(bool is_show_resource) override;
    void on_update(int delta) override;
	void on_enter(Player& player) override;

private:
	int value;  // 金币的价值
	bool is_triggered = false;  // 是否被触发过

    Animation anim_gold;

    const int OFFSET_X = 20;
    const int OFFSET_Y = 30;
};

class Trap : public Tile
{
public:
    Trap(int damage = 10);
    Trap(const Trap& t);

    Tile* clone() const override
    {
        return new Trap(*this);
    }

	int get_damage() const { return damage; }
	void set_damage(int new_damage) { damage = new_damage; }

    void on_render(bool is_show_resource) override;
    void on_update(int delta) override;
    void on_enter(Player& player) override;


private:
	int damage;  // 陷阱造成的伤害

    Animation anim_trap;

    const int OFFSET_X = 20;
    const int OFFSET_Y = 36;
};

class Start : public Tile
{
public:
	Start() : Tile(TileType::Start) {}

    Start(const Start& s) : Tile(s){}

    Tile* clone() const override
    {
        return new Start(*this);
    }

    void on_render(bool is_show_resource) override;

private:
	const int OFFSET_X = 20;
	const int OFFSET_Y = 36;
};

class End : public Tile
{
public:
    End() : Tile(TileType::End) {}

    End(const Start& e) : Tile(e) {}

    Tile* clone() const override
    {
        return new End(*this);
    }

    void on_render(bool is_show_resource) override;
    void on_enter(Player& player) override;

private:
    const int OFFSET_X = 20;
    const int OFFSET_Y = 36;
};

class Locker : public Tile
{
public:
    Locker();

    Locker(const Locker& other)
        : Tile(other), 
        clue(other.clue),
        password(other.password),
        is_triggered(other.is_triggered)
    {}

    Tile* clone() const override 
    {
        return new Locker(*this);
    }

    void on_render(bool is_show_resource) override;
    void on_enter(Player& player) override;

    std::vector<std::vector<int> > get_clue() 
    {
        return clue; 
    }

    void set_clue(std::vector<std::vector<int> > c)
    {
        clue = c;
    }

    unsigned int get_password() { return password; }

    void set_password(unsigned int p)
    {
        password = p;
    }

private:
    std::vector<std::vector<int> > clue;
    unsigned int password;
    bool is_triggered = false;  // 是否被触发过

    void generate_clue();
private:
    const int OFFSET_X = 20;
    const int OFFSET_Y = 36;
};

class Boss : public Tile
{
public:
    Boss();

    Boss(const Boss& other)
        : Tile(other), 
        hps(other.hps),
        is_triggered(other.is_triggered),
        skills(other.skills)
    {}

    Tile* clone() const override 
    {
        return new Boss(*this);
    }

    std::vector<int> get_hps() const
    {
        return hps;
    }

    void set_hps(std::vector<int> hps)
    {
        this->hps = hps;
    }

    void generate_hps_and_skills();

    void on_render(bool is_show_resource) override;
    void on_enter(Player& player) override;

private:
    std::vector<int> hps;
    std::vector<std::pair<int, int> > skills;
    bool is_triggered = false;  // 是否被触发过
};