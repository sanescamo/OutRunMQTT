// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

#ifndef PCH_H
#define PCH_H

#include "mosquittopp.h"
#include "mosquitto.h"

class pch : public mosqpp::mosquittopp
{
private:
	const char* host;
	const char* id;
	const char* topic;
	int                port;
	int                keepalive;

	void on_connect(int rc);
	void on_disconnect(int rc);
	void on_subscribe(int mid, int qos_count, const int* granted_qos);
	void on_message(const mosquitto_message* message);

public:
	pch(const char* id, const char* _topic, const char* host, int port);
	~pch();
};

#endif //PCH_H
