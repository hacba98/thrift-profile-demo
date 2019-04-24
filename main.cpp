/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   m.cpp
 * Author: cpu10855
 *
 * Created on April 23, 2019, 10:44 AM
 */

#include <cstdlib>
#include <stdio.h>
#include <unistd.h>

#include "Server.h"
#include "Client.h"

using namespace std;

/*
 * 
 */
int main(int argc, char** argv) {
	int n = fork();
	
	if (n == 0) { // for server process
		Server server(8000);
		cout << "Server is running!" << endl;
		server.serve();
	}
	else if (n > 0){
		Client client(8000);
		cout << "Client is running!" << endl;
		client.registerUser("Demo", false, 1564879);
		client.getUserData(0);
		string newname = "Demo2";
		int newDate = 95632412;
		client.updateUserData(0, &newname, NULL, &newDate);
		client.getUserData(0);
		client.unregisterUser(0);
		client.getUserData(0);
	}
	
	
	cout << "Demo" << endl;
	return 0;
}

