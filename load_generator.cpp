#include "http_client.hh"

FILE *log_file;
bool done;

int main(int argc, char *argv[])
{
    int test_duration, user_count;
    float think_time, throughput = 0, response_time = 0;

    if (argc != 4)
    {
        cerr << "Usage " << argv[0] << " user_count think_time test_duration" << endl;
        exit(EXIT_FAILURE);
    }

    user_count = atoi(argv[1]);
    think_time = atof(argv[2]);
    test_duration = atoi(argv[3]);

    cout << "Host: " << HOST << endl
         << "Port: " << PORT << endl
         << "User Count: " << user_count << endl
         << "Think Time: " << think_time << endl
         << "Test Duration: " << test_duration << endl
         << "Testing ..." << endl;

    // Opening log file
    log_file = fopen("load_gen.log", "w");

    fprintf(log_file, "Host: %s\n", HOST);
    fprintf(log_file, "Port: %d\n", PORT);
    fprintf(log_file, "User Count: %d\n", user_count);
    fprintf(log_file, "Think Time: %f\n", think_time);
    fprintf(log_file, "Test Duration: %d\n", test_duration);

    pthread_t threads[user_count];
    struct user_info *info = (user_info *)malloc(sizeof(struct user_info) * user_count);
    struct timeval start_time, end_time;

    // Starting timer for the load test
    gettimeofday(&start_time, NULL);
    done = false;

    for (int i = 0; i < user_count; i++)
    {
        // Initializing user info
        info[i].user_id = i;
        info[i].hostname = HOST;
        info[i].portno = PORT;
        info[i].think_time = think_time;
        info[i].total_rtt = 0;
        // Creating threads
        if (pthread_create(&threads[i], NULL, &user_routine, &info[i]) != 0)
        {
            cerr << "Error: Could not create thread " << i << endl;
            exit(EXIT_FAILURE);
        }
        fprintf(log_file, "Created Thread %d\n", i);
    }

    // Waiting for test duration
    sleep(test_duration);

    fprintf(log_file, "Main Thread Woke up!!\n");

    gettimeofday(&end_time, NULL);
    done = true;

    // Waiting for all threads to finish
    for (int i = 0; i < user_count; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Calculating average throughput and response time
    for (int i = 0; i < user_count; i++)
    {
        throughput += info[i].total_count;
        response_time += (info[i].total_rtt / info[i].total_count);
    }
    response_time /= user_count;
    throughput /= test_duration;

    fprintf(log_file, "\n\nAverage Throughput: %f\n", throughput);
    fprintf(log_file, "Average Response Time: %f\n", response_time * 1000);

    cout << "\n\nAverage Throughput: " << throughput << endl
         << "Average Response Time: " << response_time << endl;

    // Closing log file
    fclose(log_file);

    return 0;
}

void *user_routine(void *args)
{
    struct user_info *info = (struct user_info *)args;

    int sockfd, portno, n;
    char buffer[256];
    struct timeval start, end;

    struct hostent *server;
    struct sockaddr_in serv_addr;

    // Get the server address
    portno = info->portno;
    server = gethostbyname(info->hostname);
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

    while (1)
    {
        gettimeofday(&start, NULL);
        // Create a socket
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
        // bzero(buffer, 256);
        // cin >> buffer;
        // string url = buffer;
        string url = URL;

        string request = send_request(url);
        n = write(sockfd, request.c_str(), strlen(request.c_str()));
        if (n < 0)
        {
#if FAULT_EXIT
            cerr << "Error writing to socket" << endl;
            exit(EXIT_FAILURE);
#endif
        }

        string response = "";
        while (1)
        {
            bzero(buffer, 256);
            n = read(sockfd, buffer, 255);

            if (n < 0)
            {
#if FAULT_EXIT
                cerr << "Error reading from socket" << endl;
                exit(EXIT_FAILURE);
#endif
            }
            else if (n == 0)
            {
#if SANITY_CHECK
                cout << "Server closed connection" << endl;
#endif
                break;
            }

            response += buffer;
        }
#if OUTPUT
        cout << "Response: " << response << endl;
#endif
        close(sockfd);

        gettimeofday(&end, NULL);
        float elapsed_time = time_diff(&start, &end);

        if (done)
        {
            break;
        }

        // Updating user metrics
        info->total_rtt += elapsed_time;
        info->total_count++;

        sleep(info->think_time);
    }
    fprintf(log_file, "Thread #%d finished\n", info->user_id);
    fflush(log_file);
    pthread_exit(NULL);
}

float time_diff(struct timeval *t1, struct timeval *t2)
{
    return (t2->tv_sec - t1->tv_sec) + (t2->tv_usec - t1->tv_usec) / 1e6;
}