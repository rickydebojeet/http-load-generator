#include "http_client.hh"

int sockfd;
struct sigaction act;

int main(int argc, char *argv[])
{
    int portno, n;

    char buffer[256];

    struct hostent *server;
    struct sockaddr_in serv_addr;

    act.sa_handler = int_handler; // set signal handler for parent

    sigaction(SIGINT, &act, 0); // set interrupt signal handler for parent

    if (argc < 3)
    {
        cerr << "Usage " << argv[0] << " hostname port" << endl;
        exit(EXIT_FAILURE);
    }

    // Create a socket
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "Error opening socket" << endl;
        exit(EXIT_FAILURE);
    }

    // Get the server address
    server = gethostbyname(argv[1]);
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

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        cerr << "Error connecting" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "Please enter the url: ";
    bzero(buffer, 256);
    cin >> buffer;
    string url = buffer;
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

#if SANITY_CHECK
    cout << response << endl;
#endif

    close(sockfd);

    return 0;
}

void int_handler(int p)
{
    cout << "\nClosing the connection." << endl;
    close(sockfd);
    exit(EXIT_SUCCESS);
}