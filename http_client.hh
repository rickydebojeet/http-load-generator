#ifndef _HTTP_SERVER_HH_
#define _HTTP_SERVER_HH_

#include <iostream>
#include <sstream>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <string>
#include <vector>
#include <sys/time.h>

// Configuration
#define HOST "localhost"
#define PORT 8080
#define SANITY_CHECK 1

using namespace std;

void int_handler(int);

float time_diff(struct timeval *, struct timeval *);

struct HTTP_Request
{
    string HTTP_version;

    string method;
    string url;

    // TODO : Add more fields if and when needed

    string get_string(); // Return the string representation of the HTTP Request

    HTTP_Request(string url); // Constructor
};

string send_request(string url);

#endif