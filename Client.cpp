/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.cpp
 * Author: cpu10855
 * 
 * Created on April 24, 2019, 2:18 PM
 */

#include "Client.h"

using namespace apache::thrift;
using namespace apache::thrift::transport;
using namespace apache::thrift::protocol;

using namespace std;

int Client::counter = 0;

Client::Client(int port) {
	stdcxx::shared_ptr<TTransport> socket(new TSocket("localhost", port));
	stdcxx::shared_ptr<TTransport> transport(new TBufferedTransport(socket)); 
	stdcxx::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport)); //input protocol
	
	this->socket = socket;
	this->transport = transport;
	this->protocol = protocol;
	this->client_id = Client::counter++;
}

Client::~Client() {
}

void Client::registerUser(std::string name, bool isMale, int birthDate){
	try {
		cout << "Making request add new client from client: " << this->client_id << endl;
		InputProfileData newdata;
		newdata.__set_name(name);
		newdata.__set_isMale(isMale);
		newdata.birthDate.__set_dateInt(birthDate);
		
		this->transport->open();
		
		UserProfileClient client(this->protocol);
		int ret = client.CreateUser(newdata);
		
		this->transport->close();
		
		if (ret == -1){ // define as error when register - TODO change to ErrorCode
			cout << "Sufficient required data field(s)." << endl;
			cout << "Name: " << newdata.__isset.name << "Gender: " << newdata.__isset.isMale << "Day of birth: " << newdata.__isset.birthDate << endl;
			return ;
		}
		
		cout << "Register successful user. User's id: " << ret << endl;
		return;
	}
	catch (TException& tx){
		cout << "ERROR: " << tx.what() << endl;
	}
}

void Client::getUserData(const int id){
	try {
		cout << "Making retrieve request get user profile from client: " << this->client_id << endl;
		GetProfileResult ret;
		this->transport->open();
		
		UserProfileClient client(this->protocol);
		
		client.GetUser(ret, id);
		
		this->transport->close();
		
		if (ret.code == ErrorCode::type::SUCCESS){
			cout << "=============================" << endl;
			cout << "|| Name: " << ret.data.name << endl;
			cout << "|| Gender: " << (ret.data.isMale ? "Male" : "Female") << endl;
			cout << "|| Day of birth: " << ret.data.birthDate << endl;
			cout << "|| Id: " << ret.data._id << endl;
			cout << "|| Register Date: " << ret.data.registerTime << endl;
			cout << "=============================" << endl;
		}
		else {
			cout << "ERROR: " << ret.code << endl;
		}
	}
	catch (TException& tx) {
		cout << "ERROR " << tx.what() << endl;
	}
}

void Client::updateUserData(const int id, std::string* name, bool* isMale, int* birthDate){
	try {
		cout << "Making update request modify user profile from client: " << this->client_id << endl;
		InputProfileData update;
		
		// only update non-null value
		if (name != NULL)
			update.__set_name(*name);
		if (isMale != NULL)
			update.__set_isMale(*isMale);
		if (birthDate != NULL)
			update.birthDate.__set_dateInt(*birthDate);

		UserProfileClient client(this->protocol);
		this->transport->open();
		
		ErrorCode::type code = client.UpdateUser(id, update);
		
		this->transport->close();
		
		if (code == ErrorCode::type::SUCCESS){
			cout << "Successfully update new information." << endl;
		}
		else {
			cout << "ERROR: " << code << endl;
		}
	}
	catch(TException& tx){
		cout << "ERROR - " << tx.what() << endl; 
	}
}

void Client::unregisterUser(const int id){
	try {
		cout << "Making Unregister request delete user profile from client: " << this->client_id << endl;
		UserProfileClient client(this->protocol);
		
		this->transport->open();
		
		ErrorCode::type code = client.RemoveUser(id);
		
		this->transport->close();
		
		if (code == ErrorCode::type::SUCCESS){
			cout << "Successfully delete user information." << endl;
		}
		else {
			cout << "ERROR: " << code << endl;
		}
	}
	catch(TException& tx){
		cout << "ERROR - " << tx.what() << endl; 
	}
}