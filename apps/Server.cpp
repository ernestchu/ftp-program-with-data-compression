/*
 * Author: Ernie Chu
 * Filename: Services.cpp
 */

#include <Server/Services.hpp>

const int BACKLOG = 5;
const int BUFSIZE = 512;

int main(int argc, char* argv[]) {
    std::cout << "Starting Server ...\n";
    int port;
    if (argc <= 1) {
        std::cout << "Please specify the port to listen on: \n% ";
        std::cin >> port;
    } else if (argc == 2)
        port = atoi(argv[1]);
    else {
        std::cout << "Invalid arguments.\n";
        exit(1);
    }

    Services srvc(BACKLOG, BUFSIZE);
    srvc.listen(port);
    while (true) {
        std::cout << "\nListen on:" << std::endl;
        srvc.printHostAddrs(port);
        int clifd = srvc.accept();
        while (srvc.recv(clifd));
        close(clifd);
    }

    return 0;
}
