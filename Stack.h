#pragma once
template <typename T>

struct StackNode { // simple node struct with a pointer to next node

	T value;
	StackNode* next;

	StackNode() {
		value = {};
		this->next = nullptr;
	}
};

template <class T>
class LinkedListStack { // a templatized node based stack
	StackNode<T>* head;
public:
	LinkedListStack() {
		this->head = nullptr;
	}
	StackNode<T>* getHead() {
		return this->head;
	}
	void push(T data) {
		if (!this->head) { // pushes a value into the stack
			StackNode<T>* toPush = new StackNode<T>;
			toPush->value = data;
			this->head = toPush;
			toPush = nullptr;
			return;
		}
		StackNode<T>* toPush = new StackNode<T>;
		toPush->value = data;
		toPush->next = this->head;
		this->head = toPush;
		toPush = nullptr;
	}
	bool isEmpty() {
		return !(this->head);
	}
	T peek() {
		return this->head->value;
	}
	T pop() {
		// pops value from the top of stack and returns it
		if (!this->head) {
			return {};
		}

		StackNode<T>* toDelete = this->head;
		this->head = head->next;
		T toReturn = toDelete->value;
		delete toDelete;

		return toReturn;
	}
	void emptyStack() {
		while (!isEmpty()) {
			static_cast<void>(pop());
		}
	}
	int getListSize() {
		int size = 0;
		StackNode<T>* temp = this->head;
		while (temp) {
			size++;
			temp = temp->next;
		}
		return size;
	}
	~LinkedListStack() {
		emptyStack();
	}
};