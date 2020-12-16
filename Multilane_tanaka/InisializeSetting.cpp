#include "InisializeSetting.h"

void InisializeSetting::initialize() {
	car.initialize(N);
	map.initialize(LaneNum, L);
	information.initilaize(LaneNum);
	_setConstantValues();
	_setStrategy();
	_setInitializePosition();
}

void InisializeSetting::_setConstantValues() {
	constants.per_cell = 5.6;
	constants.Vmax = 5;
	constants.G = 15;
	constants.S = 2;
	constants.r = 0.99;
	constants.q = 0.99;
	constants.p1 = 0.99;
	constants.p2 = 0.99;
	constants.p3 = 0.98;
	constants.p4 = 0.01;
}

void InisializeSetting::_setStrategy() {
	std::vector<int> DCars(N);
	for (int i = 0; i < N; i++) DCars[i] = i;
	for (int i = 0; i < CNum; i++) {
		int rem_size = DCars.size() - 1;
		int xrem = random->random(rem_size);
		int ID = DCars[xrem];
		std::iter_swap(DCars.begin() + xrem, DCars.end() - 1);
		DCars.pop_back();
		car.strategy[ID] = Car::StrategyKind::C;
	}
	for (int i = 0; i < DCars.size(); i++) car.strategy[DCars[i]] = Car::StrategyKind::D;
	information.DCars = DCars;
}

void InisializeSetting::_setInitializePosition() {
	std::vector<int> rem(LaneNum * L);
	for (int i = 0; i < rem.size(); i++) rem[i] = i;
	for (int i = 0; i < N; i++) {
		int rem_size = rem.size() - 1;
		int xrem = random->random(rem_size);
		int p = rem[xrem];
		int laneNum = p / L;
		int position = p % L;
		std::iter_swap(rem.begin() + xrem, rem.end() - 1);
		rem.pop_back();
		map.existance[laneNum][position] = true;
		map.CarID[laneNum][position] = i;
		car.laneNumber.current[i] = car.laneNumber.previous[i] = laneNum;
		car.position.current[i] = car.position.previous[i] = position;
		information.eachLaneCarNum[laneNum]++;
		if (!information.leadCar[laneNum].existence) {
			information.leadCar[laneNum].existence = true;
			information.leadCar[laneNum].ID = i;
		}
	}
	for (int i = 0; i < information.leadCar.size(); i++) {
		if (information.leadCar[i].existence) {
			int ID = information.leadCar[i].ID;
			int startPosition = car.position.current[ID];
			int position = startPosition - 1;
			int maxDistance = 0;
			int frontID = ID;
			int distance = 0;
			while (true) {
				if (position < 0) position += L;
				if (map.existance[i][position]) {
					int rearID = map.CarID[i][position];
					car.arround[ID].front = frontID;
					car.arround[ID].rear = rearID;
					car.distance.current[ID] = car.distance.previous[ID] = distance;
					car.arround[rearID].front = ID;
					if (distance > maxDistance) {
						maxDistance = distance;
						information.leadCar[i].ID = ID;
					}
					frontID = ID;
					ID = rearID;
					distance = 0;
					if (position == startPosition) break;
				}
				else distance++;
				position--;
			}
		}
	}
}