#pragma once
#include <iostream>
#include "SplayTree.h"
#include <string>
#include "Graph.h"
#include "Globals.h"
#include "Router.h"
#include "Parser.h"
#include <fstream>


#define CLRBUFFER {std::cin.ignore(1000,'\n');; std::cin.clear();}

void printRT(Router** myR, std::string command) {

	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	int index = getRouterIndex(tokens.getAtIndex(2));

	if (index < 0) {
		return;
	}

	myR[index]->printRoutingTable();
}

void printPathsTo(Graph& topology, std::string command) {


	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	// 2 and 3

	int deviceOne = getGraphIndex(tokens.getAtIndex(3), topology.getMachinesCount());

	if (deviceOne == -1 || deviceOne >= topology.getVerticesCount()) {
		exit(1);
	}
	std::cout << "Shortest Path Between Two Machines-> " << std::endl;
	for (int i = 0; i < topology.getMachinesCount(); i++) {
		NodeDetails* info = topology.getDij(i);
		std::cout << info->pathList[deviceOne] << std::endl;
		//printArray(info->pathList, topology.getMachinesCount());
		delete info;
	}
	
}

void printPathsFrom(Graph& topology, std::string command) {


	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	// 2 and 3

	int deviceOne = getGraphIndex(tokens.getAtIndex(2), topology.getMachinesCount());

	if (deviceOne == -1 || deviceOne >= topology.getVerticesCount()) {
		exit(1);
	}

	NodeDetails* info = topology.getDij(deviceOne);
	std::cout << "Shortest Path Between Two Machines-> " << std::endl;
	printArray(info->pathList, topology.getMachinesCount());
}

void writeMessageToFile(Message& msg, std::string filename = "path.txt") {

	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out | std::ios::app);

	if (file.fail()) {
		std::cout << "Failed To Open File, Message Wont Be Written." << std::endl;
		return;
	}

	file << msg << std::endl << std::endl;
	file.close();
}
int getConnectedRouter(std::string src, Graph& tp) {

	int rIndex = tp.getRouterConnectedToMachine(extractInteger(src) - 1);
	// std::cout << rIndex - tp.getMachinesCount() << std::endl;
	return rIndex - tp.getMachinesCount();
}

Message* generateMessage(int id, int priority, Graph& tp, std::string source, std::string destination, std::string msg) {

	Message* newMsg = new Message;
	newMsg->id = id;
	newMsg->priority = priority;
	newMsg->destination = destination;
	newMsg->source = source;
	newMsg->payload = msg;

	int src = getRouterIndex(source);
	int dst = getRouterIndex(destination);
	/*
	//newMsg->trace = tp.getDij(src)->pathList[dst];


	LinkedList<std::string> tokens;
	std::stringstream pathstream(newMsg->trace);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	for (int i = 0; i < tokens.getListSize(); i++) {
		newMsg->path.insert(tokens.getAtIndex(i));
	}
	*/
	return newMsg;
}

RouterType* getRouters(Graph& topology, std::string choice = "?") {
	Router** myR = new Router * [topology.getRoutersCount()];
	RouterType* myI = new RouterType;
	//myR = new TreeRouter;
	//

	int modeChoice = 0;
	if (choice == "?") {
		std::cout << "Creating New Routers..." << std::endl;
		std::cout << "1- LinkedList Routing Tables.\n2- SplayTree Routing Tables.\n>>> ";
		std::cin >> modeChoice;
		CLRBUFFER;
	}
	else if (choice == "list") {
		modeChoice = 1;
	}
	else if (choice == "tree") {
		modeChoice = 2;
	}


	if (modeChoice == 1) {
		if (choice == "?") {
			myI->type = "list";
		}

		for (int i = 0; i < topology.getRoutersCount(); i++) {
			myR[i] = new ListRouter;
		}
	}
	else if (modeChoice == 2) {
		if (choice == "?") {
			myI->type = "tree";
		}
		for (int i = 0; i < topology.getRoutersCount(); i++) {
			myR[i] = new TreeRouter;
		}
	}
	else {
		std::cout << "Wrong Input" << std::endl;
		exit(1);
	}


	for (int i = 0; i < topology.getRoutersCount(); i++) {
		myR[i]->setID(i);
		myR[i]->setTopology(topology);
		myR[i]->generateRoutingTable();

	}
	myI->list = myR;
	return myI;
}

void sendSimpleMessage(Graph& topology, Router** myR) {
	int modeChoice = 1;
	for (int i = 0; modeChoice != 0; i++) {
		system("CLS");
		std::string source;
		std::string destination;
		std::string message;
		int priority = -1;



		std::cout << "Enter Source Machine.\n>>> ";
		std::getline(std::cin, source);

		std::cout << "Enter Destination Machine.\n>>> ";
		std::getline(std::cin, destination);

		std::cout << "Enter Message.\n>>> ";
		std::getline(std::cin, message);

		std::cout << "Enter Priority.\n>>> ";
		std::cin >> priority;

		CLRBUFFER;

		Message* msg = generateMessage(i, priority, topology, source, destination, message);
		myR[getConnectedRouter(source, topology)]->receiveMessage(*msg, myR);
		msg->trace = msg->trace.substr(0, msg->trace.length() - 1);
		std::cout << "Trace Of The Message -> " << msg->trace << std::endl;
		writeMessageToFile(*msg);
		// message ban gaya hai
		// implement a max heap to take message
		// find the router to take messages and send it to machines

		std::cout << "Do You Want To Send Another Message? Press 1 For Another Message Or 0 To End Message Mode.\n>>> ";
		std::cin >> modeChoice;
		
	}
	CLRBUFFER;
}

void deleteRouters(Router** myR, Graph& topology) {
	for (int i = 0; i < topology.getRoutersCount(); i++) {

		delete myR[i];
	}
	//std::cout << "here" << std::endl;
	delete[] myR;
	myR = nullptr;
}

Router** changeEdgeWeight(Router** myR, std::string type, Graph& topology, std::string command) {

	deleteRouters(myR, topology);
	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	int deviceOne = getGraphIndex(tokens.getAtIndex(2), topology.getMachinesCount());
	int deviceTwo = getGraphIndex(tokens.getAtIndex(3), topology.getMachinesCount());
	//std::cout << deviceOne << "-" << deviceTwo << std::endl;

	if (deviceOne == -1 || deviceTwo == -1 || deviceOne >= topology.getVerticesCount() || deviceTwo >= topology.getVerticesCount()) {
		exit(1);
	}
	topology.getAdjMatrix()[deviceOne][deviceTwo] = std::stoi(tokens.getAtIndex(4));
	topology.getAdjMatrix()[deviceTwo][deviceOne] = std::stoi(tokens.getAtIndex(4));
	myR = getRouters(topology, type)->list;
	std::cout << "\nEdge Weight Updated." << std::endl;
	return myR;
}


void printPath(Graph& topology, std::string command) {

	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	// 2 and 3

	int deviceOne = getGraphIndex(tokens.getAtIndex(2), topology.getMachinesCount());
	int deviceTwo = getGraphIndex(tokens.getAtIndex(3), topology.getMachinesCount());

	if (deviceOne == -1 || deviceTwo == -1 || deviceOne >= topology.getVerticesCount() || deviceTwo >= topology.getVerticesCount()) {
		exit(1);
	}

	NodeDetails* info = topology.getDij(deviceOne);
	std::cout << "Shortest Path Between Two Machines-> " << info->pathList[deviceTwo] << std::endl;
}

void sendMessagesByFile(Router** myR, Graph& topology, std::string command) {

	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	std::string filename = tokens.getAtIndex(2);
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (file.fail()) {
		std::cerr << "Failed To Open Messages File" << std::endl;
		exit(1);
	}
	LinkedList<std::string> messageList;
	std::string fileData;
	while (!file.eof()) {

		std::string temp;
		std::getline(file, temp);

		messageList.insert(temp);

	}
	file.close();
	Message* msgs = new Message[messageList.getListSize()];

	for (int i = 0; i < messageList.getListSize(); i++) {

		LinkedList<std::string> messageToken;
		std::stringstream messageStream(messageList.getAtIndex(i));
		std::string tempTokens;
		while (std::getline(messageStream, tempTokens, ':')) {
			messageToken.insert(tempTokens);
		}
		msgs[i].id = std::stoi(messageToken.getAtIndex(0));
		msgs[i].priority = std::stoi(messageToken.getAtIndex(1));
		msgs[i].source = messageToken.getAtIndex(2);
		msgs[i].destination = messageToken.getAtIndex(3);
		msgs[i].payload = messageToken.getAtIndex(4);

	}
	int modeChoice = 0;
	std::cout << "\n1- Send All Messages From File.\n2- Send A Particular Message.\n>>> ";
	std::cin >> modeChoice;

	if (modeChoice == 1) {
		MaxHeap<Message> priorityMsgs;

		for (int i = 0; i < messageList.getListSize(); i++) {

			priorityMsgs.insert(msgs[i]);
		}
		// fileData = fileData.substr(0, fileData.length() - 1);
		// std::cout << fileData;
		//std::cout << priorityMsgs.getArray()[0] << std::endl;
		for (int i = 0; i < messageList.getListSize(); i++) {
			Message toSend = priorityMsgs.getMax();
			static_cast<void>(priorityMsgs.popMax());
			std::cout << "Sending The Following Message\n";
			std::cout << toSend << std::endl;
			myR[getConnectedRouter(toSend.source, topology)]->receiveMessage(toSend, myR);
			toSend.trace = toSend.trace.substr(0, toSend.trace.length() - 1);
			std::cout << "Trace->" << toSend.trace << std::endl << std::endl;
			writeMessageToFile(toSend);
		}
	}
	else if (modeChoice == 2) {
		for (int i = 0; i < messageList.getListSize(); i++) {
			std::cout << msgs[i] << std::endl << std::endl;
		}
		std::cout << "\nSelect Message Id To Send\n>>> ";
		std::cin >> modeChoice;
		if (modeChoice > messageList.getListSize() || modeChoice <= 0) {
			std::cout << "ID Does Not Exit" << std::endl;
			return;
		}
		std::cout << "\nSending The Following Message\n";
		std::cout << msgs[modeChoice - 1] << std::endl;
		myR[getConnectedRouter(msgs[modeChoice - 1].source, topology)]->receiveMessage(msgs[modeChoice - 1], myR);
		msgs[modeChoice - 1].trace = msgs[modeChoice - 1].trace.substr(0, msgs[modeChoice - 1].trace.length() - 1);
		std::cout << "Trace->" << msgs[modeChoice - 1].trace << std::endl << std::endl;
		writeMessageToFile(msgs[modeChoice - 1]);
	}
	else {
		std::cout << "Wrong Input" << std::endl;
		exit(1);
	}
	CLRBUFFER;
}




void printHelpCommands() {

	std::cout << "-> send msg (to send a custom message)" << std::endl;
	std::cout << "-> send msg filename (to send messages from file)" << std::endl;
	std::cout << "-> print path M1 M2 (to print path between two machines)" << std::endl;
	std::cout << "-> print path M1 * (to print path between machine to all others machines)" << std::endl;
	std::cout << "-> print path * M1 (to print path between from all others machines to a specific machine)" << std::endl;	
	std::cout << "-> change edge R1 M2 13 (to change an edge weight dynamically between two devices)" << std::endl;
	std::cout << "-> print RT R1 (to print a routing table)" << std::endl;
	std::cout << "-> change RT R1 add M9 R4 (to add a routing field to router, M9 = destination, R4 = next device)" << std::endl;
	std::cout << "-> change RT R1 add filename (to add to routing tables using filename)" << std::endl;
	std::cout << "-> change RT R1 remove M9 R4 (to remove(if exists) a routing field from router, M9 = destination, R4 = next device)" << std::endl;
	std::cout << "-> change RT R1 remove filename (to remove from routing tables using filename)" << std::endl;
	std::cout << "-> cls (to clear the screen)" << std::endl;
	std::cout << "-> exit (to exit program)" << std::endl;


}

void addToTableUsingCLI(Router** myR, std::string command) {

	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	int routerIndex = getRouterIndex(tokens.getAtIndex(2));

	myR[routerIndex]->addToRouterTable(tokens.getAtIndex(4), tokens.getAtIndex(5));
	std::cout << "\nUpdated Routing Table: " << std::endl;
	myR[routerIndex]->printRoutingTable();
}

void addToTableUsingFile(Router** myR, std::string command) {
	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	std::string filename = tokens.getAtIndex(4);
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (file.fail()) {
		std::cerr << "Failed To Open Messages File" << std::endl;
		exit(1);
	}
	LinkedList<std::string> fieldList;
	while (!file.eof()) {

		std::string temp;
		std::getline(file, temp);

		fieldList.insert(temp);

	}
	file.close();
	int routerIndex = getRouterIndex(tokens.getAtIndex(2));
	for (int i = 0; i < fieldList.getListSize(); i++) {
		myR[routerIndex]->addToRouterTable(fieldList.getAtIndex(i).substr(0, fieldList.getAtIndex(i).find(":")), fieldList.getAtIndex(i).substr(fieldList.getAtIndex(i).find(":") + 1, fieldList.getAtIndex(i).length()));
	}
	std::cout << "\nUpdated Routing Table: " << std::endl;
	myR[routerIndex]->printRoutingTable();
}


void removeFromTableUsingCLI(Router** myR, std::string command) {

	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}

	int routerIndex = getRouterIndex(tokens.getAtIndex(2));

	myR[routerIndex]->deleteFromRouterTable(tokens.getAtIndex(4), tokens.getAtIndex(5));
	std::cout << "\nUpdated Routing Table: " << std::endl;
	myR[routerIndex]->printRoutingTable();
}

void removeFromTableUsingFile(Router** myR, std::string command) {
	LinkedList<std::string> tokens;

	std::stringstream pathstream(command);
	std::string temp;

	while (std::getline(pathstream, temp, ' ')) {
		tokens.insert(temp);
	}
	std::string filename = tokens.getAtIndex(4);
	std::fstream file;
	file.open(filename, std::ios::in | std::ios::out);
	if (file.fail()) {
		std::cerr << "Failed To Open Messages File" << std::endl;
		exit(1);
	}
	LinkedList<std::string> fieldList;
	while (!file.eof()) {

		std::string temp;
		std::getline(file, temp);

		fieldList.insert(temp);

	}
	file.close();
	int routerIndex = getRouterIndex(tokens.getAtIndex(2));
	for (int i = 0; i < fieldList.getListSize(); i++) {
		myR[routerIndex]->deleteFromRouterTable(fieldList.getAtIndex(i).substr(0, fieldList.getAtIndex(i).find(":")), fieldList.getAtIndex(i).substr(fieldList.getAtIndex(i).find(":") + 1, fieldList.getAtIndex(i).length()));
	}
	std::cout << "\nUpdated Routing Table: " << std::endl;
	myR[routerIndex]->printRoutingTable();
}