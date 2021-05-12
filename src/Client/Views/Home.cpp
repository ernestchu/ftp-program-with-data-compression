/*
 * Author: Ernie Chu
 * Filename: Home.cpp
 * Description: The Home view implementation.
 */

#include <Home.hpp>

view::Home::Home(
    const std::size_t& H,
    const std::size_t& W,
    const std::vector<std::string>& O
) : HEIGHT(H), WIDTH(W), OPT(O) {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    win = newwin(
        HEIGHT,
        WIDTH,
        yMax/2 - HEIGHT/2,
        xMax/2 - WIDTH/2
    );
}
view::Home::~Home() {
    free(win);
}
int view::Home::menu() {
    wclear(win);
    refresh();
    noecho();
    cbreak();
	keypad(win, TRUE); /* allow arrow key as input */
    curs_set(0); /* hide cursor */
    box(win, 0, 0);

    int key;
    std::size_t hl = 0; /* highlighted index */
    while (1) {
        for (std::size_t i = 0; i < OPT.size(); i++) {
            if (i == hl)
                wattron(win, A_REVERSE);
            mvwprintw(win, i+1, 1, OPT[i].c_str());
            wattroff(win, A_REVERSE);
        }

        /* get command */
        key = wgetch(win);
        switch(key) {
            case KEY_UP:
            case 'k':
                if (hl == 0)
                    hl = OPT.size()-1;
                else hl--;
                break;
            case KEY_DOWN:
            case 'j':
                if (hl == OPT.size()-1)
                    hl = 0;
                else
                    hl++;
                break;
            default:
                break;
        }
        if (key == 10) /* return key */
            break;
    }
    return hl;
}
