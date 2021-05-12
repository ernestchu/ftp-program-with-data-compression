/*
 * Author: Ernie Chu
 * Filename: Services.cpp
 * Description: Socket services header.
 */

#pragma once
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cerrno>
#include <filesystem> /* filename */
#include <Huffman.hpp>

namespace srvc {

class Link {
public:
    Link() : err(0), fd(-1) {}
    ~Link() { if (fd >= 0) close(fd); }
    void link(const std::string IP, const int port);
    int getErr() const { return err; }
    int getFd() const { return fd; }
private:
    int err; /* 0 is normal */
    int fd;
};

class Send {
public:
    Send() : BUFSIZE(512), err(0), fsize(0), origFsize(0) {}
    void send(const int fd, const std::string filepath, const int encoding);
    int getErr() const { return err; }
    unsigned int getFsize() const { return fsize; }
    int getOrigFsize() const { return origFsize; }
private:
    const int BUFSIZE;
    int err;
    unsigned int fsize;
    unsigned int origFsize;
};

}
