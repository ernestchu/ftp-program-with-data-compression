/*
 * Author: Ernie Chu
 * Filename: Home.hpp
 * Description: The Home view header.
 */

#pragma once
#include <Client/Views/Include.hpp>

namespace view {
    
class Home {
public:
    Home() = delete;
    Home(
        const std::size_t& H,
        const std::size_t& W,
        const std::vector<std::string>& O
    );
    ~Home();
    int menu();
private:
    const std::size_t HEIGHT, WIDTH;
    const std::vector<std::string> OPT;
    WINDOW* win;
};

}
