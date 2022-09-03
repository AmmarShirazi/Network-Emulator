#pragma once
#include <string>
#include <sstream>
#include "LinkedList.h"
#include "Message.h"

template <typename T>
void printArray(T* arr, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << arr[i] << std::endl;
    }
}

int extractInteger(std::string str) { // only removes first index and returns whatever is left as integer, dont trust this function
	return std::stoi(str.substr(1, str.length()));
}

int getRouterIndex(std::string str) {
    return extractInteger(str) - 1;
}

bool isStringContain(std::string mainString, std::string toCheck) {
    if (mainString.length() < toCheck.length()) {
        return false;
    }
    return strstr(mainString.c_str(), toCheck.c_str());
}

bool isNumber(char x) {
    return x > 47 && x < 58;
}

void initializeBoolArray(bool* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = false;
    }
}

void initializeIntArray(int* arr, int size, int val) {
    for (int i = 0; i < size; i++) {
        arr[i] = val;
    }
}


int minDistance(int* dArr, int size, LinkedList<int>* checkList) {

    int min = INT_MAX;
    int index = 0;
    for (int i = 0; i < size; i++) {
        if (!checkList->isContain(i) && dArr[i] <= min) {
            min = dArr[i];
            index = i;
        }
    }
    checkList->insert(index);
    return index;
}

void getPath(int* arr, std::string& list, int i) {

    if (arr[i] == INT_MAX) {
        return;
    }
    
    getPath(arr, list, arr[i]);
    list += std::to_string(i) + " ";
}

int* getDijkstraList(int** graph, int source, int size, std::string*& list) {

    int *distanceList = new int[size];
    int* path = new int[size];
    initializeIntArray(distanceList, size, INT_MAX);
    initializeIntArray(path, size, INT_MAX);
    distanceList[source] = 0;

    list = new std::string[size];
    
    LinkedList<int>* checkList = new LinkedList<int>;

    for (int i = 0; i < size - 1; i++) {
        
        // thori besharmi ho skti hai idr
        int closestIndex = minDistance(distanceList, size, checkList);
        


        for (int j = 0; j < size; j++) {
            if (!checkList->isContain(j)) {

                if (graph[closestIndex][j] != 0 && distanceList[closestIndex] != INT_MAX) {
                    if (distanceList[closestIndex] + graph[closestIndex][j] < distanceList[j]) {
                        distanceList[j] = distanceList[closestIndex] + graph[closestIndex][j];
                        path[j] = closestIndex;
                    }
                }
            }
        }
         
    }
    for (int i = 0; i < size; i++) {
        getPath(path, list[i], i);
    }
    return distanceList;

}

std::string returnMachineId(int num, int mCount) {
    num++;
    std::string temp;
    if (num <= mCount) {
        temp = "M";
    }
    else {
        temp = "R";
        num -= mCount;
    }

    temp += std::to_string(num);

    return temp;
}

std::string makePath(std::string path, int start, int mCount) {

    LinkedList<std::string> tokens;

    std::stringstream pathstream(path);
    std::string temp;

    while (std::getline(pathstream, temp, ' ')) {
        tokens.insert(temp);
    }

    std::string toReturn;

    toReturn += returnMachineId(start, mCount) + " ";

    for (int i = 0; i < tokens.getListSize(); i++) {

        toReturn += returnMachineId(std::stoi(tokens.getAtIndex(i)), mCount) + " ";
    }

    return toReturn.substr(0, toReturn.length() - 1);
}

std::string* makePathList(std::string* list, int size, int start, int mCount) {

    for (int i = 0; i < size; i++) {
        list[i] = makePath(list[i], start, mCount);
    }
    
    return list;
}

std::string getTokenAtIndex(std::string path, int index) {

    LinkedList<std::string> tokens;
    std::stringstream pathstream(path);
    std::string temp;

    while (std::getline(pathstream, temp, ' ')) {
        tokens.insert(temp);
    }

    if (tokens.getListSize() > index) {
        return tokens.getAtIndex(index);
    }

    return "";
}


int getGraphIndex(std::string name, int machinesCount) {


    if (name[0] == 'R') {
        return (extractInteger(name) - 1) + machinesCount;
    }
    else if (name[0] == 'M') {
        return extractInteger(name) - 1;
    }

    return -1;
}
