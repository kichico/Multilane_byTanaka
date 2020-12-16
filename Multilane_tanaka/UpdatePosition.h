#ifndef UPDATEPOSITION_H
#define UPDATEPOSITION_H
#include "LaneChange.h"

class UpdatePosition : public LaneChange {
public:
	struct NextInfomrations {
		std::vector<Car::Mesurement> mesure;
		std::vector<Information::Statics> eachLaneResults;
		Information::Statics allResults;
	};
	UpdatePosition(int LaneNum, int L, int N, int CNum, int filenumber);
	~UpdatePosition();
	NextInfomrations updatePosition(bool DoMesure);
private:
	bool DoMesure;
	int nextLeadCar;
	int maximuDistance;
	int sumVelocity;
	std::vector<bool> existance;
	std::vector<int> CarID;
	std::vector<Car::Mesurement> next_mesurement;
	Information::Statics nextForcusLaneResults;
	Information::Statics nextAllResults;
	int _updatePosition(int leadCar);
	void _moveForward(int ID);
};

#endif // !UPDATEPOSITION_H

