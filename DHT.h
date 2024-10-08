#ifndef  DHT_H
#define DHT_H

#pragma once 
#include "RT.h"
#include <iostream> 
//#include <filesystem>
using namespace std;


template <typename T>
class Machine {
public:
	string data;
	T id; // <- this is our KEY
	RT<int>routingTable;
	T* machinesArray;
	Machine* next;
	Machine* prev;
	bool isOn;
	Machine(T x = NULL) {
		id = x; // KEY being assigned to the machine i.e. id being generated
		next = nullptr;
		prev = nullptr;
		routingTable.head = nullptr;
		isOn = false;
	}

	void printTable() {
		//routingTable.print(); 

		Node<int>* temp = routingTable.head;
		while (temp != NULL) {
			cout << temp->machine->id << " ";
			temp = temp->next;
		}
	}
};

template <typename T>
class DHT {
public:
	Machine<T>* head;
	Machine<T>* tail;
	int idspace, noOfMachines, n; // n:- number of bits, idspace:- identifier space
	T* machinesArray;
	DHT(int x = 0) {
		srand(time(0));
		head = nullptr;
		n = x;
		idspace = noOfMachines = 0;
		machinesArray = nullptr;
	}

	void insert(T item) {
		if (!head) {
			head = new Machine<T>(item);
			head->next = head;
			head->prev = head;
		}
		else {
			Machine<T>* newNode = new Machine<T>(item);
			Machine<T>* curr = head->prev;
			curr->next = newNode;
			newNode->prev = curr;
			newNode->next = head;
			head->prev = newNode;
		}
	}

	void remove(T item) {
		if (head == nullptr) {
			cout << "KHALI LIST" << endl;
			return;
		}

		Machine<T>* curr = head;
		do {
			if (curr->data == item) {
				Machine<T>* prevNode = curr->prev;
				Machine<T>* nextNode = curr->next;

				prevNode->next = nextNode;
				nextNode->prev = prevNode;

				if (curr == head) {
					head = nextNode;
				}

				delete curr;
				return;
			}

			curr = curr->next;
		} while (curr != head);
	}



	void makeSpace(int x) {
		n = x;
		if (x < 0)
		{
			cout << "\nInvalid bits\n";
			return;
		}

		idspace = pow(2, n);

		int i = 0;
		T machineID = 0;
		while (i < idspace) {
			insert(machineID++);
			i++;

		}

		machinesArray = new T[idspace];
	}



	void addMachines() {
		if (idspace <= 0)
		{
			cout << "\nNo space\n";
			return;
		}
		int i = 0, idx = 0, idsassigned = 0;
		bool duplicate = false;
		T machineId = NULL;

		cout << "Enter number of machines : ";
		cin >> noOfMachines;
		cout << '\n';

		while (noOfMachines > idspace || noOfMachines < 0)
		{
			cout << "Please enter a valid number : ";
			cin >> noOfMachines;
		}

		cout << '\n';
		while (i < noOfMachines)
		{
			cout << "Enter machine to turn on (-1 if you want an automatically assigned machine) [" << i << "] : ";
			cin >> machineId;


			while (machineId < -1 || machineId >= idspace)
			{
				cout << "Please enter a valid number : ";
				cin >> machineId;
			}

			if (machineId == -1) {
				machineId = rand() % idspace + 1;

				idx = 0;


				while (idx < i)
				{
					if (machinesArray[idx] == machineId)
					{
						duplicate = true;
						break;
					}
					idx++;
				}

				while (duplicate)
				{
					duplicate = false;
					machineId = rand() % idspace + 1;

					idx = 0;

					while (idx < i)
					{
						if (machinesArray[idx] == machineId)
						{
							duplicate = true;
							break;
						}
						idx++;
					}
				}
			}

			Machine<T>* currMech = head;
			while (currMech->next && currMech->id != machineId)
				currMech = currMech->next;
			currMech->isOn = true;

			machinesArray[i] = machineId;
			i++;
			cout << '\n';
		}
		return;
	}


	/*void createRoutingTable() {
		int i = 1;
		bool found = false;
		Machine<T>* currNode = this;
		T nextkey;
		while (currNode->next != head && i != n)
		{
			nextKey += pow(2, i - 1);
			nextKey %= n;
			while (currNode->routingTable == nullptr)
			{
				if (currNode->key == nextKey)
					found = true;
				currNode = currNode->next;
			}
			if (found)
				routingTable.insert(currNode);
			i++;
		}

	}*/


	void updateRoutingTables() {
		Machine<T>* currNode = head;
		int i = 0;
		bool succFound = false, mechFound = false, lesgo = false;
		T nextKey;
		while (i < noOfMachines) {
			currNode = head;
			while (currNode->next != head && currNode->id != machinesArray[i])
				currNode = currNode->next;
			currNode->routingTable.head = nullptr;
			int a = 0;
			//cout << "Machine : " << currNode->id << "\nRouting Table\n";

			while (a < n) {
				mechFound = succFound = lesgo = false;
				nextKey = currNode->id + pow(2, a);
				nextKey %= idspace;
				Machine<T>* findNode = currNode;
				do {
					if (findNode->isOn)
						succFound = true;
					if (findNode->id == nextKey)
						mechFound = true;
					if (!mechFound && succFound)
						succFound = false;

					if (succFound && mechFound)
					{
						lesgo = true;
						break;
					}
					findNode = findNode->next;
				} while (findNode->next != currNode && !lesgo);

				if (succFound && mechFound)
					currNode->routingTable.insertAtEnd(findNode);
				a++;
			}

			i++;
		}
	}



	void printSpace() {
		if (head == nullptr) {
			cout << "KHALI LIST" << endl;
			return;
		}

		Machine<T>* curr = head;
		do {
			cout << curr->id;
			if (curr->isOn)
				cout << "(ON)";
			cout << " ";
			curr = curr->next;
		} while (curr != head);
		cout << endl;
	}

	void printRoutingTables() {
		if (head == nullptr) {
			cout << "KHALI LIST" << endl;
			return;
		}

		Machine<T>* curr = head;
		do {
			cout << curr->id;
			if (curr->isOn) {
				curr->printTable();
			}
			cout << '\n';
			curr = curr->next;
		} while (curr != head);
		cout << '\n';
	}


	void printAny() {
		char choice;

		do {
			T machineid;
			cout << "\nEnter machine id : ";
			cin >> machineid;

			while (machineid < 0 || machineid >= idspace) {
				cout << "Please enter a valid id : ";
				cin >> machineid;
			}

			Machine<T>* currMech = head;

			while (currMech && currMech->id != machineid)
				currMech = currMech->next;

			if (currMech) {
				if (currMech->routingTable.head) {
					currMech->printTable();
					cout << '\n';
				}
				else {
					cout << "This machine does not exist yet.\n";
				}
			}
			else {
				cout << "Machine not found.\n";
			}

			cout << "\nDo you want to print the routing table of another machine? (y/n): ";
			cin >> choice;

		} while (choice == 'y' || choice == 'Y');
	}

};

#endif // ! DHT_H


//int main() {
//	LL<int> list;
//
//	list.insert(1);
//	list.insert(2);
//	list.insert(3);
//	list.insert(4);
//	list.insert(5);
//	list.insert(534634);
//	list.insert(23);
//	list.insert(25);
//	list.print();
//	list.remove(23);
//	list.print();
//
//
//	return 0;
//}