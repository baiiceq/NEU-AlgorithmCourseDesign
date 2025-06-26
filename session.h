#pragma once
// session.h
#ifndef SESSION_H
#define SESSION_H

#include "password.h" // 需要使用 PasswordCracker
#include <string>
#include <vector>

class CrackingSession {
public:
    // 构造函数
    CrackingSession() = default;

    // 运行整个破译会话的主方法
    void run();

private:
    // --- 私有成员变量 ---
    std::string originalPassword_;
    std::string targetHash_;
    std::vector<std::vector<int>> clues_;

    // --- 私有辅助方法 ---
    // 提示用户输入原密码并生成哈希
    void setupTargetPassword();

    // 提示用户输入所有线索
    void gatherClues();

    // 执行破译并显示结果
    void executeAndReport();

    // 辅助函数，用于解析形如 "[-1,5,-1]" 的线索字符串
    // 从旧的 main.cpp 移入此类，设为 static 因为它不依赖于对象状态
    static std::vector<int> parseClue(const std::string& line);
};

#endif // SESSION_H
