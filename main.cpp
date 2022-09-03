#include <iostream>
#include "CLI.h"

// add and remove routing table fields (which can mess with shortest path but whatever u want sir :)))))))))   )



int main() {
	
	
	Graph topology;
	topology.readFromCSV();
	RouterType* rInfo = getRouters(topology);
	Router** myR = rInfo->list;
	std::string myT = rInfo->type;
	//CLRBUFFER;
	system("CLS");
	std::cout << "Commands are case sensitive (type \"help\" for commands list)" << std::endl;
	//std::cout << myT << std::endl;
	while (1) {
		
		std::cout << "\nNetwork Emulator >>> ";
		std::string command;
		std::getline(std::cin, command);
		
		std::string parseCmd =  parseCommand(command);
		if (parseCmd == "sendMessage") {
			sendSimpleMessage(topology, myR);
		}
		if (parseCmd == "printRT") {
			printRT(myR, command);
		}
		else if (parseCmd[0] == '?') {
			std::cout << std::endl << parseCmd.substr(1, parseCmd.length()) << std::endl;
		}
		else if (parseCmd == "clearScreen") {
			system("CLS");
		}
		else if (parseCmd == "exit") {
			exit(0);
		}
		else if (parseCmd == "changeEdge") {
			myR = changeEdgeWeight(myR, myT, topology, command);
		}
		else if (parseCmd == "printPath") {
			printPath(topology, command);
		}
		else if (parseCmd == "printPathTo") {
			printPathsTo(topology, command);
		}
		else if (parseCmd == "printPathFrom") {
			printPathsFrom(topology, command);
		}
		else if (parseCmd == "sendMessageFile") {
			sendMessagesByFile(myR, topology, command);
		}
		else if (parseCmd == "help") {
			printHelpCommands();
		}
		else if (parseCmd == "addTableCLI") {
			addToTableUsingCLI(myR, command);
		}
		else if (parseCmd == "addTableFile") {
			addToTableUsingFile(myR, command);
		}
		else if (parseCmd == "removeTableCLI") {
			removeFromTableUsingCLI(myR, command);
		}
		else if (parseCmd == "removeTableFile") {
			removeFromTableUsingFile(myR, command);
		}
		else {
			std::cout << "\nInvalid command (type \"help\" for commands list)" << std::endl;
		}
	}

	return 0;
}
