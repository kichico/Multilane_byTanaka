#include "LaneChange.h"

void LaneChange::insentiveCheck() {
	laneChangeID.clear();
	for (int i = 0; i < information.DCars.size(); i++) {
		int ID = information.DCars[i];
		InsentiveData leftData = _insentiveCheckForcusLane(WhichLane::left, ID);
		InsentiveData rightData = _insentiveCheckForcusLane(WhichLane::right, ID);
		int signal;
		if (leftData.motivated && rightData.motivated) {
			if (leftData.gap > rightData.gap) signal = Car::SignalKind::Left;
			else if (leftData.gap < rightData.gap) signal = Car::SignalKind::Right;
			else {
				if (random->random(1.0) < 0.5) signal = Car::SignalKind::Left;
				else signal = Car::SignalKind::Right;
			}
		}
		else if (leftData.motivated && !rightData.motivated) signal = Car::SignalKind::Left;
		else if (!leftData.motivated && rightData.motivated) signal = Car::SignalKind::Right;
		else signal = Car::SignalKind::Non;
		car.signal[ID] = signal;
		if (signal != Car::SignalKind::Non) laneChangeID.emplace_back(ID);
	}
}

bool LaneChange::laneChange() {
	if (laneChangeID.size() > 0) {
		//先行車両から順に車線変更する(laneChangeIDを並べ替える)
		_dicideUpdateOrder();
		//以下で車線変更
		int ID, nextLaneNum, currentLaneNum, position;
		ArroundData arroundData;
		for (int i = 0; i < laneChangeID.size(); i++) {
			ID = laneChangeID[i];
			currentLaneNum = car.laneNumber.current[ID];
			position = car.position.current[ID];
			if (car.signal[ID] == Car::SignalKind::Left) nextLaneNum = currentLaneNum - 1;
			else if(car.signal[ID] == Car::SignalKind::Right) nextLaneNum = currentLaneNum + 1;
			else {
				std::cout << "ERROR" << std::endl;
				getchar();
			}
			//車線変更先に車両がいたら，もちろん車線変更できない
			if (!map.existance[nextLaneNum][position]) {
				arroundData = _getArroundData(nextLaneNum, ID);
				if (arroundData.distance.rear >= car.velocity.current[arroundData.ID.rear] - car.velocity.current[ID]) {
					//安全条件クリア
					//車線変更
					//元の車線の情報を更新
					information.eachLaneCarNum[currentLaneNum]--;
					maximumSumV[currentLaneNum] -= car.canditateVelocity[ID];
					map.existance[currentLaneNum][position] = false;
					if (information.eachLaneCarNum[currentLaneNum] > 0) {
						int cFront = car.arround[ID].front;
						int cRear = car.arround[ID].rear;
						int cDistance = car.position.current[cFront] - car.position.current[cRear] - 1;
						if (cDistance < 0) cDistance += L;
						car.arround[cFront].rear = cRear;
						car.arround[cRear].front = cFront;
						car.distance.current[cRear] = cDistance;
						if (ID == information.leadCar[currentLaneNum].ID) information.leadCar[currentLaneNum].ID = cFront;
					}
					//自身の車線変更
					car.laneNumber.current[ID] = nextLaneNum;
					map.existance[nextLaneNum][position] = true;
					map.CarID[nextLaneNum][position] = ID;
					information.eachLaneCarNum[nextLaneNum]++;
					maximumSumV[nextLaneNum] += car.canditateVelocity[ID];
					//車線変更先の情報を更新
					if (information.eachLaneCarNum[nextLaneNum] == 1) {
						car.arround[ID].front = car.arround[ID].rear = ID;
						car.distance.current[ID] = L - 1;
						information.leadCar[nextLaneNum].existence = true;
						information.leadCar[nextLaneNum].ID = ID;
					}
					else {
						car.arround[ID].front = arroundData.ID.front;
						car.arround[arroundData.ID.front].rear = ID;
						car.distance.current[ID] = arroundData.distance.front;
						car.arround[ID].rear = arroundData.ID.rear;
						car.arround[arroundData.ID.rear].front = ID;
						car.distance.current[arroundData.ID.rear] = arroundData.distance.rear;
					}
				}
			}
		}
		//先頭車両を再探索する．
		int maxDistance, leadID;
		for (int i = 0; i < information.eachLaneCarNum.size(); i++) {
			if (information.eachLaneCarNum[i] == 0) information.leadCar[i].existence = false;
			else {
				leadID = information.leadCar[i].ID;
				maxDistance = car.distance.current[leadID];
				ID = car.arround[leadID].rear;
				while (ID != leadID) {
					if (maxDistance > car.distance.current[ID]) {
						maxDistance = car.distance.current[ID];
						information.leadCar[i].ID = ID;
					}
					ID = car.arround[ID].rear;
				}
			}
		}
		return true;
	}
	else return false;
}

LaneChange::ArroundData LaneChange::_getArroundData(int laneNum, int ID) {
	//真隣はfrontとみなす．
	ArroundData arroundData;
	if (information.eachLaneCarNum[laneNum] > 0) {
		int position = car.position.current[ID];
		if (map.existance[laneNum][position]) {
			arroundData.ID.front = map.CarID[laneNum][position];
			arroundData.distance.front = 0;
			arroundData.ID.rear = car.arround[arroundData.ID.front].rear;
			arroundData.distance.rear = car.arround[arroundData.ID.front].rear;
		}
		else {
			int fPosition = position + 1;
			int rPosition = position - 1;
			while (true) {
				if (fPosition == L) fPosition = 0;
				if (rPosition == -1) rPosition = L - 1;
				if (map.existance[laneNum][fPosition] || map.existance[laneNum][rPosition]) {
					int front, rear;
					if (map.existance[laneNum][fPosition] && map.existance[laneNum][rPosition]) {
						front = map.CarID[laneNum][fPosition];
						rear = map.CarID[laneNum][rPosition];
					}
					else if (map.existance[laneNum][fPosition] && !map.existance[laneNum][rPosition]) {
						front = map.CarID[laneNum][fPosition];
						rear = car.arround[front].rear;
						rPosition = car.position.current[rear];
					}
					else {
						rear = map.CarID[laneNum][rPosition];
						front = car.arround[rear].front;
						fPosition = car.position.current[front];
					}
					int fDistance = fPosition - position - 1;
					int rDistance = position - rPosition - 1;
					if (fDistance < 0) fDistance += L;
					if (rDistance < 0) rDistance += L;
					arroundData.ID.front = front;
					arroundData.ID.rear = rear;
					arroundData.distance.front = fDistance;
					arroundData.distance.rear = rDistance;
					break;
				}
				fPosition++;
				rPosition--;
			}
		}
	}
	else {
		arroundData.ID.front = arroundData.ID.rear = -1;
		arroundData.distance.front = arroundData.distance.rear = L;
	}
	return arroundData;
}

LaneChange::InsentiveData LaneChange::_insentiveCheckForcusLane(int Lane, int ID) {
	InsentiveData insentiveData;
	int laneNum = car.laneNumber.current[ID];
	if (Lane == WhichLane::left) laneNum--;
	else laneNum++;
	if (0 <= laneNum && laneNum < LaneNum) {
		if (information.eachLaneCarNum[laneNum] == 0) {
			insentiveData.motivated = true;
			insentiveData.gap = L;
		}
		else {
			ArroundData arroundData = _getArroundData(laneNum, ID);
			int front = car.arround[ID].front;
			int v = car.velocity.current[ID];
			if ((car.distance.current[ID] <= v - car.velocity.current[front])
				&& (arroundData.distance.front > v - car.velocity.current[arroundData.ID.front])
				&& (arroundData.distance.front > 0)) {
				insentiveData.motivated = true;
				insentiveData.gap = arroundData.distance.front;
			}
			else {
				insentiveData.motivated = false;
				insentiveData.gap = 0;
			}
		}
	}
	else {
		insentiveData.motivated = false;
		insentiveData.gap = 0;
	}
	return insentiveData;
}

void LaneChange::_dicideUpdateOrder() {
	//まずは車線変更を開始するポジションを各車線の先頭車両の中から等確率で選出する
	std::vector<int> canditates;
	for (int i = 0; i < information.leadCar.size(); i++) if (information.leadCar[i].existence) canditates.emplace_back(car.position.current[information.leadCar[i].ID]);
	int size = canditates.size() - 1;
	int xc = random->random(size);
	int position = canditates[xc];
	//車線変更する順番を決定する
	//上記で決定したポジションが最大値(=L-1)になるようにポジションとIDを格納
	//ポジションについて降順で並べ替えて，同じポジションのものはランダムで決定する．
	std::vector<int> dummyPosition;
	std::vector<int> dummyID;
	int ID;
	int correctionValue = L - position - 1;
	for (int i = 0; i < laneChangeID.size(); i++) {
		ID = laneChangeID[i];
		int p = car.position.current[ID];
		if (p <= position) p += correctionValue;
		else p -= (position + 1);
		dummyPosition.emplace_back(p);
		dummyID.emplace_back(ID);
	}
	Sort sort;
	sort.descending_sort(dummyPosition);
	int lastP = dummyPosition[0];
	ID = dummyID[sort.number[0]];
	std::vector<int> samePositionID(1, ID);
	int n = 1;
	laneChangeID.clear();
	while (n < dummyPosition.size()) {
		ID = dummyID[sort.number[n]];
		if (lastP == dummyPosition[n]) samePositionID.emplace_back(ID);
		else {
			_laneChangeID_emplace_back(samePositionID);
			lastP = dummyID[n];
			samePositionID = std::vector<int>(1, ID);
		}
		n++;
	}
	_laneChangeID_emplace_back(samePositionID);
}

void LaneChange::_laneChangeID_emplace_back(std::vector<int>& samePositionID) {
	int size, x, ID;
	while (0 < samePositionID.size()) {
		if (samePositionID.size() == 1) x = 0;
		else {
			size = samePositionID.size() - 1;
			x = random->random(size);
		}
		ID = samePositionID[x];
		std::iter_swap(samePositionID.begin() + x, samePositionID.end() - 1);
		samePositionID.pop_back();
		laneChangeID.emplace_back(ID);
	}
}
