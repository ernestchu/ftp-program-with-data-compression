/*
 * Author: Ernie Chu
 * Filename: Send.hpp
 * Description: The Send view header.
 */

#pragma once
#include <Client/Views/Include.hpp>
#include <fstream> 

namespace view {
    
class Send {
public:
    Send() = delete;
    Send(
        const std::size_t& H,
        const std::size_t& W,
        const std::vector<std::string>& O
    );
    ~Send();
    void send();
    void sendResult(const int err, const unsigned int fsize, const int origFsize);
    bool getIsFile() const { return isFile; }
    std::string getFpath() const { return fpath; }
    int getEncoding() const { return encoding; }
private:
    const std::size_t HEIGHT, WIDTH;
    const std::vector<std::string> OPT;
    WINDOW* win;
    bool isFile;
    std::string fpath;
    int encoding;
};

}
