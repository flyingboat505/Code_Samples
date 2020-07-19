/******************************************************************************

 Complete implementation of SecurityLookup class defined below.

 This class maintains Security objects and provides methods for their lookup
 via Security attributes.

 • Every Security object has a unique id (id_)
 • Every Security object has a unique symbol (symbol_)
 • Every Security object has a (potentially non-unique) channel id (channel_)

 You are responsible for implementing the following methods:

 Security GetSecurity(int id) const

 Return Security object with given id or default (invalid) Security object if not found

 Security GetSecurity(const std::string& symbol) const

 Return Security object with given symbol or default (invalid) Security object if not found

 Securities GetSecurities(int channel) const

 Returns all Security objects with given channel value
 or an empity List of Security objects, if not matching securities are found

 void SaveSecurity(const Security& sec)

 Add a Security object to the lookup instance

 If a Security with a given id already exists:
 • Update Security with new channel id

 Before state:
 Security: 1, "MSFT", 1
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 Mutations:
 Save Security: 1, "MSFT, 2

 After state:
 Security: 1, "MSFT", 2
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 • Update Security with new symbol

 Before state:
 Security: 1, "MSFT", 1
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 Mutations:
 Save Security: 1, "AAPL", 1

 After state:
 Security: 1, "AAPL", 1
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 • Throw std::runtime_error if symbol is being used by an another Security

 Before state:
 Security: 1, "MSFT", 1
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 Mutations:
 Save Security 1, "GOOG", 1
 <<< Exception! >>>

 After state:
 Security: 1, "MSFT", 1
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 • Do not throw std::runtime_error if symbol is in use by the same Security (NOOP)

 Before state:
 Security: 1, "MSFT", 1
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 Mutations:
 Save Security 1, "MSFT", 1

 After state:
 Security: 1, "MSFT", 1
 Security: 2, "TSLA", 1
 Security: 3, "GOOG", 1

 If a Security with a given id does not exist:
 • Save new Security

 Before state:
 Security: 1, "MSFT", 1

 Mutations:
 Save Security: 2, "TSLA", 2

 After state:
 Security: 1, "MSFT", 1
 Security: 2, "TSLA", 2


 The expected number of Security objects within this Lookup is on the order of
 10s of millions.  The choice of underlying data structures must ensure
 efficient lookups.

 Note:
 - Feel free to use a text editor or IDE of your choice to do this instead.
 - If you do, simply just copy-paste your code here when you're done.
 - You are free to add any supporting helper functions, data types, or
 structures you wish.
 - You may make use of STL data structures and algorithms
 - main function has been provided to make code run inside Rextester Web
 IDE. It is not required to be part of the submission and will not be
 graded.

 ******************************************************************************/

#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

//edge cases- if id{0} then it's null s
#include <iostream>
using namespace std;
struct Security {
	uint32_t id_;
	std::string symbol_;
	uint16_t channel_;

	Security() :
			id_ { 0 }, channel_ { 0 } {
	}

	Security(uint32_t id, std::string symbol, uint16_t channelId) :
			id_(id), symbol_(symbol), channel_(channelId) {
	}

	bool IsValid() const {
		return (id_ != 0);
	}
};

ostream& operator <<(ostream &out, const Security &sec) {
	out << "ID " << sec.id_ << " symbol " << sec.symbol_ << " channel_ "
			<< sec.channel_ << endl;
	return out;
}

bool operator==(const Security &sec, const Security &sec2) {
	if (sec2.symbol_ == sec.symbol_ && sec2.channel_ == sec.channel_
			&& sec2.id_ == sec.id_)
		return true;
	else
		return false;
}

using Securities = std::vector<Security>;

class SecurityLookup {
public:
// Security GetSecurity(int id) const
//Return Security object with given id or default (invalid) Security object if not found
	Security GetSecurity(int id) const {
		auto it = id_map.find(id);
		if (it == id_map.end()) //nothing found
			return Security();
		else {
			Security *x = (Security*) it->second;
			return *x;
		}
	}

	Security GetSecurity(const std::string &symbol) const {
		auto it = symbol_map.find(symbol);
		if (it == symbol_map.end()) //nothing found
			return Security();
		else {
			Security *x = (Security*) it->second;
			return *x;
		}
	}

	Securities GetSecurities(int channel) const {
		auto it = channel_map.find((uint16_t) channel);
		if (it == channel_map.end()) //nothing found
			return Securities();
		else
			return it->second;
	}

	void SaveSecurity(const Security &sec) {
		//empty object added (assuming ids are reserved)
		if (!sec.IsValid()) { //call IsValid
			cout << "Empty Object-Not Added " << endl;
			return;
		}
		if (sec.symbol_.size() == 0) {
			return;
		}

		Security current_sec = GetSecurity(sec.id_);
		Security symbol_sec = GetSecurity(sec.symbol_);
		//if id does not exist,just insert object DOUBLE CHECK
		if (!current_sec.IsValid()) {
			// add a test case if Securty is valid{} return true
			if (symbol_sec.IsValid()) { //Securty Object already exists
				throw std::runtime_error("Security Symbol Already in Use!");
				return;
			}
			insert_ChannelObject(sec.channel_, sec);
			insert_IdObject(sec.id_, sec);
			insert_SymbolObject(sec.symbol_, sec);
			return;
		}
		//sec.ID DOES EXIST in the systerm
		else {
			//SYMBOL IS THE SAME and CHANNEL THE SAME- update the channel ID
			if (current_sec.symbol_ == sec.symbol_
					and current_sec.id_ == sec.id_) {
				if (current_sec.channel_ == sec.channel_) {
					return;
				} else {
					//update channelObject and respective Addresses
					remove_ChannelObject(current_sec.channel_, current_sec);
					insert_ChannelObject(sec.channel_, sec);
					auto *p = &sec;
					uintptr_t i = (uintptr_t) p;
					id_map[current_sec.id_] = i; //since Channel map is updated, the other two maps must update the struct address
					symbol_map[current_sec.symbol_] = i;
					return;
				}
			}
			if (current_sec.symbol_ != sec.symbol_
					and current_sec.id_ == sec.id_) {
				if (symbol_sec.IsValid()) {
					throw std::runtime_error("Security Object Already in Use!");
					return;
				} else {
					uintptr_t value = symbol_map[current_sec.symbol_]; //retriev address
					//defaulting to the O(n) solution, will fix if time permits
					remove_ChannelObject(current_sec.channel_, current_sec);
					insert_ChannelObject(sec.channel_, sec);
					auto *p = &sec;
					uintptr_t i = (uintptr_t) p;
					id_map[current_sec.id_] = i;
					symbol_map.erase(current_sec.symbol_);
					symbol_map[sec.symbol_] = i;
					return;
					/* This is my intial O(1) solution that didn't work cause mutablity issues, in hindsight wouldve done this a little differently if  created my own version of Vector class which would allow an O(1) run time for both GetSecurities and this situation
					 //UPDATE STRUCT VALUE IN MAP OF VECTORS
					 Security *x= (Security*) value;
					 x->symbol_=sec.symbol_;*/
				}
			}
			/*
			 • Update Security with new symbol

			 Before state:
			 Security: 1, "MSFT", 1
			 Security: 2, "TSLA", 1
			 Security: 3, "GOOG", 1

			 Mutations:
			 Save Security: 1, "AAPL", 1

			 After state:
			 Security: 1, "AAPL", 1
			 Security: 2, "TSLA", 1
			 Security: 3, "GOOG", 1*/

			//symbol change
			/*  if(current_sec.symbol_ != sec.symbol_){
			 Security x=GetSecurity(sec.symbol_);
			 if(x.channel_ ==0){//symbol not found, update symbol value

			 //remove Key, Same value
			 uintptr_t value=symbol_map[current_sec.symbol_]; //retriev address

			 //defaulting to the O(n) solution, will fix if time permits
			 remove_ChannelObject(current_sec.channel_,current_sec);
			 insert_ChannelObject(sec.channel_,sec);
			 auto *p=&sec;
			 uintptr_t i= (uintptr_t) p;
			 id_map[current_sec.id_]=i;
			 symbol_map.erase(current_sec.symbol_ );
			 symbol_map[sec.symbol_ ]= i;
			 return ;
			 /* My O(1) solution that didn't work cause mutablity issues, in hindsight wouldve done this a little differently if I had ID_map contain the structs (There would be a trade off for GetSecurities(since it's currently
			 a O(1 solution). Thus I would've created my own version of Vector class which would allow an O(1) run time for both GetSecurities and this situation
			 //UPDATE STRUCT VALUE IN MAP OF VECTORS
			 Security *x= (Security*) value;
			 x->symbol_=sec.symbol_;
			 print_ChannelMap();

			 //Throw std::runtime_error
			 }
			 else
			 cout<<"Throw";

			 }*/

		}

	}

	void PrintAll() {
		print_IdMap();
		print_Symbol_map();
		print_ChannelMap();
	}
	/* void testremove_ChannelObject(uint16_t key,const Security &sec){
	 remove_ChannelObject( key, sec);
	 }*/

private:
	map<uint16_t, Securities> channel_map; // key-CharID, Value Securties-Vector of structs Addresses. Assuming the case "If multiple entries per key is very common use map, vector case- otherwise would've used Mulyimap if opposite were true.
	map<uint32_t, uintptr_t> id_map; //key-Id Value-of ADDRESS struct(to save space)
	map<string, uintptr_t> symbol_map; //key-Id Value ADDRESS of struct(to save space)

	//double check
	void print_ChannelMap() {
		for (auto mapIt = begin(channel_map); mapIt != end(channel_map);
				++mapIt) {
			cout << " Key " << mapIt->first << " : " << endl;

			for (auto c : mapIt->second) {
				std::cout << c << " ";
			}

			std::cout << std::endl;
		}
	}
	void insert_ChannelObject(uint16_t key, const Security &sec) {
		auto Hash_it = channel_map.find(key);
		if (Hash_it == channel_map.end()) {
			channel_map.insert(pair<uint16_t, Securities>(key, Securities()));
			channel_map[key].push_back(sec);
		} else
			channel_map[key].push_back(sec);
	}
	void remove_ChannelObject(uint16_t key, const Security &sec) {
		auto Hash_it = channel_map.find(key);
		for (auto vec_it = (*Hash_it).second.begin();
				vec_it != (*Hash_it).second.end();) //change to auto, add a break
				{
			if ((*vec_it) == sec) {
				vec_it = Hash_it->second.erase(vec_it);
				return;
			} else
				++vec_it;
		}
	}

	void insert_IdObject(uint32_t key, const Security &sec) {
		auto *p = &sec;
		uintptr_t i = (uintptr_t) p;
		id_map[key] = i;
	}
	void print_IdMap() {
		for (auto it = begin(id_map); it != end(id_map); ++it) {
			cout << "ID Key" << it->first << "Value" << it->second << endl;
		}
	}

	void insert_SymbolObject(string key, const Security &sec) {
		auto *p = &sec;
		uintptr_t i = (uintptr_t) p;
		symbol_map[key] = i;
	}
	void print_Symbol_map() {
		for (auto it = begin(symbol_map); it != end(symbol_map); ++it) {
			cout << "Symbol Key " << it->first << "Value" << it->second << endl;
		}
	}

};

int main() {
	SecurityLookup lookup;

	Security s1 = { 1, "MSFT", 1 };
	Security s2 = { 2, "TSLA", 1 };
	Security s3 = { 3, "GOOG", 1 };
	Security s4 = { 1, "", 1 };
	//  Security s6={9,"obj3",20 } ;
	lookup.SaveSecurity(s1);
	lookup.SaveSecurity(s2);
	lookup.SaveSecurity(s3);
	lookup.SaveSecurity(s4);
	//  lookup.SaveSecurity(s5);
	//  lookup.SaveSecurity(s6);
	/*lookup.SaveSecurity(s7);
	 lookup.SaveSecurity(s8);
	 lookup.SaveSecurity(s9);*/
	lookup.PrintAll();
	/* cout<<"Testing Get";
	 cout<<lookup.GetSecurity(11)<<endl;
	 cout<<lookup.GetSecurity("obj5");
	 cout<<lookup.GetSecurity("obj1");
	 Securities x=lookup.GetSecurities(1212);
	 for (auto it=x.begin();it!=x.end();it++){
	 cout<<*it;
	 }*/

}





