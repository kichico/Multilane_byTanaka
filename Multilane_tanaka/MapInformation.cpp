#include "MapInformation.h"

void MapInformation::initialize(int LaneNum, int L) {
	existance = std::vector<std::vector<bool>>(LaneNum, std::vector<bool>(L, false));
	CarID = std::vector<std::vector<int>>(LaneNum, std::vector<int>(L));
}