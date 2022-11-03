#ifndef _HTTP_SERVER_HH_
#define _HTTP_SERVER_HH_

#include <iostream>
#include <sstream>
#include <string.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <string>
#include <sys/time.h>
#include <pthread.h>

// Configuration
#define HOST "localhost"
#define URL "/index.html"
#define PORT 8080
#define SANITY_CHECK 0
#define OUTPUT 0
#define FAULT_EXIT 0

using namespace std;

float time_diff(struct timeval *, struct timeval *);
void *user_routine(void *args);

struct HTTP_Request
{
    string HTTP_version;

    string method;
    string url;

    // TODO : Add more fields if and when needed

    string get_string(); // Return the string representation of the HTTP Request

    HTTP_Request(string url); // Constructor
};

struct user_info
{
    // User id
    int user_id;

    // socket info
    int portno;
    char *hostname;
    float think_time;

    // user metrics
    int total_count;
    float total_rtt;
};

string send_request(string url);

#endif