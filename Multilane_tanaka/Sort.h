#ifndef SORT_H
#define SORT_H
#include <vector>

template<typename _T>
_T _MAX(_T x, _T y);

template<typename _T>
_T _MIN(_T x, _T y);

class Sort {
public:

	std::vector<int> number;

	void ascending_sort(std::vector<int>& A);
	void ascending_sort(std::vector<unsigned int>& A);
	void ascending_sort(std::vector<long>& A);
	void ascending_sort(std::vector<unsigned long>& A);
	void ascending_sort(std::vector<long long>& A);
	void ascending_sort(std::vector<unsigned long long>& A);
	void ascending_sort(std::vector<float>& A);
	void ascending_sort(std::vector<double>& A);
	void ascending_sort(std::vector<char>& A);

	void descending_sort(std::vector<int>& A);
	void descending_sort(std::vector<unsigned int>& A);
	void descending_sort(std::vector<long>& A);
	void descending_sort(std::vector<unsigned long>& A);
	void descending_sort(std::vector<long long>& A);
	void descending_sort(std::vector<unsigned long long>& A);
	void descending_sort(std::vector<float>& A);
	void descending_sort(std::vector<double>& A);
	void descending_sort(std::vector<char>& A);

private:

	template<typename _T>
	void _ascending_sort(std::vector<_T>& A);

	template<typename _T>
	void _descending_sort(std::vector<_T>& A);

	template<typename _T>
	_T _med3(_T x, _T y, _T z);

	void _Create_number(int size);

	template<typename _T>
	void _ascendingsort(std::vector<_T>* A, int left, int right);

	template<typename _T>
	void _descendingsort(std::vector<_T>* A, int left, int right);
};

#endif // !SORT_H
