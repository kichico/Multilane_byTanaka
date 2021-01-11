#include "OneLaneRevised_S_NFS.h"

int main(/*int argc, char* argv[]*/) {
	//if (argc == 1) return 0;
	int filenumber = 1;//std::stoi(argv[1]);
	OneLaneRevised_S_NFS SNFS(filenumber, 3, 1000);
	SNFS.calculate();
	return 0;
}
