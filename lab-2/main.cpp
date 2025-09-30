#include "Server.h"

int main() {
	Server server(2001);

	server.start();
	server.run();

	return 0;
}