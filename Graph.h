#pragma once

#include "Globals.h"
#include <iostream>
#include <string>
#include <fstream>


struct NodeDetails {

	std::string* pathList;
	int* shortestDistanceList;
	int numOfOutConnections;

	NodeDetails() {
		pathList = nullptr;
		shortestDistanceList = nullptr;
		numOfOutConnections = 0;
	}
};

class Graph {

	int** adjMatrix;
	int verticesCount;
	int machinesCount;
	int routersCount;

public:
	Graph() {
		adjMatrix = nullptr;
		verticesCount = 0;
		routersCount = 0;
		machinesCount = 0;
	}
	int getRoutersCount() {
		return this->routersCount;
	}
	int getVerticesCount() {
		return verticesCount;
	}
	int** getAdjMatrix() {
		return adjMatrix;
	}
	int getMachinesCount() {
		return machinesCount;
	}
	void makeAdjMatrix(int count) {
		adjMatrix = new int* [count];

		for (int i = 0; i < count; i++) {
			adjMatrix[i] = new int[count];
		}
	}


	int getCommasCount(std::string filename = "network.csv") {
		std::fstream file;
		file.open(filename, std::ios::in | std::ios::out);
		if (file.fail()) {
			std::cerr << "Failed To Open File.\n";
			exit(1);
			return -1;
		}
		// istream& getline(istream& is, string& str, char delim);
		// delim is the char it stops reading at
		// what a useful feature :)
		int commasCount = 0;

		std::string firstLine;
		std::getline(file, firstLine);
		for (size_t i = 0; i < firstLine.length(); i++) {
			if (firstLine[i] == ',') {
				commasCount++;
			}
		}
		file.close();
		
		return commasCount;
	}

	void readFromCSV(std::string filename = "network.csv") {
		this->verticesCount = getCommasCount(filename);
		makeAdjMatrix(verticesCount);

		std::fstream file;
		file.open(filename, std::ios::in | std::ios::out);
		if (file.fail()) {
			std::cerr << "Failed To Open File.\n";
			return;
		}
		//std::cout << "->" << verticesCount << std::endl;
		int i = 0, j = 0;
		std::string garbage;
		std::getline(file, garbage); // dump first row read
		while (!file.eof()) {

			for (int k = 0; k <= verticesCount; k++) {
				if (k == 0) {
					std::string temp;
					std::getline(file, temp, ',');
					if (temp[0] == 'M') {
						machinesCount++;
					}
					else if (temp[0] == 'R') {
						routersCount++;
					}
					continue;
				}
				else {
					std::string temp;
					if (k == verticesCount) {
						
						std::getline(file, temp, '\n');
						
						if (temp == "?") {
							adjMatrix[i][j++] = 0; // infinite distance
						}
						else {
							adjMatrix[i][j++] = std::stoi(temp);
						}
						j = 0;
						i++;
					}
					else {
						std::getline(file, temp, ',');
						if (temp == "?") {
							adjMatrix[i][j++] = 0; // no paths exist to your heart :(
						}
						else {
							adjMatrix[i][j++] = std::stoi(temp);
						}
					}
				}
			}
			if (i == verticesCount) {
				break;
			}
		}
		file.close();
	}
	int getRouterConnectedToMachine(int Machine) {

		for (int i = machinesCount; i < verticesCount; i++) {
			if (adjMatrix[Machine][i] != 0) {
				return i;
			}
		}
		return -1;
	}
	NodeDetails* getDij(int src) {
		NodeDetails* info = new NodeDetails;
		std::string* list;


		int* arr = getDijkstraList(this->adjMatrix, src, verticesCount, list);
		
		//printArray(list, 20);

		makePathList(list, verticesCount, src, machinesCount);
		
		
		int outgoingCount = getLinksCount(src);


		info->numOfOutConnections = outgoingCount;
		info->pathList = list;
		info->shortestDistanceList = arr;
		return info;
	}
	int getLinksCount(int src) {
		int count = 0;
		for (int i = 0; i < verticesCount; i++) {
			if (adjMatrix[src][i] != 0) {
				count++;
			}
		}
		return count;
	}
	void printAdjMatrix() {
		for (int i = 0; i < verticesCount; i++) {
			for (int j = 0; j < verticesCount; j++) {
				std::cout << adjMatrix[i][j] << ", ";
			}
			std::cout << std::endl;
		}
	}
	
};
