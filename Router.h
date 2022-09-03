#pragma once
#include <string>
#include <iostream>
#include "LinkedList.h"
#include "Queue.h"
#include "MaxHeap.h"
#include "Message.h"
#include "SplayTree.h"
#include "Globals.h"
#include "Graph.h"
#include <thread>
#include <Windows.h>


template <typename T>
struct namedQueue {
	
	std::string name;
	Queue<T> pQueue;

	namedQueue() {

	}
	namedQueue(std::string name) {
		this->name = name;
	}
};

struct routingTableField {

	std::string nextDevice;
	std::string destination;

	routingTableField() {

	}

	routingTableField(std::string src, std::string dest) {
		this->destination = dest;
		this->nextDevice = src;
	}
	bool operator ==(routingTableField rhs) {
		return this->destination == rhs.destination && this->nextDevice == rhs.nextDevice;
	}
	bool operator !=(routingTableField rhs) {
		return this->destination != rhs.destination || this->nextDevice != rhs.nextDevice;
	}

	bool operator >(routingTableField rhs) { // fix these overloads below, extract substrings by excluding 0th index
		return extractInteger(this->destination) > extractInteger(rhs.destination); // and then compare the leftover integer part
	}												// templatization is a pain and i have regrets
	bool operator <(routingTableField rhs) {
		return extractInteger(this->destination) < extractInteger(rhs.destination);
	}
	bool operator >=(routingTableField rhs) {
		return extractInteger(this->destination) >= extractInteger(rhs.destination);
	}
	bool operator <=(routingTableField rhs) {
		return extractInteger(this->destination) <= extractInteger(rhs.destination);
	}
	friend std::ostream& operator <<(std::ostream& out, const routingTableField& Field);
};
std::ostream& operator <<(std::ostream& out, const routingTableField& Field) {
	out << Field.destination;
	out << "\t\t" << Field.nextDevice;
	return out;
}


class Router {
protected:
	int id;
	Graph topology;
	MaxHeap<Message> incomingQueue; // implement using binary heap
	LinkedList<namedQueue<Message>> outgoingQueueList;
	std::string name;
	int outGoingCount; // represents outgoing queue count and routing table size
						// intermediate connections
public:	

	Router() {
		id = 0;
		outGoingCount = 0;
	}
	virtual void setName(std::string name) = 0;
	virtual void setID(int ID) = 0;
	virtual void setTopology(Graph& topo) = 0;
	virtual void setOutgoingCount(int outGoingCount) = 0;
	virtual void generateTopology() = 0;
	virtual void generateRoutingTable() = 0;
	virtual void sendMessage(Message& msg, Router** routersList) = 0;
	virtual void receiveMessage(Message& msg, Router** routersList) = 0;
	virtual std::string getNextBestPath(Message& msg) = 0;
	virtual void addToRouterTable(std::string destination, std::string nextBest) = 0;
	virtual void printRoutingTable() = 0;
	virtual void deleteFromRouterTable(std::string destination, std::string nextBest) = 0;
	~Router() {
		
	}
};

class ListRouter : public Router {
	LinkedList<routingTableField> routingTable;

public:
	
	ListRouter() {
		id = 0;
		outGoingCount = 0;
	}
	void setID(int ID) {
		//std::cout << "->" << ID << std::endl;
		this->id = ID;
	}
	void setTopology(Graph& topo) {
		this->topology = topo;
	}
	void setName(std::string name) {
		this->name = name;
	}
	void setOutgoingCount(int outGoingCount) {
		this->outGoingCount = outGoingCount;
	}
	void generateTopology() {

		outGoingCount = topology.getLinksCount(id + topology.getMachinesCount());
		this->name = returnMachineId(id + topology.getMachinesCount(), topology.getMachinesCount());
	}

	void generateRoutingTable() {

		generateTopology();

		for (int i = 0; i < topology.getVerticesCount(); i++) {
			if (topology.getAdjMatrix()[id + topology.getMachinesCount()][i] != 0) {
				outgoingQueueList.insert(*(new namedQueue<Message>(returnMachineId(i, topology.getMachinesCount()))));
			}
		}
		routingTableField* table = new routingTableField[topology.getMachinesCount()];
		NodeDetails* info = topology.getDij(id + topology.getMachinesCount());


		for (int i = 0; i < topology.getMachinesCount(); i++) {
			table[i].destination = returnMachineId(i, topology.getMachinesCount());
			table[i].nextDevice = getTokenAtIndex(info->pathList[i], 1);
			
		}
		for (int i = 0; i < topology.getMachinesCount(); i++) {
			routingTable.insert(table[i]);
		}
		//routingTable.printList();
	}
	void receiveMessage(Message& msg, Router** routersList) {
		
		incomingQueue.insert(msg);
		msg = incomingQueue.getMax();
		static_cast<void>(incomingQueue.popMax());
		msg.trace += returnMachineId(id + topology.getMachinesCount(), topology.getMachinesCount()) + " ";
		std::cout << returnMachineId(id + topology.getMachinesCount(), topology.getMachinesCount()) << std::endl;

		
		
		sendMessage(msg, routersList);
		
	}

	void sendMessage(Message& msg, Router** routersList) {
		
		std::string nextPath = getNextBestPath(msg);
		if (nextPath == "directLinkWithDestination" || nextPath == msg.destination) {
			//put in outgoing queue and then break link
			Sleep(1000);
			std::cout << "Message Reached" << std::endl;
		}
		else if (nextPath == "noLinkFound") {
			// tell the mf that no link is found and break recursion

			std::cout << "Message Not Reached" << std::endl;
		}
		else {
			namedQueue<Message>* q = nullptr;
			int routerId = extractInteger(nextPath) - 1;
			
			for (int i = 0; i < topology.getLinksCount(this->id + topology.getMachinesCount()); i++) {
				//std::cout << ">" << topology.getLinksCount(routerId + topology.getMachinesCount()) << std::endl;
				//std::cout << outgoingQueueList.getAtIndex(i).name << " " << nextPath << std::endl;
				if (outgoingQueueList.getAtIndex(i).name == nextPath) {
					//std::cout << std::endl;
					
					outgoingQueueList.getAtIndexRef(i).pQueue.enqueue(msg);
					
					Sleep(1000);
					outgoingQueueList.getAtIndexRef(i).pQueue.virtualDequeue();
					//std::cout << "->" << routerId << std::endl;
					routersList[routerId]->receiveMessage(msg, routersList);
					
					break;
				}
				
			}
		}
	}

	std::string getNextBestPath(Message& msg) {

		for (int i = 0; i < topology.getMachinesCount(); i++) {
			if (msg.destination == routingTable.getAtIndex(i).destination) {
				//std::cout << routingTable.getAtIndex(i).destination << std::endl;
				if (msg.destination == routingTable.getAtIndex(i).nextDevice) {
					return "directLinkWithDestination";
				}
				else {
					return routingTable.getAtIndex(i).nextDevice;
				}
				
			}
		}

		return "noLinkFound";
	}
	void printRoutingTable() {
		std::cout << "\tDestinations\tNext Best Device" << std::endl;
		routingTable.printList();
	}
	void addToRouterTable(std::string destination, std::string nextBest) {

		routingTableField* newField = new routingTableField;

		newField->destination = destination;
		newField->nextDevice = nextBest;
		if (routingTable.isContain(*newField)) {
			delete newField;
			return;
		}
		
		routingTable.insert(*newField);
		//printRoutingTable();

	}
	void deleteFromRouterTable(std::string destination, std::string nextBest) {

		routingTableField* newField = new routingTableField;

		newField->destination = destination;
		newField->nextDevice = nextBest;
		if (routingTable.isContain(*newField)) {
			routingTable.remove(*newField);
			delete newField;
			return;
		}

		delete newField;
	}
	~ListRouter() {

	}
};

class TreeRouter : public Router {

	SplayTree<routingTableField> routingTable;

public:

	TreeRouter() {
		id = 0;
		outGoingCount = 0;
	}
	void setID(int ID) {
		this->id = ID;
	}
	void setName(std::string name) {
		this->name = name;
	}
	void setOutgoingCount(int outGoingCount) {
		this->outGoingCount = outGoingCount;
	}
	void setTopology(Graph& topo) {
		this->topology = topo;
	}
	void generateTopology() {

		outGoingCount = topology.getLinksCount(id + topology.getMachinesCount());
		this->name = returnMachineId(id + topology.getMachinesCount(), topology.getMachinesCount());
	}

	void generateRoutingTable() {

		generateTopology();

		for (int i = 0; i < topology.getVerticesCount(); i++) {
			if (topology.getAdjMatrix()[id + topology.getMachinesCount()][i] != 0) {
				outgoingQueueList.insert(*(new namedQueue<Message>(returnMachineId(i, topology.getMachinesCount()))));
			}
		}
		routingTableField* table = new routingTableField[topology.getMachinesCount()];
		NodeDetails* info = topology.getDij(id + topology.getMachinesCount());


		for (int i = 0; i < topology.getMachinesCount(); i++) {
			table[i].destination = returnMachineId(i, topology.getMachinesCount());
			table[i].nextDevice = getTokenAtIndex(info->pathList[i], 1);

		}
		//std::cout << std::endl << name << std::endl;
		//std::cout << "Destination     Next Best Path" << std::endl;
		for (int i = topology.getMachinesCount() - 1; i >= 0; i--) {
			routingTable.insert(table[i]);
		}
		//routingTable.levelOrderTraversal();
	}

	void receiveMessage(Message& msg, Router** routersList) {

		incomingQueue.insert(msg);
		msg = incomingQueue.getMax();
		static_cast<void>(incomingQueue.popMax());
		msg.trace += returnMachineId(id + topology.getMachinesCount(), topology.getMachinesCount()) + " ";
		std::cout << returnMachineId(id + topology.getMachinesCount(), topology.getMachinesCount()) << std::endl;



		sendMessage(msg, routersList);

	}

	void sendMessage(Message& msg, Router** routersList) {

		std::string nextPath = getNextBestPath(msg);
		if (nextPath == "directLinkWithDestination" || nextPath == msg.destination) {
			//put in outgoing queue and then break link
			Sleep(1000);
			std::cout << "Message Reached" << std::endl;
		}
		else if (nextPath == "noLinkFound") {
			// tell the mf that no link is found and break recursion

			std::cout << "Message Not Reached" << std::endl;
		}
		else {
			namedQueue<Message>* q = nullptr;
			int routerId = extractInteger(nextPath) - 1;
			
			for (int i = 0; i < topology.getLinksCount(this->id + topology.getMachinesCount()); i++) {
				//std::cout << ">" << topology.getLinksCount(routerId + topology.getMachinesCount()) << std::endl;
				//std::cout << outgoingQueueList.getAtIndex(i).name << " " << nextPath << std::endl;
				if (outgoingQueueList.getAtIndex(i).name == nextPath) {
					//std::cout << std::endl;

					outgoingQueueList.getAtIndexRef(i).pQueue.enqueue(msg);

					Sleep(1000);
					outgoingQueueList.getAtIndexRef(i).pQueue.virtualDequeue();
					//std::cout << "->" << routerId << std::endl;
					
					routersList[routerId]->receiveMessage(msg, routersList);
					
					break;
				}
				
			}
		}

	}

	std::string getNextBestPath(Message& msg) {

		routingTableField* toSearch = new routingTableField;
		toSearch->destination = msg.destination;
		routingTable.splay(*toSearch);

		delete toSearch;

		if (routingTable.getRoot()->data.destination == msg.destination && routingTable.getRoot()->data.nextDevice == msg.destination) {
			return "directLinkWithDestination";
		}
		else if (routingTable.getRoot()->data.destination == msg.destination) {
			return routingTable.getRoot()->data.nextDevice;
		}
		return "noLinkFound";
	}
	void printRoutingTable() {
		std::cout << "\tDestinations\tNext Best Device" << std::endl;
		routingTable.levelOrderTraversal();
	}
	void addToRouterTable(std::string destination, std::string nextBest) {

		routingTableField* newField = new routingTableField;

		newField->destination = destination;
		newField->nextDevice = nextBest;
		routingTable.splay(*newField);
		if (routingTable.getRoot()->data == *newField) {
			delete newField;
			return;
		}
		routingTable.insert(*newField);
		//printRoutingTable();

	}

	void deleteFromRouterTable(std::string destination, std::string nextBest) {

		routingTableField* newField = new routingTableField;

		newField->destination = destination;
		newField->nextDevice = nextBest;
		routingTable.splay(*newField);
		if (routingTable.getRoot()->data == *newField) {
			routingTable.remove(*newField);
			std::cout << *newField << std::endl;
			delete newField;
			return;
		}
		delete newField;
	}

	~TreeRouter() {

	}

};

struct RouterType {

	Router** list;
	std::string type;

	RouterType() {
		list = nullptr;
	}
};