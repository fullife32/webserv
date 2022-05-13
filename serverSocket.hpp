#include <string>
#include <vector>

struct serverBlock
{
	std::string server_name;
	std::vector<std::string[2]> listen;
	std::vector<std::string> index;
	// list of serverLoc;
};

struct serverLoc
{
	/* data */
};


class serverSocket {
	int sockfd;

};