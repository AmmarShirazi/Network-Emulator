#pragma once
#include <string>
#include <iostream>
#include "Graph.h"
#include "Globals.h"
#include "Stack.h"
struct Message {

	int id;
	int priority;
	std::string payload;
	std::string source;
	std::string destination;
	std::string trace;
	LinkedList<std::string> path;


	Message() {
		id = 0;
		priority = 0;
	}

	bool operator <(Message rhs) {
		return this->priority < rhs.priority;
	}
	bool operator >(Message rhs) {
		return this->priority > rhs.priority;
	}
	bool operator <=(Message rhs) {
		return this->priority <= rhs.priority;
	}
	bool operator >=(Message rhs) {
		return this->priority >= rhs.priority;
	}
	bool operator ==(Message rhs) {
		return this->priority == rhs.priority;
	}
	friend std::ostream& operator <<(std::ostream& out, const Message& msg);
};
std::ostream& operator <<(std::ostream& out, const Message& msg) {
	out << "ID" << "\t\t" << "Priority" << "\t" << "Pay Load" << "\t" << "Source" << "\t\t" << "Destination" << "\t" << "Trace\n";
	out << msg.id << "\t\t" << msg.priority << "\t\t" << msg.payload << "\t\t" << msg.source << "\t\t" << msg.destination << "\t\t" << msg.trace;
	return out;
}