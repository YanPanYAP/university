#include <iostream>
#include <Windows.h>
#include "HT.h"
using namespace std;
using namespace HT;

int main() {
	HT::HTHANDLE* ht = nullptr;
	try {
		ht = Create(1000, 3, 16, 256, "C:\\Users\\user\\Desktop\\VS\\сп\\SP\\space\\HTspace.ht");
		if (ht) cout << "-- Create: success";
		else throw "-- Create: error";

		if (Insert(ht, new Element("key222", 7, "payload", 8))) cout << "-- Insert: success";
		else throw "-- Insert: error";

		Element* hte = Get(ht, new Element("key222", 7));
		if (hte) cout << "-- Get: success";
		else throw "-- Get: error";

		print(hte);

		if (Snap(ht)) cout << "-- Snap: success";
		else throw "-- Snap: error";

		if (Update(ht, hte, "newpayload", 11)) cout << "-- Update: success";
		else throw "-- Update: error";

		Element* hte1 = Get(ht, new Element("key222", 7));
		if (hte1) cout << "-- Get: success";
		else throw "-- Get: error";

		print(hte1);

		if (Delete(ht, hte1)) cout << "-- Delete: success";
		else throw "-- Delete: error";

		if (Close(ht)) cout << "-- Close: success";
		else throw "-- Close: error";

	}
	catch (char* msg) {
		cout << msg << "\n";
		if (ht != nullptr) cout << GetLastError(ht);
	}
}