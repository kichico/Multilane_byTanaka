#ifndef MAPINFORMATION_H
#define MAPINFORMATION_H
#include <vector>

class MapInformation {
public:
	std::vector<std::vector<bool>> existance;
	std::vector<std::vector<int>> CarID;
	void initialize(int LaneNum, int L);
};

#endif // !MAPINFORMATION_H

