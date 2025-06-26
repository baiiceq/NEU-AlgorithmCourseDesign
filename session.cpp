// session.cpp
#include "session.h"
#include "sha256.h"   // 用于生成哈希
#include <iostream>
#include <sstream>
#include <limits>    // 用于 std::numeric_limits
#include <algorithm> // 用于 std::all_of
#include <cctype>    // 用于 ::isdigit

// 主运行方法，协调整个流程
void CrackingSession::run(const std::string& target_password) {
    setupTargetPassword(target_password);
    //gatherClues();
    executeAndReport();
}

// 实现 getTargetHash 函数，它动态计算并返回原密码的哈希值。
std::string CrackingSession::getTargetHash() const {
    // 如果 originalPassword_ 为空 (例如，如果 setup 失败), 则返回空哈希
    if (originalPassword_.empty()) {
        return "";
    }
    return sha256::hash_string(originalPassword_);
}

// 步骤 1: 接收一个密码参数并设置目标哈希值
void CrackingSession::setupTargetPassword(const std::string& password_to_set) {
    // 验证传入的密码是否为三位数字
    if (password_to_set.length() == 3 && std::all_of(password_to_set.begin(), password_to_set.end(), ::isdigit)) {
        originalPassword_ = password_to_set; // 输入有效，设置密码
    }
    else {
        // 由于没有交互，如果密码无效，则打印错误并终止程序
        std::cerr << "错误: 提供的密码 \"" << password_to_set << "\" 无效。密码必须是三位数字。程序将退出。\n";
        exit(1);
    }

    // 使用 getTargetHash() 函数计算并显示哈希值
    std::cout << "原密码 '" << originalPassword_ << "' 的SHA-256哈希值为:\n" << getTargetHash() << std::endl;
    std::cout << "----------------------------------------------------------------\n" << std::endl;
}

// 步骤 2: 接收破译线索 (此部分保持不变，仍然是交互式)
void CrackingSession::gatherClues() {
    int x;
    std::cout << "请输入线索数量 (0-3，输入0代表无任何线索): ";
    std::cin >> x;

    if (x < 0 || x > 3) {
        std::cerr << "错误：线索数量必须在 0 到 3 之间。程序将退出。" << std::endl;
        exit(1);
    }

    if (x == 0) {
        std::cout << "无任何线索，将进行暴力破解。" << std::endl;
        return;
    }

    // 消耗掉 cin 留下的换行符，以便 getline 正常工作
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    clues_.clear(); // 确保线索向量是空的
    std::cout << "请输入 " << x << " 条线索, 每条一行 (例如, [-1,-1] 或 [1,0]):" << std::endl;
    for (int i = 0; i < x; ++i) {
        std::string line;
        std::getline(std::cin, line);
        std::vector<int> parsed = parseClue(line);
        if (!parsed.empty()) {
            clues_.push_back(parsed);
        }
        else {
            // 如果解析失败，让用户重新输入当前这条线索
            i--;
        }
    }
}

// 步骤 3 & 4: 创建破译器、执行破译并显示结果
void CrackingSession::executeAndReport() {
    // 使用 getTargetHash() 获取哈希值来创建破译器实例
    PasswordCracker cracker(getTargetHash());

    // 添加所有线索
    for (const auto& clue : clues_) {
        cracker.addClue(clue);
    }

    std::cout << "\n正在根据线索破译密码..." << std::endl;

    // 开始破译
    std::string result = cracker.crack();

    // 获取因密码错误而扣除的金币总数
    int coinsDeducted = cracker.getDeductedCoins();

    // 获取所有有效尝试
    const auto& attempts = cracker.getAttemptedPasswords();

    std::cout << "----------------------------------------" << std::endl;

    // 打印所有尝试过的密码
    std::cout << "所有符合线索的有效猜测 (" << attempts.size() << " 个):" << std::endl;
    if (attempts.empty()) {
        std::cout << "(无)" << std::endl;
    }
    else {
        // 为了美观，每10个换一行
        for (size_t i = 0; i < attempts.size(); ++i) {
            std::cout << attempts[i] << ((i % 10 == 9 || i == attempts.size() - 1) ? "\n" : ", ");
        }
    }
    std::cout << std::endl; // 添加一个空行以增加可读性

    std::cout << "破译过程结束。总共扣除的金币数: " << coinsDeducted << std::endl;

    // 输出最终结果
    if (!result.empty()) {
        std::cout << "成功! 破译出的密码是: " << result << std::endl;
        // 验证破译结果是否与最开始输入的密码一致
        if (result == originalPassword_) {
            std::cout << "验证通过：破译结果与原密码一致。" << std::endl;
        }
        else {
            std::cout << "警告：破译结果与原密码不一致！可能是哈希碰撞或线索/代码逻辑错误。" << std::endl;
        }
    }
    else {
        std::cout << "失败。未找到符合所有线索和哈希值的密码。" << std::endl;
    }
    std::cout << "----------------------------------------" << std::endl;
}

// 从 main.cpp 移过来的辅助函数
std::vector<int> CrackingSession::parseClue(const std::string& line) {
    std::vector<int> clue;
    std::string numbers_part;
    // 提取中括号内的内容
    size_t start = line.find('[');
    size_t end = line.find(']');
    if (start != std::string::npos && end != std::string::npos) {
        numbers_part = line.substr(start + 1, end - start - 1);
    }
    else {
        std::cerr << "错误：无效的线索格式，缺少[]: " << line << std::endl;
        return {};
    }

    std::stringstream ss(numbers_part);
    std::string item;
    while (std::getline(ss, item, ',')) {
        try {
            // 清除可能存在的空格
            item.erase(std::remove_if(item.begin(), item.end(), ::isspace), item.end());
            clue.push_back(std::stoi(item));
        }
        catch (const std::exception& e) {
            std::cerr << "错误：无效的线索格式，无法解析数字: " << line << std::endl;
            return {}; // 返回空向量表示解析失败
        }
    }
    return clue;
}