/*
 * Author: Ernie Chu
 * Filename: Link.hpp
 * Description: The Link view header.
 */

#pragma once
#include <Client/Views/Include.hpp>
#include <fstream> 

namespace view {

struct Addr {
    std::string IP;
    int port;
};

class Link {
public:
    Link() = delete;
    Link(
        const std::size_t& H,
        const std::size_t& W,
        const std::vector<std::string>& O
    );
    ~Link();
    Addr link();
    void linkResult(const int err);
    void linkResult(const int err, std::string, int);
    Addr getSrvAddr() const { return srvAddr; }
    bool getIsConnection() const { return isConnection; }
private:
    const std::size_t HEIGHT, WIDTH;
    const std::vector<std::string> OPT;
    WINDOW* win;
    bool isConnection;
    Addr srvAddr;
};

}
