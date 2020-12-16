#include "Information.h"

void Information::initilaize(int LaneNum) {
	allResults.AV = 0;
	allResults.CountNum = 0;
	eachLaneResults = std::vector<Statics>(LaneNum, allResults);
	LeadCarInformations LCI;
	LCI.existence = false;
	LCI.ID = -1;
	leadCar = std::vector<LeadCarInformations>(LaneNum, LCI);
	eachLaneCarNum = std::vector<int>(LaneNum, 0);
}