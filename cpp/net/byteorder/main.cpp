
#include <iostream>

#ifdef _LINUX_
#include <arpa/inet.h>
#else
#include <WS2tcpip.h>
#endif

using namespace std;

uint32_t ReversebytesUint32t(uint32_t value)
{
	return (value & 0x000000FFU) << 24 | (value & 0x0000FF00U) << 8 |
		(value & 0x00FF0000U) >> 8 | (value & 0xFF000000U) >> 24;
}

int main()
{
    unsigned int localAddr{};
    inet_pton(AF_INET, "127.0.0.1", &localAddr);
    cout << "127.0.0.1's net byteOrder: " << localAddr << endl;
    auto littleEndingLocalAddr = ReversebytesUint32t(localAddr);
    cout << "little ending byteOrder: " << littleEndingLocalAddr << endl;

    char buffer[100 + 1]{};
    inet_ntop(AF_INET, &localAddr, buffer, 100);
    cout << "ip: " << buffer << endl;

    cout << ntohl(localAddr) << endl;
    cout << htonl(localAddr) << endl;
    cout << htonl(littleEndingLocalAddr) << endl;

    localAddr = htonl(localAddr);
    cout << localAddr << endl;
    localAddr = ntohl(localAddr);
    cout << localAddr << endl;

    return 0;
}