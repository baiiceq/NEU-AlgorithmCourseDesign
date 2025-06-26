#pragma once
// sha256.h
#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <vector>
#include <array>
#include <cstdint> // For uint32_t, uint64_t
#include <sstream>
#include <iomanip>

namespace sha256 {

    // 内部辅助函数，不对外暴露
    namespace {
        // 右循环移位
        inline uint32_t right_rotate(uint32_t value, unsigned int count) {
            return (value >> count) | (value << (32 - count));
        }
    }

    // 主计算函数，此函数现在总是使用固定的盐值进行哈希。
    inline std::string hash_string(const std::string& input) {
        // --- MODIFICATION START ---
        // 定义固定的盐值
        static const std::string salt = {
            (char)0xb2, (char)0x53, (char)0x22, (char)0x65, (char)0x7d, (char)0xdf, (char)0xb0, (char)0xfe,
            (char)0x9c, (char)0xde, (char)0xde, (char)0xfe, (char)0xf3, (char)0x1d, (char)0xdc, (char)0x3e
        };

        // 将盐值和输入拼接在一起 (salt + input)
        std::string salted_input = salt + input;
        // --- MODIFICATION END ---


        // 定义SHA-256常量
        static const std::array<uint32_t, 64> k = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
            0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
            0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
        };

        // 定义初始哈希值
        std::array<uint32_t, 8> h = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
        };

        // 1. 预处理 (填充)
        // --- MODIFICATION: Use salted_input instead of input ---
        std::vector<unsigned char> msg(salted_input.begin(), salted_input.end());
        uint64_t original_len_bits = msg.size() * 8;
        msg.push_back(0x80); // 添加 '1' bit
        while ((msg.size() * 8) % 512 != 448) {
            msg.push_back(0x00); // 添加 '0' bits
        }
        // 添加原始长度 (64-bit)
        for (int i = 7; i >= 0; --i) {
            msg.push_back((original_len_bits >> (i * 8)) & 0xff);
        }

        // 2. 将消息分解成 512-bit 的块进行处理
        for (size_t i = 0; i < msg.size(); i += 64) {
            std::array<uint32_t, 64> w;
            // a. 将块分解为16个32-bit字
            for (int j = 0; j < 16; ++j) {
                w[j] = (static_cast<uint32_t>(msg[i + 4 * j]) << 24) | (static_cast<uint32_t>(msg[i + 4 * j + 1]) << 16) | (static_cast<uint32_t>(msg[i + 4 * j + 2]) << 8) | static_cast<uint32_t>(msg[i + 4 * j + 3]);
            }

            // b. 扩展为64个32-bit字
            for (int j = 16; j < 64; ++j) {
                uint32_t s0 = right_rotate(w[j - 15], 7) ^ right_rotate(w[j - 15], 18) ^ (w[j - 15] >> 3);
                uint32_t s1 = right_rotate(w[j - 2], 17) ^ right_rotate(w[j - 2], 19) ^ (w[j - 2] >> 10);
                w[j] = w[j - 16] + s0 + w[j - 7] + s1;
            }

            // c. 初始化此块的哈希值
            std::array<uint32_t, 8> temp_h = h;

            // d. 主循环
            for (int j = 0; j < 64; ++j) {
                uint32_t S1 = right_rotate(temp_h[4], 6) ^ right_rotate(temp_h[4], 11) ^ right_rotate(temp_h[4], 25);
                uint32_t ch = (temp_h[4] & temp_h[5]) ^ (~temp_h[4] & temp_h[6]);
                uint32_t temp1 = temp_h[7] + S1 + ch + k[j] + w[j];
                uint32_t S0 = right_rotate(temp_h[0], 2) ^ right_rotate(temp_h[0], 13) ^ right_rotate(temp_h[0], 22);
                uint32_t maj = (temp_h[0] & temp_h[1]) ^ (temp_h[0] & temp_h[2]) ^ (temp_h[1] & temp_h[2]);
                uint32_t temp2 = S0 + maj;

                temp_h[7] = temp_h[6];
                temp_h[6] = temp_h[5];
                temp_h[5] = temp_h[4];
                temp_h[4] = temp_h[3] + temp1;
                temp_h[3] = temp_h[2];
                temp_h[2] = temp_h[1];
                temp_h[1] = temp_h[0];
                temp_h[0] = temp1 + temp2;
            }

            // e. 更新哈希值
            for (int j = 0; j < 8; ++j) {
                h[j] += temp_h[j];
            }
        }

        // 3. 生成最终的哈希值 (摘要)
        std::stringstream ss;
        for (int i = 0; i < 8; ++i) {
            ss << std::hex << std::setw(8) << std::setfill('0') << h[i];
        }
        return ss.str();
    }
} // namespace sha256

#endif // SHA256_H