#include "UpdatePosition.h"

UpdatePosition::UpdatePosition(int LaneNum, int L, int N, int CNum, int filenumber) {
	this->LaneNum = LaneNum;
	this->CNum = CNum;
	this->N = N;
	this->L = L;
	random = new Random();
}

UpdatePosition::~UpdatePosition() {
	delete random;
}

UpdatePosition::NextInfomrations UpdatePosition::updatePosition(bool DoMesure) {
	NextInfomrations nextInfo;
	nextInfo.mesure = car.mesure;
	nextInfo.eachLaneResults = information.eachLaneResults;
	nextInfo.allResults = information.allResults;
	this->DoMesure = DoMesure;
	sumVelocity = 0;
	car.velocity.current = std::vector<int>(N, 0);
	for (int i = 0; i < information.leadCar.size(); i++) {
		if (information.leadCar[i].existence) {
			int leadCar = information.leadCar[i].ID;
			int front = car.arround[leadCar].front;
			bool fg = true;
			int sumV = maximumSumV[i];
			while (fg) {
				existance = std::vector<bool>(L, false);
				CarID = std::vector<int>(L);
				if (DoMesure) {
					next_mesurement = nextInfo.mesure;
					nextForcusLaneResults = nextInfo.eachLaneResults[i];
					nextAllResults = nextInfo.allResults;
				}
				existance[car.position.current[front]] = true;
				CarID[car.position.current[front]] = front;
				sumV = _updatePosition(leadCar);
				if (sumVelocity == sumV) fg = false;
				else sumVelocity = sumV;
			}
			map.existance[i] = existance;
			map.CarID[i] = CarID;
			information.leadCar[i].ID = nextLeadCar;
			if (DoMesure) {
				nextInfo.mesure = next_mesurement;
				nextInfo.eachLaneResults[i] = nextForcusLaneResults;
				nextInfo.allResults = nextAllResults;
				for (int i = 0; i < N; ++i) {
					if (car.strategy[i] == Car::StrategyKind::C) information.ave_C += car.velocity.current[i];
					else if (car.strategy[i] == Car::StrategyKind::D) information.ave_D += car.velocity.current[i];
					//std::cout << i << "th car velocity is " << car.velocity.current[i] << std::endl;
				}
			}
		}
	}
	return nextInfo;
}

int UpdatePosition::_updatePosition(int leadCar) {
	int sumV = 0;
	int ID = leadCar;
	maximuDistance = -1;
	while (true) {
		_moveForward(ID);
		if (DoMesure) {
			//以下，計測パート
			if (car.mesure[ID].merured) {
				if (car.position.previous[ID] < 28 && 28 <= car.position.current[ID]) {
					next_mesurement[ID].merured = false;
					double time = next_mesurement[ID].elapsed + 1.0 * (28 - car.position.previous[ID]) / car.velocity.current[ID];
					double AV = 18 * constants.per_cell / time;
					next_mesurement[ID].elapsed = 0;
					nextForcusLaneResults.CountNum++;
					nextForcusLaneResults.AV += AV;
					nextAllResults.CountNum++;
					nextAllResults.AV += AV;
				}
				else {
					next_mesurement[ID].elapsed += 1;
				}
			}
			else {
				if (car.position.previous[ID] < 10 && 10 <= car.position.current[ID]) {
					next_mesurement[ID].merured = true;
					next_mesurement[ID].elapsed = 1.0 * (10 - car.position.previous[ID]) / car.velocity.current[ID];
				}
			}
		}
		sumV += car.velocity.current[ID];
		ID = car.arround[ID].rear;
		if (ID == leadCar) break;
	}
	return sumV;
}

void UpdatePosition::_moveForward(int ID) {
	if (car.velocity.current[ID] < car.canditateVelocity[ID]) {
		int front = car.arround[ID].front;
		int nextPosition = car.position.previous[ID];
		int v = 0;
		for (int i = 0; i < car.canditateVelocity[ID]; i++) {
			nextPosition++;
			if (nextPosition == L) nextPosition -= L;
			if (existance[nextPosition]) {
				if (CarID[nextPosition] == front) {
					nextPosition--;
					if (nextPosition < 0) nextPosition += L;
					break;
				}
			}
			v++;
		}
		car.velocity.current[ID] = v;
		car.position.current[ID] = nextPosition;
		existance[nextPosition] = true;
		CarID[nextPosition] = ID;
		int nextDistance;
		if (car.position.current[front] <= nextPosition) {
			nextDistance = car.position.current[front] - nextPosition + L - 1;
		}
		else {
			nextDistance = car.position.current[front] - nextPosition - 1;
		}
		car.distance.current[ID] = nextDistance;
		if (nextDistance > maximuDistance) {
			maximuDistance = nextDistance;
			nextLeadCar = ID;
		}
	}
	else {
		int position = car.position.current[ID];
		existance[position] = true;
		CarID[position] = ID;
	}
}
