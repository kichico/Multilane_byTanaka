#include "String.h"

std::string to_string(int I) {
	std::stringstream ss;
	ss << I;
	return ss.str();
}

std::string to_string(double D) {
	std::stringstream ss;
	ss << D;
	return ss.str();
}

std::string int_to_string(int I, int decimal) {
	std::stringstream ss;
	ss << std::fixed;
	ss << std::setprecision(decimal) << double(I);
	return ss.str();
}

std::string double_to_string(double D, int decimal) {
	std::stringstream ss;
	ss << std::fixed;
	ss << std::setprecision(decimal) << D;
	return ss.str();
}

int stoi(std::string str) {
	std::istringstream ss(str);
	int I;
	ss >> I;
	return I;
}

double stod(std::string str) {
	double D = 0;
	int decimalP = 0;
	for (int i = 0; i < str.size(); i++ ) {
		if (str[i] == '.') break;
		else decimalP++;
	}
	int digit = 0;
	for (int i = decimalP - 1; i >= 0; i--) {
		std::string S(1, str[i]);
		D += stoi(S) * std::pow(10, digit);
		digit++;
	}
	digit = -1;
	for (int i = decimalP + 1; i < str.size(); i++) {
		std::string S(1, str[i]);
		D += stoi(S) * std::pow(10, digit);
		digit--;
	}
	if (str != double_to_string(D, -digit)) {
		digit++;
		double minVal = std::pow(10, digit);
		std::string SVal = double_to_string(D, -digit);
		while (true) {
			if (str < SVal) D -= minVal;
			else D += minVal;
			SVal = double_to_string(D, -digit);
			if (str == SVal) break;
		}
	}
	return D;
}
