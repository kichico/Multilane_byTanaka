#ifndef ONELANEREVISED_S_NFS_H
#define ONELANEREVISED_S_NFS_H
#include <string>
#include <sstream>
#include "UpdatePosition.h"

class OneLaneRevised_S_NFS {
public:
	OneLaneRevised_S_NFS(int filenumber, int LaneNum, int L);
	void calculate();
private:
	int filenumber;
	int LaneNum;
	int L;
	//double ave_C=0;
	//double ave_D=0;
};

#endif // !ONELANEREVISED_SNFS_H

