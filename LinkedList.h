#pragma once
#include<iostream>
#include<vector>

using namespace std;


// initialize linked list with all node functionality
template<typename T>
class LinkedList {
public:

	struct Node {
		T data;
		Node* next;
		Node* prev;
	};
private:
	Node* head;
	Node* tail;
	unsigned int nodeCount;


public:
	// part one
	LinkedList();
	void AddHead(const T& data);
	void AddTail(const T& data);
	void AddNodesHead(const T* data, unsigned int count);
	void AddNodesTail(const T* data, unsigned int count);
	unsigned int NodeCount() const;
	void PrintForward() const;
	void PrintReverse() const;
	~LinkedList();

	// part two
	Node* Head();
	const Node* Head() const;
	Node* Tail();
	const Node* Tail() const;
	Node* GetNode(unsigned int index);
	const Node* GetNode(unsigned int index) const;
	T& operator[](unsigned int index);
	const T& operator[](unsigned int index) const;
	Node* Find(const T& data);
	const Node* Find(const T& data) const;
	void FindAll(vector<Node*>& outData, const T& value) const; 
	LinkedList<T>& operator=(const LinkedList<T>& list);
	LinkedList(const LinkedList<T>& list);

	// part three
	void InsertBefore(Node* node, const T& data);
	void InsertAfter(Node* node, const T& data);
	void InsertAt(const T& data, unsigned int index);
	bool operator==(const LinkedList<T>& rhs);

	// part four
	bool RemoveHead();
	bool RemoveTail();
	unsigned int Remove(const T& data);
	bool RemoveAt(unsigned int index);
	void Clear();
	void PrintForwardRecursive(const Node* node) const;
	void PrintReverseRecursive(const Node* node) const;

};

//======CONSTRUCTOR, BIG 3=========//

template<typename T>
LinkedList<T>::LinkedList() {
	head = nullptr;
	tail = nullptr;
	nodeCount = 0;
}

template<typename T>
LinkedList<T>::~LinkedList() {
	for (unsigned int i = 0; i < nodeCount; i++) {
		Node* soonToBeHead = head->next;
		delete head;
		head = soonToBeHead;
	}
}

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& rhs) {
	nodeCount = 0;
	for (unsigned int i = 0; i < rhs.nodeCount; i++) {
		AddTail(rhs[i]);
	}
}

// add information to container
template<typename T>
void LinkedList<T>::AddHead(const T& data) {
	Node* newNode = new Node;
	newNode->data = data;
	newNode->prev = nullptr;
	if (nodeCount > 0) {
		newNode->next = head;
		head->prev = newNode;
		head = newNode;
	}
	else if (nodeCount == 0) {
		head = newNode;
		newNode->next = nullptr;
	}
	tail = head;
	while (tail->next != nullptr) {
		tail = tail->next;
	}
	nodeCount++;

}

template<typename T>
void LinkedList<T>::AddNodesHead(const T* data, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		AddHead(data[count - (i + 1)]);
	}
}

template<typename T>
void LinkedList<T>::AddTail(const T& data) {
	Node* lastNode = new Node;
	lastNode->data = data;
	lastNode->next = nullptr;
	if (nodeCount > 0) {
		lastNode->prev = tail;
		tail->next = lastNode;
		tail = lastNode;
	}
	else if (nodeCount == 0) {
		tail = lastNode;
		lastNode->prev = nullptr;
	}
	head = tail;
	while (head->prev != nullptr) {
		head = head->prev;
	}
	nodeCount++;
}

template<typename T>
void LinkedList<T>::AddNodesTail(const T* data, unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		AddTail(data[i]);
	}
}

template<typename T>
void LinkedList<T>::InsertBefore(Node* node, const T& data) {
	Node* newNode = new Node;
	newNode->data = data;
	newNode->next = node;
	if (node->prev != nullptr) {
		newNode->prev = node->prev;
		node->prev->next = newNode;
	}
	else {
		newNode->prev = nullptr;
		head = newNode;
	}
	node->prev = newNode;
	nodeCount++;
}

template<typename T>
void LinkedList<T>::InsertAfter(Node* node, const T& data) {
	Node* newNode = new Node;
	newNode->data = data;
	newNode->prev = node;
	if (node->next != nullptr) {
		newNode->next = node->next;
		newNode->next->prev = newNode;
	}
	else {
		newNode->next = nullptr;
		tail = newNode;
	}
	node->next = newNode;
	nodeCount++;

}

template<typename T>
void LinkedList<T>::InsertAt(const T& data, unsigned int index) {
	if (index > nodeCount) {
		throw out_of_range("Index out of range");
	}
	else if (index >= 1) {
		InsertAfter(GetNode(index - 1), data);
	}
	else if (index < 1) {
		InsertBefore(GetNode(index), data);
	}
}

// remove information from container
template<typename T>
bool LinkedList<T>::RemoveHead() {
	if (head != nullptr) {
		if (head->next != nullptr) {
			Node* soonToBeHead = head->next;
			delete head;
			head = soonToBeHead;
			head->prev = nullptr;
		}
		else if (head == tail) {
			delete head;
			head = nullptr;
			tail = nullptr;
		}
		nodeCount--;
		return true;
	}
	else {
		return false;
	}
}

template<typename T>
bool LinkedList<T>::RemoveTail() {
	if (tail != nullptr) {
		if (tail->prev != nullptr) {
			Node* soonToBeTail = tail->prev;
			delete tail;
			tail = soonToBeTail;
			tail->next = nullptr;
		}
		else if (head == tail) {
			delete tail;
			head = nullptr;
			tail = nullptr;
		}
		nodeCount--;
		return true;
	}
	else {
		return false;
	}
}

template<typename T>
unsigned int LinkedList<T>::Remove(const T& data) {
	unsigned int count = 0;
	vector<Node*> outData;
	FindAll(outData, data);
	for (unsigned int i = 0; i < outData.size(); i++) {
		outData[i]->next->prev = outData[i]->prev;
		outData[i]->prev->next = outData[i]->next;
		delete outData[i];
		nodeCount--;
		count++;
	}

	return count;
}

template<typename T>
bool LinkedList<T>::RemoveAt(unsigned int index) {
	if (index <= (nodeCount - 1)) {
		Node* toBeRemoved = GetNode(index);
		toBeRemoved->next->prev = toBeRemoved->prev;
		toBeRemoved->prev->next = toBeRemoved->next;
		delete toBeRemoved;
		nodeCount--;
		return true;
	}
	else {
		return false;
	}
}

template<typename T>
void LinkedList<T>::Clear() {
	for (unsigned int i = 0; i < nodeCount; i++) {
		RemoveAt(i);
	}
	tail = nullptr;
	head = nullptr;
}
//=======ACCESORS=========//

template<typename T>
unsigned int LinkedList<T>::NodeCount() const {
	return nodeCount;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::Head() {
	return head;
}

template<typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Head() const {
	return head;
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::Tail() {
	return tail;
}

template<typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Tail() const {
	return tail;
}


template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::GetNode(unsigned int index) {
	if (index > (nodeCount - 1)) {
		throw out_of_range("index out of range");
	}
	if (index < 0) {
		throw out_of_range("index out of range");
	}
	else {
		Node* indexedNode = head;
		for (unsigned int i = 0; i < index; i++) {
			indexedNode = indexedNode->next;
		}
		return indexedNode;
	}

}

template<typename T>
const typename LinkedList<T>::Node* LinkedList<T>::GetNode(const unsigned int index) const {
	if (index > (nodeCount - 1)) {
		throw out_of_range("index out of range");
	}
	if (index < 0) {
		throw out_of_range("index out of range");
	}
	else {
		Node* indexedNode = head;
		for (unsigned int i = 0; i < index; i++) {
			indexedNode = indexedNode->next;
		}
		return indexedNode;
	}
}

//======BEHAVIOURS======//
template<typename T>
void LinkedList<T>::PrintForward() const {
	Node* toBePrinted = head;
	while (toBePrinted != nullptr) {
		cout << toBePrinted->data << endl;
		toBePrinted = toBePrinted->next;
	}
}

template<typename T>
void LinkedList<T>::PrintForwardRecursive(const Node* node) const {
	if (node == tail) {
		cout << node->data << endl;
		return;
	}
	else {
		cout << node->data << endl;
		PrintForwardRecursive(node->next);
	}
}

template<typename T>
void LinkedList<T>::PrintReverse() const {
	Node* reverseToBePrinted = tail;
	while (reverseToBePrinted != nullptr) {
		cout << reverseToBePrinted->data << endl;
		reverseToBePrinted = reverseToBePrinted->prev;
	}
}

template<typename T>
void LinkedList<T>::PrintReverseRecursive(const Node* node) const {
	if (node == head) {
		cout << node->data << endl;
		return;
	}
	else {
		cout << node->data << endl;
		PrintReverseRecursive(node->prev);
	}
}

template<typename T>
typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) {
	for (Node* i = head; i != nullptr; i = i->next) {
		if (i->data == data) {
			return i;
		}
	}
	return nullptr;
}

template<typename T>
const typename LinkedList<T>::Node* LinkedList<T>::Find(const T& data) const {
	for (Node* i = head; i != nullptr; i = i->next) {
		if (i->data == data) {
			return i;
		}
	}
	return nullptr;
}

template<typename T>
void LinkedList<T>::FindAll(vector<Node*>& outData, const T& value) const {
	Node* findingNodes = head;
	while (findingNodes != nullptr) {
		if (findingNodes->data == value) {
			outData.push_back(findingNodes);
		}
		findingNodes = findingNodes->next;
	}
}

//========OPERATORS=========//

template<typename T>
T& LinkedList<T>::operator[](unsigned int index) {
	if (index < 0 || (index > nodeCount)) {
		throw out_of_range("index out of range");
	}
	else {
		return GetNode(index)->data;
	}
}

template<typename T>
const T& LinkedList<T>::operator[](unsigned int index) const {
	if (index < 0 || index > nodeCount) {
		throw out_of_range("index out of range");

	}
	else {
		return GetNode(index)->data;
	}
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
	while (head != nullptr) {
		Node* soonToBeHead = head->next;
		delete head;
		head = soonToBeHead;
	}
	nodeCount = 0;
	for (unsigned int i = 0; i < rhs.NodeCount(); i++) {
		AddTail(rhs[i]);
	}
	return *this;
}

template<typename T>
bool LinkedList<T>::operator==(const LinkedList<T>& rhs) {
	if (nodeCount == rhs.NodeCount()) {
		int loop = 0;
		for (Node* i = head; i != nullptr; i = i->next) {
			if (i->data != rhs[loop]) {
				return false;
			}
			loop++;
		}
		return true;
	}
	else {
		return false;
	}
}