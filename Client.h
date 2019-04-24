/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.h
 * Author: cpu10855
 *
 * Created on April 24, 2019, 2:18 PM
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <string.h>
#include <iostream>

#include "gen-cpp/UserProfile.h"

class Client {
public:
	Client(int );
	virtual ~Client();
	
	// services
	void registerUser (std::string name, bool isMale, int birthDate);
	void getUserData(const int id);
	void updateUserData(const int id, std::string *name, bool *isMale, int * birthDate);
	void unregisterUser(const int id);
	
protected:
	static int counter;
	
private:
	int client_id; // for saving file, not match with _id
	apache::thrift::stdcxx::shared_ptr<apache::thrift::transport::TTransport> socket;
	apache::thrift::stdcxx::shared_ptr<apache::thrift::transport::TTransport> transport;
	apache::thrift::stdcxx::shared_ptr<apache::thrift::protocol::TProtocol> protocol;
};

#endif /* CLIENT_H */

