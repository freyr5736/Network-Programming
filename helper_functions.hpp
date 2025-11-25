#pragma once

#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "base64.hpp"
#include <curl/curl.h>

using json = nlohmann::json;


// Extract DATA from .config
std::string read_config() {
    std::ifstream file(".config");
    std::string content((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
    return content;
}


// To get the properly formatted string without new line and flush characters
std::string get_string(std::string s) {
    s.erase(std::remove(s.begin(), s.end(), '\r'), s.end());
    s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());

    auto pos = s.find(':');
    pos++;
    if (pos == std::string::npos) return "";
    
    std::string res = "";
    int count = 0;
    while(pos<s.size() && count<2){
       if(s[pos]=='"'){
            count++;
            pos++;
            continue;
       }
        res+=s[pos];
        pos++;
    }

    while (!res.empty() && isspace(res[0])) res.erase(0, 1);

    return res;
}


std::string get_href_string(std::string s){
    std::string res = "";

    for(char c : s){
        if(c==' '){
            res+="%20";
        }
        else{
            res+=c;
        }
    }
    return res;
}


void trim(std::string& s) {
    s.erase(0, s.find_first_not_of(" \t\n"));          // remove front
    s.erase(s.find_last_not_of(" \t\n") + 1);          // remove back
}


/*
void* content -> Pointer to the data buffer containing the chunk of data libcurl just downloaded.
size_t size   -> The size of each element in the buffer (almost always 1).
size_t nmemb  -> The number of elements in the buffer.
void* userp   -> User-provided pointer for storing data
*/
size_t write_call_back(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t total = size * nmemb;
    std::string* s = reinterpret_cast<std::string*>(userp); // stores the downloaded data

    try {
        s->append((char*)contents, total);
    } 
    
    catch (...) {
        return 0;  // tells curl "write failed"
    }

    return total;
}


std::string get_user_id(std::string token){
    CURL* curl = curl_easy_init();
    if(!curl) return "getting user id failed";

    struct curl_slist* headers = NULL;
    std::string auth_header = "Authorization: Bearer " + token;
    std::string response;

    curl_easy_setopt(curl, CURLOPT_URL, "https://api.spotify.com/v1/me");
    curl_easy_setopt(curl, CURLOPT_HTTPGET,1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_call_back);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    return response;
}


std::string get_client_token(const std::string& auth_string)
{
    std::string auth_base64 = base64::to_base64(auth_string);

    CURL* curl = curl_easy_init();
    if (!curl) return "getting token failed";

    std::string response;
    const std::string post_fields = "grant_type=client_credentials";

    /*
    creating this :
    headers: {
        'content-type': 'application/x-www-form-urlencoded',
        'Authorization': 'Basic ' + (new Buffer.from(client_id + ':' + client_secret).toString('base64'))
      },
    */
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/x-www-form-urlencoded");
    std::string auth_header = "Authorization: Basic " + auth_base64;
    headers = curl_slist_append(headers, auth_header.c_str());

    // POST request
    curl_easy_setopt(curl, CURLOPT_URL, "https://accounts.spotify.com/api/token");
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_call_back);   // Required in order to store the data locally that is received from the server
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "curl error: " << curl_easy_strerror(res) << std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return response;
}

