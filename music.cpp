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


class commands{
    public:
        void artist (std::string artist_name,std::string token);
        void song(std::string song_name, std::string token);
        void top(int number, std::string token);
};

void commands::artist(std::string artist_name, std::string token) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    std::string response;

    struct curl_slist* headers = NULL;
    std::string auth_header = "Authorization: Bearer " + token;
    headers = curl_slist_append(headers, auth_header.c_str());

    std::string query ="https://api.spotify.com/v1/search?q=" + artist_name + "&type=artist&limit=1";

    curl_easy_setopt(curl, CURLOPT_URL, query.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L); // creates GET
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_call_back);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Perform the request
    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        std::cerr << "curl failed: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        return;
    } 
    
    //std::cout << "Response:\n" << response << std::endl;

    // response into json object
    json data = json::parse(response);

    auto items = data["artists"]["items"];

    if (items.is_array() && !items.empty()) {
        std::cout << "Artist name      : " << get_string(items[0]["name"]) << "\n";
        std::cout << "Artist id        : " << get_string(items[0]["id"]) << "\n";
        for(int i=0;i<items[0]["genres"].size();++i){
            std::cout << "Artist genres    : " << get_string(items[0]["genres"][i]) << " ";
        }
        std::cout << "\nArtist followers : " << items[0]["followers"]["total"] << "\n";
    }
    else{
        std::cerr<<"internal error occured"<<std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}


void commands :: song(std::string song_name, std::string token){
    
}

void top (int number, std::string token){
    
}








// To get TOKEN 
std::string get_token(const std::string& auth_string)
{
    std::string auth_base64 = base64::to_base64(auth_string);

    CURL* curl = curl_easy_init();
    if (!curl) return "";

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


int main(int argc, char* argv[]){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    std::string input_command;
    commands cmd;

    std::string credentials = read_config();
    std:: istringstream creds (credentials);
    
    std::string id;
    std::string secret;
    std::string auth_string;

    getline(creds,id);
    getline(creds,secret);
    id = get_string(id);
    secret = get_string(secret);
    auth_string = id + ":" + secret;

    std::string token = get_token(auth_string);
    token = get_string(token);

    while(true){
        std::getline(std::cin,input_command);

        if(input_command=="exit"){
            return 0;
        }

        std::istringstream  command_name (input_command);
        std::string first_word;
        command_name>>first_word;
        std::transform(first_word.begin(), first_word.end(), first_word.begin(), [](char c){return std::tolower(c);});

        if(first_word != "artist" && first_word != "top" && first_word!= "song" ){
            std::cerr<<"invalid command \n";
        }

        if(first_word=="artist"){
            std::string artist_name = input_command.substr(first_word.size());
            trim(artist_name);
            std::cout<<artist_name<<std::endl;
            cmd.artist(artist_name, token);
        }
       
    }
    return 0;
}