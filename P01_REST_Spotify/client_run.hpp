#pragma once

#include "helper_functions.hpp"

class client_commands{
    public:
        void artist (std::string artist_name,std::string token);
        void song(std::string song_name, std::string token);
};

void client_commands::artist(std::string artist_name, std::string token) {
    CURL* curl = curl_easy_init();
    if (!curl) return;

    artist_name = get_href_string(artist_name);
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
    
   //std::cout << "\nResponse:\n" << response << std::endl;

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


void client_commands::song(std::string song_name, std::string token){
    CURL* curl = curl_easy_init();
    if (!curl) return;

    song_name = get_href_string(song_name);
    std::string response;

    struct curl_slist* headers = NULL;
    std::string auth_header = "Authorization: Bearer " + token;
    headers = curl_slist_append(headers, auth_header.c_str());

    std::string query ="https://api.spotify.com/v1/search?q=" + song_name + "&type=track&limit=1";

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
    
    //std::cout << "\nResponse:\n" << response << std::endl;

    // response into json object
    json data = json::parse(response);

    auto items = data["tracks"]["items"];

    if (items.is_array() && !items.empty()) {
        std::cout << "Song name      : " << get_string(items[0]["name"]) << "\n";

        std::cout << "Artist name    : " << get_string(items[0]["artists"][0]["name"]) << "\n";

        std::cout << "Song Album     : " << get_string(items[0]["album"]["name"]) << "\n";

        std::cout << "Album Artist   : " << get_string(items[0]["album"]["artists"][0]["name"]) << "\n";

        std::cout << "Release Date   : " << get_string(items[0]["album"]["release_date"]) << "\n";

        std::cout << "Duration       : " << (int)items[0]["duration_ms"] / 1000 << "s\n";
    }

    else{
        std::cerr<<"internal error occured"<<std::endl;
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
}

void print_menu(){
    std::cout << "\n===== MENU =====\n";
    std::cout << "song [song_name]         - get song metadata\n";
    std::cout << "artist [artist_name]     - get artist data\n";
    std::cout << "see_token                - view current access token\n";
    std::cout << "creds                    - view client ID and secret\n";
    std::cout << "help                     - show this menu\n";
    std::cout << "exit                     - terminate program\n";
    std::cout << "================\n\n";
}

void run_client(){

    std::string input_command;
    client_commands cmd;

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

    std::string token = get_client_token(auth_string);
    token = get_string(token);
    //std::cout<<token<<std::endl;
    print_menu();
    while(true){
        std::getline(std::cin,input_command);

        if(input_command=="exit"){
            return ;
        }

        std::istringstream  command_name (input_command);
        std::string first_word;
        command_name>>first_word;
        std::transform(first_word.begin(), first_word.end(), first_word.begin(), [](char c){return std::tolower(c);});

        if(first_word != "artist" && first_word!= "song" && first_word!= "see_token" && first_word!= "creds" && first_word!="help"){
            std::cout<<std::endl;
            std::cerr<<"invalid command \n";
            std::cout<<std::endl;
        }

        if(first_word=="artist"){
            std::cout<<std::endl;
            std::string artist_name = input_command.substr(first_word.size());
            trim(artist_name);
            cmd.artist(artist_name, token);
            std::cout<<std::endl;
        }

        if(first_word=="song"){
            std::cout<<std::endl;
            std::string song_name = input_command.substr(first_word.size());
            trim(song_name);
            cmd.song(song_name, token);
            std::cout<<std::endl;
        }

        if(first_word=="see_token"){
            std::cout<<std::endl;
            std::cout<<"Current token : "<<token<<std::endl;
            std::cout<<std::endl;
        }

        if(first_word=="creds"){
            std::cout<<std::endl;
            std::cout<<"Client ID     : "<<id<<std::endl;
            std::cout<<"Client Secret : "<<secret<<std::endl;
            std::cout<<std::endl;
        }

        if(first_word=="help"){
            print_menu();
        }
       
    }
}
