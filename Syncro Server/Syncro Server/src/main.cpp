#include "SyncroServer.h"

int main(int argc, char** argv) {
	syncro::CSyncroServer oServer;
	bool fOK = oServer.Run();
	return fOK ? 0 : 1;
}
