/*
 * Author: Ernie Chu
 * Filename: Huffman.hpp
 * Description: Huffman coding header.
 */

#pragma once
#include <string>
#include <unordered_map>
#include <queue> /* priority queue */
#include <vector>
#include <functional> /* greater */
#include <memory> /* shared_pty */
#include <bitset> /* bit string */
#include <tuple>
#include <fstream>
#include <cassert>

#define BYTESIZE 8

namespace huf {

struct Node {
    Node() {};
    Node(const char& c, const int& f) : ch(c), freq(f) {}

    char ch;
    int freq;
    std::shared_ptr<Node> left = nullptr;
    std::shared_ptr<Node> right = nullptr;
};

class Huffman {
public:
    Huffman() = delete;
    Huffman(const std::string& fname); /* used by the client to encode */
    Huffman(const std::unordered_map<char, std::string>& code) : codewords(code) {}; /* used by the server to decode */
    virtual ~Huffman() = default;
    std::tuple<
        std::string, 
        std::unordered_map<char, std::string>, 
        int
    > encode(const std::string& fname);
    virtual std::string decode(const std::string& fname, const int pad) = 0;
protected:
    std::string strToBitStr(std::string s);
    std::string bitStrToStr(std::string bs);
    std::unordered_map<char, unsigned int> freqs;
    std::unordered_map<char, std::string> codewords;
};

class HuffmanFix : public Huffman {
public:
    HuffmanFix(const std::string& fname);
    HuffmanFix(const std::unordered_map<char, std::string>& code) : Huffman(code) {}
    std::string decode(const std::string& fname, const int pad);
private:
    void increment(std::vector<char>& vec);
};

class HuffmanVar : public Huffman {
public:
    HuffmanVar(const std::string& fname);
    HuffmanVar(const std::unordered_map<char, std::string>& code) : Huffman(code) {}
    std::string decode(const std::string& fname, const int pad);
};

}
