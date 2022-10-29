#include "http_client.hh"

int sockfd;
struct sigaction act;

int main(int argc, char *argv[])
{
    int portno, test_duration, n;
    float think_time;

    struct timeval start_time, end_time;

    char buffer[256];

    struct hostent *server;
    struct sockaddr_in serv_addr;

    act.sa_handler = int_handler; // set signal handler for parent

    sigaction(SIGINT, &act, 0); // set interrupt signal handler for parent

    if (argc != 3)
    {
        cerr << "Usage " << argv[0] << " think_time test_duration" << endl;
        exit(EXIT_FAILURE);
    }

    think_time = atof(argv[1]);
    test_duration = atoi(argv[2]);

    cout << "Host: " << HOST << endl;
    cout << "Port: " << PORT << endl;
    cout << "Think Time: " << think_time << endl;
    cout << "Test Duration: " << test_duration << endl;

    // Get the server address
    portno = PORT;
    server = gethostbyname(HOST);
    if (server == NULL)
    {
        cerr << "Error, no such host" << endl;
        exit(EXIT_FAILURE);
    }

    // Ready the socket address for connecting
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    gettimeofday(&start_time, NULL);

    // Create a socket

    while (1)
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            cerr << "Error opening socket" << endl;
            exit(EXIT_FAILURE);
        }
        // Connect to the server
        if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        {
            cerr << "Error connecting" << endl;
            exit(EXIT_FAILURE);
        }

        // cout << "Please enter the url: ";
        bzero(buffer, 256);
        // cin >> buffer;
        // string url = buffer;
        string url = "/index.html";

        string request = send_request(url);
        n = write(sockfd, request.c_str(), strlen(request.c_str()));
        if (n < 0)
        {
            cerr << "Error writing to socket" << endl;
            exit(EXIT_FAILURE);
        }

        string response = "";

        while (1)
        {
            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);

            if (n < 0)
            {
                cerr << "Error reading from socket" << endl;
                exit(EXIT_FAILURE);
            }
            else if (n == 0)
            {
                cout << "Server closed connection" << endl;
                break;
            }

            response += buffer;
        }

        sleep(think_time);

        gettimeofday(&end_time, NULL);
        int elapsed_time = time_diff(&start_time, &end_time);

#if SANITY_CHECK
        cout << "Response: " << endl;
        cout << response << endl;
        cout << "Elapsed Time: " << elapsed_time << endl;
#endif
        if (elapsed_time >= test_duration)
        {
            break;
        }
        close(sockfd);
    }

    return 0;
}

void int_handler(int p)
{
    cout << "\nClosing the connection." << endl;
    close(sockfd);
    exit(EXIT_SUCCESS);
}

float time_diff(struct timeval *t1, struct timeval *t2)
{
    return (t2->tv_sec - t1->tv_sec) + (t2->tv_usec - t1->tv_usec) / 1e6;
}