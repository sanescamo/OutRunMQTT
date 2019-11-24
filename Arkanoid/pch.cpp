// pch.cpp: source file corresponding to pre-compiled header; necessary for compilation to succeed

#include "pch.h"

#include <iostream>

using namespace std;

pch::pch(const char* _id, const char* _topic, const char* _host, int _port) : mosquittopp(_id)
{
	mosqpp::lib_init();        // Mandatory initialization for mosquitto library
	this->keepalive = 120;    // Basic configuration setup for myMosq class
	this->id = _id;
	this->port = _port;
	this->host = _host;
	this->topic = _topic;
	connect_async(host,     // non blocking connection to broker request
		port,
		keepalive);
	subscribe(NULL, this->topic, 2);
	loop_start();            // Start thread managing connection / publish / subscribe
};


pch::~pch() {
	loop_stop();            // Kill the thread
	mosqpp::lib_cleanup();    // Mosquitto library cleanup
}

void pch::on_disconnect(int rc) {
	std::cout << ">> mySubs - disconnection(" << rc << ")" << std::endl;
}


void pch::on_connect(int rc)
{
	if (rc == 0) {
		std::cout << ">> mySubs - connected with server" << std::endl;
	}
	else {
		std::cout << ">> mySubs - Impossible to connect with server(" << rc << ")" << std::endl;
	}
}

void pch::on_subscribe(int mid, int qos_count, const int* granted_qos)
{
	std::cout << ">> subscription succeeded (" << mid << ") " << std::endl;
}

void pch::on_message(const mosquitto_message* message)
{
	cout << "Subscriber " << id << " received message of topic: " << message->topic << " Data: " << reinterpret_cast<char*>(message->payload) << "\n";
}
