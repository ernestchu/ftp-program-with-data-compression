/*
 * Author: Ernie Chu
 * Filename: Services.cpp
 * Description: Socket services implementation.
 */

#include <Client/Services.hpp>

void srvc::Link::link(const std::string IP, const int port) {
    if (fd >= 0)
        close(fd);
    /* socket procedure */
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* connect procedure */
    struct sockaddr_in srv;
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = inet_addr(IP.c_str());

    err = connect(fd, (struct sockaddr*) &srv, sizeof(srv));
    if (err < 0) err = errno;
}


void srvc::Send::send(const int fd, const std::string filepath, const int encoding) {
    auto sendFile = [fd, this] (const std::string filepath) {
        /* send filesize */
        std::ifstream ifs(filepath, std::ifstream::binary);
        ifs.seekg(0, ifs.end);
        fsize = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        auto dummy = write(fd, (void*)&fsize, sizeof(fsize));
        dummy++;
        /* send content */
        unsigned int nTotal = 0;
        while (nTotal < fsize) {
            char buf[BUFSIZE];
            size_t len = sizeof(buf);
            if ((fsize-nTotal) < len)
                len = fsize-nTotal;
            ifs.read(buf, len);
            err = write(fd, (void*)buf, len); /* prevent dummy bytes */
            if (err < 0) {
                err = errno;
                return;
            }
            nTotal += err;
        }
        err = 0;
        ifs.close();
    };

    /* send filename */
    std::string fname = std::filesystem::path(filepath).filename();
    int fnameSize = fname.size() + 1;
    auto dummy = write(fd, (void*)&fnameSize, sizeof(fnameSize));
    dummy = write(fd, (void*)fname.c_str(), fnameSize);
    dummy++;
    /* send encoding algorithm */
    dummy = write(fd, (void*)&encoding, sizeof(encoding));
    
    if (encoding) {
        /* original file size */
        std::ifstream ifs(filepath, std::ifstream::binary);
        ifs.seekg(0, ifs.end);
        origFsize = ifs.tellg();
        ifs.close();

        std::unique_ptr<huf::Huffman> encoder;
        if (encoding == 1) /* polymorphism */
            encoder = std::unique_ptr<huf::HuffmanFix>(new huf::HuffmanFix(filepath));
        else
            encoder = std::unique_ptr<huf::HuffmanVar>(new huf::HuffmanVar(filepath));
        /* string, unordered_map<char, string>, int */
        auto [encoded, codewords, pad] = encoder->encode(filepath);

        /* write header */
        std::ofstream encofs("encHead");
        encofs << pad << std::endl;
        for (auto const& [ch, code] : codewords) {
            encofs << ch << " " << code << "\n";
        }
        encofs.close();
        sendFile("encHead");
        std::remove("encHead");
        
        /* write encoded data */
        encofs.open("encData", std::ofstream::binary);
        encofs.write(encoded.c_str(), encoded.size());
        encofs.close();
        sendFile("encData");
        std::remove("encData");
    } else
        sendFile(filepath);
}
