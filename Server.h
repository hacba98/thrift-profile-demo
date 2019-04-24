/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: cpu10855
 *
 * Created on April 23, 2019, 11:38 AM
 */

#ifndef SERVER_H
#define SERVER_H

#include <thrift/server/TThreadedServer.h> // using TThreadServer - support multi-threads
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include "gen-cpp/UserProfile.h" // contain data structure auto-generated

//#include <time.h>
#include <chrono> // for system clock
#include <iostream>

class Server {
public:
//	Server();
//	Server(const Server& orig);
//	Server( const boost::shared_ptr<apache::thrift::TProcessorFactory>& processorFactory,
//		const boost::shared_ptr<apache::thrift::transport::TServerTransport>& serverTransport,
//		const boost::shared_ptr<apache::thrift::transport::TTransportFactory>& transportFactory,
//		const boost::shared_ptr<apache::thrift::protocol::TProtocolFactory>& protocolFactory,
//		const boost::shared_ptr<apache::thrift::concurrency::ThreadFactory>& threadFactory);
	Server(int);
	virtual ~Server();
	
	void serve();
private:
	apache::thrift::server::TSimpleServer *core;
};

#endif /* SERVER_H */

