#pragma once
#include <iostream>
#include <Windows.h>
#include "LinkedList.h"
#include <sstream>
#include <string>

bool isStringNumber(std::string str) {
    for (size_t i = 0; i < str.length(); i++) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }

    return true;
}


// here write a parser that can detect input
// error message and clear screen for wrong input
// right input then run apppropriate codes :))))))

std::string parseCommand(std::string command) {

    LinkedList<std::string> tokens;

    std::stringstream pathstream(command);
    std::string temp;

    while (std::getline(pathstream, temp, ' ')) {
        tokens.insert(temp);
    }

    if (tokens.getAtIndex(0) == "help" && tokens.getListSize() == 1) {
        return "help";
    }
    if ((tokens.getAtIndex(0) == "Ammar" || tokens.getAtIndex(0) == "ammar") && tokens.getListSize() == 1) {
        return "?EZCLAP LOL";
    }
    if ((tokens.getAtIndex(0) == "Daniyal" || tokens.getAtIndex(0) == "daniyal") && tokens.getListSize() == 1) {
        return "?BOSS\n.\n.\n.\nlol";
    }
    if ((tokens.getAtIndex(0) == "Aiza" || tokens.getAtIndex(0) == "aiza") && tokens.getListSize() == 1) {
        return "?YE HAIN SKILLS!!!!!!!!";
    }
    if (tokens.getAtIndex(0) == "exit" && tokens.getListSize() == 1) {
        return "exit";
    }
    if (tokens.getAtIndex(0) == "cls" && tokens.getListSize() == 1) {
        //system("CLS");
        return "clearScreen";
    }
    if (tokens.getListSize() < 2) {
        return "invalidCommand";
    }
    
    if (tokens.getListSize() == 3 && tokens.getAtIndex(0) == "print" && tokens.getAtIndex(1) == "RT" && tokens.getAtIndex(2)[0] == 'R') {
        return "printRT";
    }

    if (tokens.getListSize() == 2 && tokens.getAtIndex(0) == "send" && tokens.getAtIndex(1) == "msg") {
        return "sendMessage";
    }

    if (tokens.getListSize() == 3 && tokens.getAtIndex(0) == "send" && tokens.getAtIndex(1) == "msg") {
        return "sendMessageFile";
    }

    if (tokens.getAtIndex(0) == "change" && tokens.getAtIndex(1) == "RT" && tokens.getAtIndex(2)[0] == 'R' && tokens.getAtIndex(3) == "add") {
        if (tokens.getListSize() == 5) {
            return "addTableFile";
        }
        else if (tokens.getListSize() == 6 && tokens.getAtIndex(4)[0] == 'M') {
            return "addTableCLI";
        }
        
    }
    if (tokens.getAtIndex(0) == "change" && tokens.getAtIndex(1) == "RT" && tokens.getAtIndex(2)[0] == 'R' && tokens.getAtIndex(3) == "remove") {
        if (tokens.getListSize() == 5) {
            return "removeTableFile";
        }
        else if (tokens.getListSize() == 6 && tokens.getAtIndex(4)[0] == 'M') {
            return "removeTableCLI";
        }
    }
    if (tokens.getListSize() == 4 && tokens.getAtIndex(0) == "print" && tokens.getAtIndex(1) == "path" && tokens.getAtIndex(2)[0] == 'M' &&  tokens.getAtIndex(3)[0] == 'M') {
        return "printPath";
    }

    if (tokens.getListSize() == 4 && tokens.getAtIndex(0) == "print" && tokens.getAtIndex(1) == "path" && tokens.getAtIndex(2) == "*" && tokens.getAtIndex(3)[0] == 'M') {
        return "printPathTo";
    }

    if (tokens.getListSize() == 4 && tokens.getAtIndex(0) == "print" && tokens.getAtIndex(1) == "path" && tokens.getAtIndex(2)[0] == 'M' && tokens.getAtIndex(3) == "*") {
        return "printPathFrom";
    }

    if (tokens.getListSize() == 5 && tokens.getAtIndex(0) == "change" && tokens.getAtIndex(1) == "edge") {
        if (isStringNumber(tokens.getAtIndex(4))) {
            if ((tokens.getAtIndex(2)[0] == 'R' || tokens.getAtIndex(2)[0] == 'M') && (tokens.getAtIndex(3)[0] == 'R' || tokens.getAtIndex(3)[0] == 'M')) {
                return "changeEdge";
            }
        }
        
    }

    return "invalidCommand";

}