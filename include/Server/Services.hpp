/*
 * Author: Ernie Chu
 * Filename: Services.hpp
 */

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <iomanip> /* setprecision */
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <Huffman/Huffman.hpp>

class Services {
public:
    Services() = delete;
    Services(const int back, const int buf);
    ~Services();
    void printHostAddrs(int port);
    void listen(const int port);
    int accept(); /* reutrn client's fd */
    bool recv(const int clifd);
private:
    const int BACKLOG;
    const int BUFSIZE;
    int fd;
};
