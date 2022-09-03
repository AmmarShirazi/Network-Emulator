#include <iostream>
#include <cmath>
#include <limits>

template <typename T>
void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

template <class T>
class MaxHeap {
	T* heapArray;
	int currSize;
	int capacity;
public:

	

	MaxHeap() {
		heapArray = new T[200];
		currSize = 0;
		capacity = 200;
	}

	T* getArray() {
		return this->heapArray;
	}
	
	void insert(T& data) {

		if (currSize == 0) {
			heapArray[currSize++] = data;
			return;
		}
		heapArray[currSize++] = data;
		reShift(currSize - 1);
	}
	int getParent(int index) {
		return (index - 1) / 2;
	}
	int getLeftChild(int index) {
		return 2 * index;
	}
	int getRightChild(int index) {
		return (2 * index) + 1;
	}
	void reShift(int index) {

		int parent = getParent(index);


		while (parent >= 0 && heapArray[index] > heapArray[parent]) {

			swap<T>(heapArray[index], heapArray[parent]);

			index = parent;
			parent = getParent(index);
		}

	}
	void heapify(int index) {
		
		int left = getLeftChild(index);
		int right = getRightChild(index);
		int max = index;

		if (left < currSize && heapArray[left] > heapArray[index]) {
			max = left;
		}

		if (right < currSize && heapArray[right] > heapArray[max]) {
			max = right;
		}
		if (max != index) {
			swap(heapArray[index], heapArray[max]);
			heapify(max);
		}
	}
	T& getMax() {
		return heapArray[0];
	}
	T& popMax() {
		if (currSize == 0) {
			T* temp = new T;
			*temp = {};
			return *temp;
		}
		if (currSize == 1) {
			currSize--;
			return heapArray[0];
		}
		T* toReturn = &heapArray[0];
		heapArray[0] = heapArray[--currSize];
		heapify(0);

		return *toReturn;
	}
	void test() { // remove this code later
		for (int i = 0; i < currSize / 2; i++) {
			std::cout << "Parent Node: " << heapArray[i];
			if (getLeftChild(i) < currSize) {
				std::cout << " Left Child: " << heapArray[getLeftChild(i)];
			}
			if (getRightChild(i) < currSize) {
				std::cout << " Right Child: " << heapArray[getRightChild(i)];
			}
			std::cout << std::endl;
		}
	}
	bool isEmpty() {
		return currSize == 0;
	}
	~MaxHeap() {
		delete[] heapArray;
	}
};