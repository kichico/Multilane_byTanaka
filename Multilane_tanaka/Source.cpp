#include "OneLaneRevised_S_NFS.h"

int main() {
	int filenumber = 1;
	OneLaneRevised_S_NFS SNFS(filenumber, 2, 1000);
	SNFS.calculate();
	return 0;
}
