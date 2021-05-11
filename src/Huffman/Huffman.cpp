/*
 * Author: Ernie Chu
 * Filename: Huffman.cpp
 * Description: Huffman coding implementation.
 */

#include <Huffman/Huffman.hpp>

/******************** Base Class Huffman ********************/

huf::Huffman::Huffman(const std::string& fname) {
    /* This constructor generate frequencies from file */
    freqs.clear();
    std::ifstream ifs(fname);
    while (ifs) {
        char ch;
        ifs.get(ch);
        if (freqs.find(ch) == freqs.end())
            freqs[ch] = 0;
        else
            freqs[ch]++;
    }
    ifs.close();
}
std::tuple<
        std::string, 
        std::unordered_map<char, std::string>, 
        int
> huf::Huffman::encode(const std::string& fname) {
    std::ifstream ifs(fname, std::ifstream::binary);
    std::string encoded;
    char ch;
    while (ifs.get(ch))
        encoded += codewords[ch];
    int pad = BYTESIZE - encoded.size() % BYTESIZE;
    encoded = std::string(pad, '0') + encoded; /* pad in the front */
    encoded = bitStrToStr(encoded);
    return std::make_tuple(encoded, codewords, pad);
}
std::string huf::Huffman::strToBitStr(std::string s) {
    std::string bs;
    for (char ch : s) {
        std::bitset<BYTESIZE> b(ch);
        bs += b.to_string();
    }
    return bs;
}
std::string huf::Huffman::bitStrToStr(std::string bs) {
    assert(!(bs.size()%BYTESIZE) && "size of bit string is not multiple of bytes size\n.");
    std::string s;
    for (int i = 0; i < (int)bs.size(); i+=BYTESIZE) {
        char ch = 0;
        /* binary value of the bit string */
        /* small endian */
        for (int j = 0; j < BYTESIZE; j++)
            ch += (bs[i+j]-48) * (1<<(BYTESIZE-j-1));
        s += ch;
    }
    return s;
}
/******************** Huffman Fixed Length ********************/


huf::HuffmanFix::HuffmanFix(const std::string& fname) :
    huf::Huffman::Huffman(fname) {
    /* increment till the length is long enough */
    size_t codeLen;
    for (codeLen = 1; (1<<codeLen) < (int)freqs.size(); codeLen++);
    std::vector<char> counter(codeLen, '0');
    for (auto const& [ch, _] : freqs) {
        codewords[ch] = std::string(counter.begin(), counter.end());
        increment(counter);
    }
}
void huf::HuffmanFix::increment(std::vector<char>& vec) {
    bool carry = false;
    /* lambda function */
    auto addByIndex = [&vec, &carry] (const int i) {
        if (vec[i] == '0') { vec[i] = '1'; carry = false; } 
        else { vec[i] = '0'; carry = true; }
    };
    addByIndex(vec.size()-1);
    for (int i = vec.size()-2; i >= 0; i--) {
        if (!carry) continue;
        addByIndex(i);
    }
}
std::string huf::HuffmanFix::decode(const std::string& fname, const int pad) {
    std::ifstream ifs(fname);
    std::istreambuf_iterator<char> begin(ifs), end;
    std::string encoded(begin, end); /* copy file into string */
    ifs.close();
    encoded = strToBitStr(encoded);
    encoded = encoded.substr(pad, encoded.size()-pad); /* drop padding */

    std::string decoded;
    std::unordered_map<std::string, char> revCodewords; /* reversed */
    for (auto const& [ch, code] : codewords)
        revCodewords[code] = ch;
    std::string code;
    auto const codeLen = revCodewords.begin()->first.size();
    assert(!(encoded.size()%codeLen) && "code length and file length not aligned.\n");

    auto i = codeLen;
    for (auto bit : encoded) {
        code += bit;
        if (--i == 0) {
            decoded += revCodewords[code];
            code.clear();
            i = codeLen;
        }
    }
    return decoded;
}

/******************** Huffman Variable Length ********************/

/* specialization needed for priority queue */
template<>
struct std::greater<std::shared_ptr<huf::Node>> {
    const bool operator()(
        const std::shared_ptr<huf::Node>& lhs,
        const std::shared_ptr<huf::Node>& rhs
    ) const {
        return lhs->freq > rhs->freq;
    }
};

huf::HuffmanVar::HuffmanVar(const std::string& fname) : 
    huf::Huffman::Huffman(fname) {
    /* This constructor generate huffman tree and codewords from frequencies */
    std::priority_queue<
        std::shared_ptr<huf::Node>,
        std::vector<std::shared_ptr<huf::Node>>,
        std::greater<std::shared_ptr<huf::Node>>
    > pq;
    for (auto const& [ch, freq] : freqs)
        pq.push(std::shared_ptr<huf::Node>(new huf::Node(ch, freq)));

    /* from CLRS page 431 */
    std::size_t n = pq.size();
    for (std::size_t i = 1; i < n; i++) {
        std::shared_ptr<huf::Node> z(new huf::Node);
        z->left = pq.top(); pq.pop();
        z->right = pq.top(); pq.pop();
        z->freq = z->left->freq + z->right->freq;
        pq.push(z);
    }
    /* root of the huffman tree */
    std::shared_ptr<Node> root = pq.top(); pq.pop();

    /* recursive lambda function */
    std::function<void(const std::shared_ptr<Node>, std::string)> genCode = 
        [&genCode, this] (const std::shared_ptr<Node> subroot, std::string code) {
        /* recursive till leaf node */
        if (subroot->left)
            genCode(subroot->left, code+"0");
        if (subroot->right)
            genCode(subroot->right, code+"1");
        if (!subroot->left && !subroot->right) /* leaf node */
            codewords[subroot->ch] = code;
    };
    codewords.clear();
    genCode(root, "");
}
std::string huf::HuffmanVar::decode(const std::string& fname, const int pad) {
    std::ifstream ifs(fname);
    std::istreambuf_iterator<char> begin(ifs), end;
    std::string encoded(begin, end); /* copy file into string */
    ifs.close();
    encoded = strToBitStr(encoded);
    encoded = encoded.substr(pad, encoded.size()-pad); /* drop padding */
    
    std::string decoded;
    std::unordered_map<std::string, char> revCodewords; /* reversed */
    for (auto const& [ch, code] : codewords)
        revCodewords[code] = ch;
    std::string code;
    for (auto bit : encoded) {
        code += bit;
        if (revCodewords.find(code) != revCodewords.end()) {
            decoded += revCodewords[code];
            code.clear();
        }
    }
    return decoded;
}
