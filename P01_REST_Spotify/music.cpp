#include "helper_functions.hpp"
#include "client_run.hpp"

int main(int argc, char* argv[]){
    curl_global_init(CURL_GLOBAL_DEFAULT);
    
    run_client();
    
    return 0;
}