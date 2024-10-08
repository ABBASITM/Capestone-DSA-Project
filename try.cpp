#include "DHT.h" 

int main() {
	DHT<int> myDHT;
	int n = -1;
	cout << "Enter number of bits 'n' of identifier space (Space will be of size 2^n ) : ";
	cin >> n;
	myDHT.makeSpace(n);
	myDHT.printSpace();
	myDHT.addMachines();
	myDHT.printSpace(); 
	myDHT.updateRoutingTables(); 
	//myDHT.printRoutingTables(); 
	myDHT.printAny();
	return 0;
}