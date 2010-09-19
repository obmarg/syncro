#include "SyncroServer.h"

int main(char **argc, int argv) {
	CSyncroServer oServer;
	bool fOK = oServer.Run();
	return fOK ? 0 : 1;
}