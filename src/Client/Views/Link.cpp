/*
 * Author: Ernie Chu
 * Filename: Link.cpp
 * Description: The Link view implementation.
 */

#include <Client/Views/Link.hpp>

view::Link::Link(
    const std::size_t& H,
    const std::size_t& W,
    const std::vector<std::string>& O
) : HEIGHT(H), WIDTH(W), OPT(O), isConnection(false) {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    win = newwin(
        HEIGHT,
        WIDTH,
        yMax/2 - HEIGHT/2,
        xMax/2 - WIDTH/2
    );
}
view::Link::~Link() {
    free(win);
}
view::Addr view::Link::link() {
    clear();
    wclear(win);
    attron(A_REVERSE);
    printw("Link");
    attroff(A_REVERSE);
    refresh();
    noecho();
    cbreak();
    curs_set(1); /* show cursor */
    box(win, 0, 0);
    wrefresh(win);

    /* get IP, port */
    echo();
    nocbreak();
    Addr addr;
    /* using buf cuz the limitation of ncurses */
    std::string buf;

    mvwprintw(win, 1, 1, "The server's IP (x.x.x.x): ");
    wrefresh(win);
    int ch = wgetch(win);
    while (ch != '\n') {
        buf.push_back(ch);
        ch = wgetch(win);
    }
    addr.IP = buf;
    buf.clear();
    mvwprintw(win, 2, 1, "Port (1024~49151): ");
    wrefresh(win);
    ch = wgetch(win);
    while (ch != '\n') {
        buf.push_back(ch);
        ch = wgetch(win);
    }
    addr.port = std::stoi(buf);
    srvAddr = addr;

    noecho();
    cbreak();
    curs_set(0); /* hide cursor */

    return addr;
}

void view::Link::linkResult(const int err) {
    if (err)
        mvwprintw(win, 4, 1, (
            "Link failed: " + 
            std::string(std::strerror(err))
        ).c_str());
    else
        mvwprintw(win, 4, 1, "Connection established");
    isConnection = (err==0);
    mvwprintw(win, HEIGHT-2, 1, "Press anything to continue.");
    wgetch(win);
}

void view::Link::linkResult(const int err, std::string IP, int port) {
    Addr addr;
    addr.IP = IP;
    addr.port = port;
    srvAddr = addr;
    linkResult(err);
}

