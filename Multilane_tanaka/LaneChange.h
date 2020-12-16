#ifndef LANECHANGE_H
#define LANECHANGE_H
#include "DecideVelocityCanditate.h"
#include "Sort.h"

class LaneChange : public DecideVelocityCanditate {
public:
	void insentiveCheck();
	bool laneChange();
private:
	enum WitchLane {
		left, right
	};
	struct ArroundData {
		Car::FrontRearCar ID;
		Car::FrontRearCar distance;
	};
	struct InsentiveData {
		bool motivated;
		int gap;
	};
	std::vector<int> laneChangeID;

	ArroundData _getArroundData(int laneNum, int ID);
	InsentiveData _insentiveCheckForcusLane(WitchLane witchLane, int ID);
	void _dicideUpdateOrder();
	void _laneChangeID_emplace_back(std::vector<int>& samePositionID);
};

#endif // !LANECHANGE_H

