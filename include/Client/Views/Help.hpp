/*
 * Author: Ernie Chu
 * Filename: Help.hpp
 * Description: The Help view header.
 */

#pragma once
#include <Client/Views/Include.hpp>
#include <fstream> 

namespace view {
    
class Help {
public:
    Help() = delete;
    Help(
        const std::size_t& H,
        const std::size_t& W,
        const std::vector<std::string>& O
    );
    ~Help();
    void show();
private:
    const std::size_t HEIGHT, WIDTH;
    const std::vector<std::string> OPT;
    WINDOW* win;
};

}
