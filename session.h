// session.h
#ifndef SESSION_H
#define SESSION_H

#include "password.h"
#include <string>
#include <vector>

class CrackingSession {
public:
    // 构造函数
    CrackingSession() = default;

    // 运行整个破译会话的主方法，接收一个目标密码
    void run(const std::string& target_password);


    // 获取由用户密码生成的SHA-256哈希值
    // 声明为 const 是因为此函数不会修改类的任何成员变量
    std::string getTargetHash() const;

    void set_clue(std::vector<std::vector<int>> clue)
    {
        clues_ = clue;
    }

private:
    // --- 私有成员变量 ---
    std::string originalPassword_;
    std::vector<std::vector<int>> clues_;

    // --- 私有辅助方法 ---
    // 接收一个密码并进行设置
    void setupTargetPassword(const std::string& password_to_set);
    void gatherClues();
    void executeAndReport();
    static std::vector<int> parseClue(const std::string& line);
};

#endif // SESSION_H