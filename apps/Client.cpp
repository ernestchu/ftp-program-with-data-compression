/*
 * Author: Ernie Chu
 * Filename: Client.cpp
 * Description: The main program for the Client app.
 */

#include <Client/Views/Views.hpp>
#include <Client/Services.hpp>

const std::size_t HEIGHT = 25;
const std::size_t WIDTH = 60;
const std::vector<std::string> OPT = { 
    "Link",
	"Send",
    "Help",
	"Exit",
};

int main(int argc, char** argv) {
    /* start ncurses */
	initscr();
    refresh();

    /* Views */
    view::Home homeView(HEIGHT,WIDTH, OPT);
    view::Link linkView(HEIGHT,WIDTH, OPT);
    view::Send sendView(HEIGHT,WIDTH, OPT);
    view::Help helpView(HEIGHT,WIDTH, OPT);
    view::Addr addr;

    /* Services */
    srvc::Link linkService;
    srvc::Send sendService;

    if ((argc!=1) && (argc!=3)) {
        std::cout << "Invalid argument.\n";
        endwin();
        exit(1);
    }
    if (argc == 3) {
        std::string IP(argv[1]);
        int port = std::atoi(argv[2]);
        linkService.link(IP, port);
        linkView.linkResult(linkService.getErr(), IP, port);
    }

    std::size_t optSel = 0; /* option selected */
    while (optSel!=OPT.size()-1) {
        int yMax, xMax;
        getmaxyx(stdscr, yMax, xMax);
        clear();
        printw("Welcome to the FTP client!\nPlease select one of the option below...");
        if (linkView.getIsConnection()) 
            mvprintw(
                yMax-1, 0, 
                "Connection established with %s:%d", 
                linkView.getSrvAddr().IP.c_str(),
                linkView.getSrvAddr().port
            );
        else {
            if (optSel==1)
                mvprintw(yMax-1, 0, "Connection must be established before sending in TCP");
        }

        optSel = homeView.menu();
        switch (optSel) {
            case 0: /* Link */
                addr = linkView.link();
                linkService.link(addr.IP, addr.port);
                linkView.linkResult(linkService.getErr());
                break;
            case 1: /* Send */
                if (linkView.getIsConnection()) { /* to promise a connection, i.e. valid fd */
                    sendView.send();
                    if (sendView.getIsFile()) {
                        sendService.send(linkService.getFd(), sendView.getFpath(), sendView.getEncoding());
                        sendView.sendResult(sendService.getErr(), sendService.getFsize(), sendService.getOrigFsize());
                    }
                }
                break;
            case 2: /* Help */
                helpView.show();
                break;
        }
    }
    endwin();
    return 0;
}
