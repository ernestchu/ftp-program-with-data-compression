/*
 * Author: Ernie Chu
 * Filename: Send.cpp
 * Description: The Send view implementation.
 */

#include <Send.hpp>

view::Send::Send(
    const std::size_t& H,
    const std::size_t& W,
    const std::vector<std::string>& O
) : HEIGHT(H), WIDTH(W), OPT(O), isFile(false), encoding(0) {
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);
    win = newwin(
        HEIGHT,
        WIDTH,
        yMax/2 - HEIGHT/2,
        xMax/2 - WIDTH/2
    );
}
view::Send::~Send() {
    delwin(win);
}
void view::Send::send() {
    clear();
    wclear(win);
    attron(A_REVERSE);
    printw("Send");
    attroff(A_REVERSE);
    refresh();
    noecho();
    cbreak();
	keypad(win, TRUE); /* allow arrow key as input */
    curs_set(1); /* show cursor */
    box(win, 0, 0);


    /* get file name */
    echo();
    nocbreak();
    /* using buf cuz the limitation of ncurses */
    std::string buf;

    mvwprintw(win, 1, 1, "File path: ");
    wrefresh(win);
    int ch = wgetch(win);
    while (ch != '\n') {
        if (ch != '\\')
            buf.push_back(ch);
        ch = wgetch(win);
    }
    fpath = buf;
    while (fpath.back() == ' ') /* remove trailing space */
        fpath.pop_back();

    noecho();
    cbreak();
    curs_set(0); /* hide cursor */

    std::ifstream ifs(fpath);

    if (!ifs) { /* file not found */
        isFile = false;
        mvwprintw(win, 5, 1, (
            "Send file failed: " +
            std::string(std::strerror(errno))
        ).c_str());
        mvwprintw(win, HEIGHT-2, 1, "Press anything to continue.");
        wgetch(win);
        ifs.close();
        return;
    }
    /* successfully locate the file */
    isFile = true;
    ifs.close();

    /* select encoding */
    mvwprintw(win, 5, 1, "Encoding Algorithm:");
    std::vector<std::string> encodings = {
        " None",
        " Huffman Codes (Fixed-length Codeword)",
        " Huffman Codes (Variable-length Codeword)"
    };

    int key;
    std::size_t hl = 0; /* highlighted index */
    while (1) {
        for (std::size_t i = 0; i < encodings.size(); i++) {
            mvwprintw(win, i+6, 1, "%c", ((i==hl)? '>': ' '));
            mvwprintw(win, i+6, 2, encodings[i].c_str());
        }

        /* get command */
        key = wgetch(win);
        switch(key) {
            case KEY_UP:
            case 'k':
                if (hl == 0)
                    hl = encodings.size()-1;
                else hl--;
                break;
            case KEY_DOWN:
            case 'j':
                if (hl == encodings.size()-1)
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
    encoding = hl;
}

void  view::Send::sendResult(const int err, const unsigned int fsize, const int origFsize) {
    if (err)
        mvwprintw(win, 10, 1, (
            "Send failed: " +
            std::string(std::strerror(err))
        ).c_str());
    else {
        auto clock = time(nullptr);
        char* noEOL = ctime(&clock);
        noEOL[strlen(noEOL)-1] = '\0';
        mvwprintw(win, 10, 1, "File sent at %s", noEOL);
        if (encoding) {
            mvwprintw(win, 11, 1, "Original file size: %d bytes", origFsize);
            mvwprintw(win, 12, 1, "Compressed file size: %d bytes", fsize);
            mvwprintw(win, 13, 1, "Compression rate: %.2f%%", (double)fsize/(double)origFsize*100.0);
        } else
            mvwprintw(win, 11, 1, "Total %d bytes transmitted.", fsize);
    }
    mvwprintw(win, HEIGHT-2, 1, "Press anything to continue.");
    wgetch(win);
}
