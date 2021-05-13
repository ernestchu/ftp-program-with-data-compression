/*
 * Author: Ernie Chu
 * Filename: Services.cpp
 */

#include <Services.hpp>

Services::Services(
    const int back, 
    const int buf
) :
    BACKLOG(back),
    BUFSIZE(buf),
    fd(-1)
{}

void Services::printHostAddrs(int port) {
    ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        perror("getifaddrs");
        exit(-1);
    }
    ifaddrs *address = addresses;
    while(address) { /* all network interface */
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET) {
            char ap[100];
            const int family_size = sizeof(sockaddr_in);
            getnameinfo(address->ifa_addr,family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            std::cout << "\t" << ap << ":" << port << std::endl;
        }
        address = address->ifa_next;
    }
    freeifaddrs(addresses);
}

void Services::listen(const int port) {
     /* socket procedure */
    fd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* bind procedure */
    sockaddr_in srv; /* used by bind() */
    srv.sin_family = AF_INET; /* IPv4 */
    srv.sin_port = htons(port);
    srv.sin_addr.s_addr = htonl(INADDR_ANY); /* any of server addresses */
    if (::bind(fd, (sockaddr*) &srv, sizeof(srv)) < 0) {
        perror("bind");
        exit(1);
    }

    /* listen procedure */
    if (::listen(fd, BACKLOG) < 0) {
        perror("listen");
        exit(1);
    }
}

Services::~Services() {
    close(fd);
}

int Services::accept() {
    /* accept procedure */
    sockaddr_in cli;
    int clifd;
    socklen_t cli_len = sizeof(cli);
    
    /* TCP established individual connection with each client */
    clifd = ::accept(fd, (sockaddr*) &cli, &cli_len);
    if (clifd < 0) {
        perror("accept");
        exit(1);
    }

    auto clock = time(nullptr);
    std::cout << "\nConnection established at " << ctime(&clock);

    std::cout
        << "Client's IP address: "
        << inet_ntoa(cli.sin_addr) << ":"
        << cli.sin_port
        << "\n│\n";
    std::cout.flush();

    return clifd;
}
      
bool Services::recv(const int clifd) {
    auto recvFile = [clifd, this] (const std::string fname) -> unsigned int {
        int n;
        unsigned int nTotal = 0;
        std::ofstream ofs(fname, std::ofstream::binary);
        if (!ofs) {
            perror(fname.c_str());
            exit(1);
        }
        /* read file size */
        unsigned int fsize;
        if (read(clifd, (void*)&fsize,sizeof(fsize)) < 0) {
            perror("read file size");
            exit(1);
        }

        /* read file content */
        while (nTotal < fsize) {
            char buf[BUFSIZE];
            size_t len = sizeof(buf);
            if ((fsize-nTotal) < len)
                len = fsize-nTotal;
            if ((n = read(clifd, (void*)buf, len)) < 0) {
                perror("read data");
                exit(1);
            }
            nTotal += n;
            ofs.write(buf, len);
        }
        ofs.close();
        auto clock = time(nullptr);
        std::cout << "├── Received `" << fname << "` at " << ctime(&clock);
        std::cout << "│   Total " << nTotal << " bytes transmitted.\n│\n";
        return nTotal;
    };

    int n;
    /* read file name */
    int fnameSize;
    if ((n = read(clifd, (void*)&fnameSize, sizeof(fnameSize))) < 0) {
        perror("read filename size");
        exit(1);
    } else if (n == 0) {
        std::cout << "└── Connection terminated.\n";
        return false;
    }
    const char* fnameTemp = new char[fnameSize];
    if (read(clifd, (void*)fnameTemp, fnameSize) < 0) {
        perror("read filename");
        exit(1);
    }

    std::string fname = "./Data/";
    fname += fnameTemp;
    delete[] fnameTemp;

    /* read encoding algorithm */
    int encoding;
    if (read(clifd, (void*)&encoding, sizeof(encoding)) < 0) {
        perror("encoding");
        exit(1);
    }
    if (encoding) {
        recvFile("./HuffmanCache/encHead");
        unsigned int compSize = recvFile("./HuffmanCache/encData");

        std::ifstream encifs("./HuffmanCache/encHead");
        int pad; encifs >> pad;
        std::unordered_map<char, std::string> codewords;
        while (encifs) {
            char ch;
            std::string code;
            encifs.ignore(5, '\n');
            encifs.get(ch);
            if (encifs >> code)
                codewords[ch] = code;
        }
        encifs.close();

        std::unique_ptr<huf::Huffman> decoder;
        if (encoding == 1) /* polymorphism */
            decoder = std::unique_ptr<huf::HuffmanFix>(new huf::HuffmanFix(codewords));
        else
            decoder = std::unique_ptr<huf::HuffmanVar>(new huf::HuffmanVar(codewords));

        std::string decoded = decoder->decode("./HuffmanCache/encData", pad);
        unsigned int origSize = decoded.size();
        std::ofstream ofs(fname, std::ofstream::binary);
        ofs.write(decoded.c_str(), decoded.size());
        ofs.close();
        auto clock = time(nullptr);
        std::cout << "├── Decompressed `" << fname << "` at " << ctime(&clock);
        std::cout << "│   Original file size: " << origSize << " bytes.\n";
        std::cout << "│   Compressed file size: " << compSize << " bytes.\n";
        std::cout << "│   Compression rate: " << std::fixed << std::setprecision(2) << (double)compSize/(double)origSize*100.0 << "%\n│\n";
    } else
        recvFile(fname);

    return true;
}
