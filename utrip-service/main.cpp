#include"UtripService.hpp"
#include"Handlers.hpp"
#include"../server/server.hpp"
#include<iostream>
using namespace std;
#define HOTELS_FILE_PATH_INDEX 1
#define RATINGS_FILE_PATH_INDEX 2
#define SERVER_PORT 8080
#define SIGNIN_PAGE_PATH "template/signin.html"
#define SIGNUP_PAGE_PATH "template/signup.html"
#define SIGNIN_PATH "/"
#define SIGNUP_PATH "/signup"
#define SIGNOUT_PATH "/signout"
#define HOME_PATH "/home"
#define FILTER_PATH "/filter"
#define WALLET_PATH "/wallet"
#define DETAILS_PATH "/details"
int main(int argc,char* argv[]){
    Server server(SERVER_PORT);
    Utrip utrip;
    ErrorHandler error_handler;
    utrip.save_hotels_informations(argv[HOTELS_FILE_PATH_INDEX]);
    try{
        server.get(SIGNIN_PATH,new SigninPageHandler(SIGNIN_PAGE_PATH,&error_handler));
        server.post(SIGNIN_PATH,new SigninHandler(&utrip,&error_handler));
        server.get(SIGNUP_PATH,new SignupPageHandler(SIGNUP_PAGE_PATH,&error_handler));
        server.post(SIGNUP_PATH,new SignupHandler(&utrip,&error_handler));
        server.get(SIGNOUT_PATH,new SignoutHandler(&utrip,&error_handler));
        server.get(HOME_PATH,new HomeHandler(&utrip,&error_handler));
        server.get(FILTER_PATH,new FilterHandler(&utrip,&error_handler));
        server.get(DETAILS_PATH,new DetailsHandler(&utrip,&error_handler));
        server.get(WALLET_PATH,new WalletPageHandler(&utrip,&error_handler));
        server.post(WALLET_PATH,new WalletHandler(&utrip,&error_handler));
        server.run();
    }
    catch(Server::Exception& exception){
        cout<<exception.getMessage()<<endl;
    }
    return 0;
}