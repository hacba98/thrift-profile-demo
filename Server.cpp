/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.cpp
 * Author: cpu10855
 * 
 * Created on April 23, 2019, 11:38 AM
 */

#include "Server.h"

using namespace apache::thrift;
using namespace apache::thrift::server;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

using namespace std;

class ServicesHandler: virtual public UserProfileIf {
public:
	ServicesHandler(): counter_id(0), storage_data(){
		// code for initialize handler
		// TODO - log some information
	}
	
	// Implement interface
	/**
	 * Service for register new user information.
	 * 
	 * @param profile Input profile data 
	 * @return new user's id in system
	 */
	int32_t CreateUser(const InputProfileData &profile){
		// Check if required data: name, gender, day of birth is provided
		if (profile.__isset.name == false or profile.__isset.isMale == false or profile.__isset.birthDate == false){
			cout << "Name provided: " << profile.__isset.name << "; Gender provided: " << profile.__isset.isMale << "; Day of birth provided: " << profile.__isset.birthDate << endl;
			return -1; // TODO - Return error code
		}
		
		Profile newProfile;
		// generate _id
		int newUserId = this->counter_id++;
		newProfile._id = newUserId;
		//newProfile.lastActiveTime = time(NULL); // get
		newProfile.name = profile.name;
		newProfile.isMale = profile.isMale;
		
		// convert time from string (if have) to int
		if (profile.birthDate.__isset.dateString){
			// TODO - find a way to convert - Should it be converted in server-side
		}
		else {
			newProfile.birthDate = profile.birthDate.dateInt;
		}
		
		// register time
		auto sys_time = std::chrono::system_clock::now();
		newProfile.registerTime = static_cast<int32_t>(std::chrono::system_clock::to_time_t(sys_time));
		
		// store to DB
		this->storage_data[newProfile._id] = newProfile;
		
		// return back user id;
		return newUserId;
	}
	
	/**
	 * Services for client to retrieve user's data from server
	 * 
	 * @param _return	pass-by-ref return data
	 * @param _id	user's id in system
	 */
	void GetUser(GetProfileResult& _return, const int32_t _id){	
		// find user in storage_data
		auto search = this->storage_data.find(_id);
		if (search == this->storage_data.end()){ // in case of not found user
			_return.__isset.data = false;
			_return.__set_code(ErrorCode::type::USER_NOT_FOUND);
			return;
		}
		else { // case that found result
			_return.__isset.data = true;
			_return.__set_data(search->second);
			_return.__set_code(ErrorCode::type::SUCCESS);
			return;
		}
	}
	
	/**
	 * Service for update information
	 * 
	 * @param _id
	 * @param updateData
	 * @return ErrorCode::type structure
	 */
	ErrorCode::type UpdateUser(const int32_t _id, const InputProfileData& updateData) {
		// find user in storage data
		auto search = this->storage_data.find(_id);
		if (search == this->storage_data.end()){ // in case of not found user
			return ErrorCode::type::USER_NOT_FOUND;
		}
		else { // case that found result
			// update only field that .__isset == true
			if (updateData.__isset.name == true)
				search->second.__set_name(updateData.name);
			if (updateData.__isset.isMale == true)
				search->second.__set_isMale(updateData.isMale);
			if (updateData.__isset.birthDate == true){
				if (updateData.birthDate.__isset.dateInt)
					search->second.__set_birthDate(updateData.birthDate.dateInt);
				else {
					// find a way to convert
				}
			}
				
			return ErrorCode::type::SUCCESS;
		}
	}
	
	/**
	 * Service for remove user from system
	 * 
	 * @param _id	
	 * @return ErrorCode::type structure
	 */
	ErrorCode::type RemoveUser(const int32_t _id){
		// find user in storage data
		auto search = this->storage_data.find(_id);
		if (search == this->storage_data.end()){ // in case of not found user
			return ErrorCode::type::USER_NOT_FOUND;
		}
		else {
			// remove in data
			auto ret = this->storage_data.erase(search);
			std::cout << "User unregister: " << ret->first << endl;
			return ErrorCode::type::SUCCESS;
		}
	}
	
	
private:
	std::map<int32_t, Profile> storage_data;
	int counter_id = 0;
};

//Server::Server() {
//}
//
//Server::Server(const Server& orig) {
//}

Server::~Server() {
}

//Server::Server( const boost::shared_ptr<apache::thrift::TProcessorFactory>& processorFactory,
//		const boost::shared_ptr<apache::thrift::transport::TServerTransport>& serverTransport,
//		const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& transportFactory,
//		const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& protocolFactory,
//		const boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>& threadFactory)
//	: core(processorFactory, serverTransport, transportFactory, protocolFactory){	
//}

Server::Server(int port){
	std::shared_ptr<ServicesHandler> handler (new ServicesHandler());
	std::shared_ptr<TProcessor> processor (new UserProfileProcessor(handler));
	std::shared_ptr<apache::thrift::server::TServerTransport> serverTransport (new TServerSocket(port));
	std::shared_ptr<apache::thrift::transport::TTransportFactory> transportFactory (new TTransportFactory());
	std::shared_ptr<apache::thrift::protocol::TProtocolFactory> protocolFactory (new TBinaryProtocolFactory());
	
	this->core = new TSimpleServer(processor, serverTransport, transportFactory, protocolFactory);
}

void Server::serve(){
	cout << "Server is running!";
	this->core->serve();
}

