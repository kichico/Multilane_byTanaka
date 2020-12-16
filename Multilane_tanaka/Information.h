#ifndef INFORMATION_H
#define INFORMATION_H
#include <vector>

class Information {
public:
	struct Statics {
		int CountNum;
		double AV;
	};
	struct LeadCarInformations {
		int ID;
		bool existence;
	};
	std::vector<int> eachLaneCarNum;
	std::vector<LeadCarInformations> leadCar;
	std::vector<Statics> eachLaneResults;
	Statics allResults;
	std::vector<int> DCars;
	void initilaize(int LaneNum);
};

#endif // !INFORMATION_H

