#include "http_client.hh"

HTTP_Request::HTTP_Request(string url)
{
    this->url = url;
    this->HTTP_version = "1.0"; // 1.0 for this assignment
    this->method = "GET";       // GET for this assignment
}

string HTTP_Request::get_string()
{
    string request = "";
    request += this->method + " ";
    request += this->url + " ";
    request += "HTTP/" + this->HTTP_version + "\r\n";
    request += "\r\n";
    return request;
}

string send_request(string url)
{
    HTTP_Request *request = new HTTP_Request(url);
    string request_string = request->get_string();

    delete request;

    return request_string;
}