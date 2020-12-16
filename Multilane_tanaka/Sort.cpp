#include "Sort.h"

template<typename _T>
_T _MAX(_T x, _T y) { return x < y ? y : x; }

template<typename _T>
_T _MIN(_T x, _T y) { return x > y ? y : x; }

void Sort::ascending_sort(std::vector<int>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<unsigned int>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<long>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<unsigned long>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<long long>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<unsigned long long>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<float>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<double>& A) { _ascending_sort(A); }
void Sort::ascending_sort(std::vector<char>& A) { _ascending_sort(A); }

void Sort::descending_sort(std::vector<int>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<unsigned int>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<long>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<unsigned long>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<long long>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<unsigned long long>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<float>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<double>& A) { _descending_sort(A); }
void Sort::descending_sort(std::vector<char>& A) { _descending_sort(A); }

template<typename _T>
void Sort::_ascending_sort(std::vector<_T>& A) {
	int left = 0;
	int right = A.size() - 1;
	_Create_number(A.size());
	_ascendingsort(&A, left, right);
}

template<typename _T>
void Sort::_descending_sort(std::vector<_T>& A) {
	int left = 0;
	int right = A.size() - 1;
	_Create_number(A.size());
	_descendingsort(&A, left, right);
}

template<typename _T>
_T Sort::_med3(_T x, _T y, _T z) { return _MIN(_MAX(x, y), z); }

void Sort::_Create_number(int size) {
	number = std::vector<int>(size);
	for (int i = 0; i < size; i++) number[i] = i;
}

template<typename _T>
void Sort::_ascendingsort(std::vector<_T>* A, int left, int right) {
	if (left < right) {
		_T pivot = _med3((*A)[left], (*A)[left + (right - left) / 2], (*A)[right]);
		int l = left;
		int r = right;
		while (true) {
			while ((*A)[l] < pivot) l++;
			while ((*A)[r] > pivot) r--;
			if (l >= r) break;
			std::swap((*A)[l], (*A)[r]);
			std::swap(number[l], number[r]);
			l++;
			r--;
		}
		_ascendingsort(A, left, l - 1);
		_ascendingsort(A, r + 1, right);
	}
}

template<typename _T>
void Sort::_descendingsort(std::vector<_T>* A, int left, int right) {
	if (left < right) {
		_T pivot = _med3((*A)[left], (*A)[left + (right - left) / 2], (*A)[right]);
		int l = left;
		int r = right;
		while (true) {
			while ((*A)[l] > pivot) l++;
			while ((*A)[r] < pivot) r--;
			if (l >= r) break;
			std::swap((*A)[l], (*A)[r]);
			std::swap(number[l], number[r]);
			l++;
			r--;
		}
		_descendingsort(A, left, l - 1);
		_descendingsort(A, r + 1, right);
	}
}
