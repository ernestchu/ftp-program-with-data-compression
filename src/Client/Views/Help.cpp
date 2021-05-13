/*
 * Author: Ernie Chu
 * Filename: Help.cpp
 * Description: The Help view implementation.
 */

#include <Help.hpp>

view::Help::Help(
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
view::Help::~Help() {
    delwin(win);
}
void view::Help::show() {
    clear();
    wclear(win);
    attron(A_REVERSE);
    printw("Help");
    attroff(A_REVERSE);
    refresh();
    noecho();
    cbreak();
	keypad(win, TRUE); /* allow arrow key as input */
    box(win, 0, 0);

    std::string readmePath = "HelpMsg.txt";
    std::ifstream ifs(readmePath);
    if (!ifs) {
        mvwprintw(win, 1, 1, (
            readmePath + ": " +
            std::string(std::strerror(errno))
        ).c_str());
        wgetch(win);
        exit(1);
    } else {
        int y = 1, c;
        wmove(win, y, 1);
        while ((c = ifs.get()) != EOF) {
            if (c=='\n')
                wmove(win, ++y, 1);
            else
                wprintw(win, "%c", c);
        }
    }
    ifs.close();
    mvwprintw(win, HEIGHT-2, 1, "Press anything to continue.");
    wgetch(win);
}
