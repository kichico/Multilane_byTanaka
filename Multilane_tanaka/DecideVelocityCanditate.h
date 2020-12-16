#ifndef DECIDEVELOCITYCANDITATE_H
#define DECIDEVELOCITYCANDITATE_H
#include "InisializeSetting.h"

class DecideVelocityCanditate : public InisializeSetting {
public:
	std::vector<int> maximumSumV;
	void decideVelocity();
private:
	void _rule1(int ID);
	void _rule2(int ID);
	void _rule3(int ID);
	void _rule4(int ID);
};

#endif // !DECIDEVELOCITYCANDITATE_H

