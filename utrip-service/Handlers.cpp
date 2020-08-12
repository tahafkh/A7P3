#include"Handlers.hpp"
#include"UtripService.hpp"
#include"../utrip-details/StarFilter.hpp"
#include<string>
#include<sstream>
using namespace std;
#define USERNAME "username"
#define PASSWORD "password"
#define REPASSWORD "repassword"
#define EMAIL "email"
#define HOME_PATH "/home"
#define SIGNIN_PATH "/"
#define SIGNUP_PATH "/signup"
#define WALLET_PATH "/wallet"
#define ERROR "error"
#define PASSWORD_ERROR "password_error"
#define SIGNIN_ERROR "signin_error"
#define ACTIVE "active"
#define DEACTIVE "deactive"
#define MIN_STAR "min_star"
#define MAX_STAR "max_star"
#define ID "id"
#define VALUE "value"
#define SIGNOUT_SESSION_ID "none"
SigninHandler::SigninHandler(Utrip* utrip_,ErrorHandler* handler){
    errorhandler = handler;
    utrip = utrip_;
}
Response* SigninHandler::callback(Request* request){
    Response* response = Response::redirect(HOME_PATH);
    string username = request->getBodyParam(USERNAME),password = request->getBodyParam(PASSWORD);
    try{
        response->setSessionId(to_string(utrip->signin(username,password)));
    }
    catch(exception& except){
        errorhandler->activate_signin_error();
        response = Response::redirect(SIGNIN_PATH);
    }
    return response;
}
SignupHandler::SignupHandler(Utrip* utrip_,ErrorHandler* handler){
    errorhandler = handler;
    utrip = utrip_;
}
Response* SignupHandler::callback(Request* request){
    Response* response = Response::redirect(HOME_PATH);
    string username = request->getBodyParam(USERNAME),email = request->getBodyParam(EMAIL),
    password = request->getBodyParam(PASSWORD),repassword = request->getBodyParam(REPASSWORD);
    if(password != repassword){
        response = Response::redirect(SIGNUP_PATH);
        errorhandler->activate_password_error();
    }
    else{
        try{
            response->setSessionId(to_string(utrip->signup(email,username,password)));
        }
        catch(exception& except){
            errorhandler->activate_signup_error();
            response = Response::redirect(SIGNUP_PATH);
        }
    }
    return response;
}
void ErrorHandler::deactivate_errors(){
    has_activatd_wallet_error = false;
    has_activated_password_error = false;
    has_activated_signin_error = false;
    has_activated_signup_error = false;
    hasnot_signedin_error = false;
}
SigninPageHandler::SigninPageHandler(string filepath,ErrorHandler* handler) : TemplateHandler(filepath){
    errorhandler = handler;
}
map<string,string> SigninPageHandler::handle(Request* request){
    map<string,string> context;
    if(errorhandler->has_signin_error()){
        context[ERROR] = ACTIVE;
    }
    else{
        context[ERROR] = DEACTIVE;
    }
    if(errorhandler->hasnot_signed_in()){
        context[SIGNIN_ERROR] = ACTIVE;
    }
    else{
        context[SIGNIN_ERROR] = DEACTIVE;
    }
    errorhandler->deactivate_errors();
    return context;
}
SignupPageHandler::SignupPageHandler(string filepath,ErrorHandler* handler) : TemplateHandler(filepath){
    errorhandler = handler;
}
map<string,string> SignupPageHandler::handle(Request* request){
    map<string,string> context;
    if(errorhandler->has_password_error()){
        context[PASSWORD_ERROR] = ACTIVE;
    }
    else{
        context[PASSWORD_ERROR] = DEACTIVE;
    }
    if(errorhandler->has_signup_error()){
        context[ERROR] = ACTIVE;
    }
    else{
        context[ERROR] = DEACTIVE;
    }
    errorhandler->deactivate_errors();
    return context;
}
SignoutHandler::SignoutHandler(Utrip* utrip_,ErrorHandler* handler){
    utrip = utrip_;
    errorhandler = handler;
}
Response* SignoutHandler::callback(Request* request){
    auto response = Response::redirect(SIGNIN_PATH);
    try{
        utrip->login(request->getSessionId());
        utrip->signout(request->getSessionId());
        response->setSessionId(SIGNOUT_SESSION_ID);
    }
    catch(exception& except){
        errorhandler->activate_not_signed_in_error();
    }
    return response;
}
HomeHandler::HomeHandler(Utrip* utrip_,ErrorHandler* handler){
    utrip = utrip_;
    errorhandler = handler;
}
Response* HomeHandler::callback(Request* request){
    try{
        utrip->login(request->getSessionId());
        Response* response = new Response;
        ostringstream body;
        response->setHeader("Content-Type","text/html");
        body <<
            "<!DOCTYPE html>"<<endl<<
            "<html>"<< endl<<
            "<head>"<<endl<<
            "<meta name='viewport' content='width=device-width, initial-scale=1'>"<<endl<<
            "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>"<<endl<<
            "<style>"<<endl<<
            "* {box-sizing: border-box;}"<<endl<<
            ".checked {color: orange;}"<<endl<<
            "ul { list-style-type: none; margin: 0; padding: 0; overflow: hidden; background-color: #333;}"<<endl<<
            "body { background-color: white; padding: 20px; font-family: Arial;}"<<endl<<
            "li { float: left;}"<<endl<<
            "li a { display: block; color: white; text-align: center; padding: 14px 16px; text-decoration: none;}"<<endl<<
            "li a:hover:not(.active) { background-color: #111;}"<<endl<<
            "input[type=text] { width: 10%; padding: 12px 20px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; box-sizing: border-box; }"<<endl<<
            "button { background-color: #4CAF50; color: white; padding: 14px 20px; margin: 8px 0; border: none; cursor: pointer; width: 10%; }"<<endl<<
            ".active { background-color: #4CAF50;}"<<endl<<
            ".row { margin: 8px -16px;}"<<endl<<
            ".row,"<<endl<<
            ".row > .column { padding: 8px;}"<<endl<<
            ".column { float: left; width: 25%; height: 500px;}"<<endl<<
            ".row:after { content: ''; display: table; clear: both;}"<<endl<<
            ".container { padding: 16px; }"<<endl<<
            "button:hover {opacity: 0.8;}"<<endl<<
            ".content { background-color: #4CAF50;padding: 10px; height: 100%;}"<<endl<<
            "</style>"<<endl<<
            "</head>"<<endl<<
            "<body>"<<endl<<
            "  <ul>"<<endl<<
                "<li><a class='active' href='/home' method='get'>Home</a></li>"<<endl<<
                "<li><a href='/wallet' method='get'>Wallet</a></li>"<<endl<<endl<<
                "<li style='float:right'><a href='/signout' method='get'>Sign out</a></li>"<<endl<<
            "  </ul>"<<endl<<
            "<h2>Welcome, "<<utrip->show_username()<<"!</h2>"<<endl<<
            "<form action='/filter' method='get'>"<<endl<<
            "<div class='container'>"<<endl<<
            "<input type='text' placeholder='Min Star' name='min_star'>"<<endl<<
            "<input type='text' placeholder='Max Star' name='max_star'>"<<endl<<
            "<button type='submit'>Filter!</button>"<<endl<<
            "</div>"<<endl<<
            "</form>"<<endl<<
            "<div class='row'>"<<endl;
        utrip->show_total_hotels(body);
        body<<
            "</div>"<<endl<<
            "</body>"<<endl<<
            "</html>"<<endl;
        response->setBody(body.str());
        utrip->logout();
        return response;
    }
    catch(exception& except){
        errorhandler->activate_not_signed_in_error();
        return Response::redirect(SIGNIN_PATH);
    }
}
FilterHandler::FilterHandler(Utrip* utrip_,ErrorHandler* handler){
    utrip= utrip_;
    errorhandler = handler;
}
Response* FilterHandler::callback(Request* request){
    try{
        utrip->login(request->getSessionId());
        string min_star = request->getQueryParam(MIN_STAR),max_star = request->getQueryParam(MAX_STAR);
        utrip->add_new_filter(new StarFilter(stoi(min_star),stoi(max_star)));
        return Response::redirect(HOME_PATH);
    }
    catch(exception& except){
        errorhandler->activate_not_signed_in_error();
        return Response::redirect(SIGNIN_PATH);
    }
}
DetailsHandler::DetailsHandler(Utrip* utrip_,ErrorHandler* handler){
    utrip = utrip_;
    errorhandler = handler;
}
Response* DetailsHandler::callback(Request* request){
    try{
        utrip->login(request->getSessionId());
        Response* response = new Response;
        ostringstream body;
        response->setHeader("Content-Type","text/html");
        body <<
            "<!DOCTYPE html>"<<endl<<
            "<html>"<< endl<<
            "<head>"<<endl<<
            "<meta name='viewport' content='width=device-width, initial-scale=1'>"<<endl<<
            "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>"<<endl<<
            "<style>"<<endl<<
            "ol.a { list-style-type: circle; }"<<endl<<
            "table,th,td{ border= 1px; border-collapse: collapse;}"<<endl<<
            "th, td { padding: 5px; text-align: center; }"<<endl<<
            "* {box-sizing: border-box;}"<<endl<<
            ".checked {color: orange;}"<<endl<<
            "ul { list-style-type: none; margin: 0; padding: 0; overflow: hidden; background-color: #333;}"<<endl<<
            "body { background-color: white; padding: 20px; font-family: Arial;}"<<endl<<
            "li a { display: block; color: white; text-align: center; padding: 14px 16px; text-decoration: none;}"<<endl<<
            "li a:hover:not(.active) { background-color: #111;}"<<endl<<
            ".column { float: left; width: 100%; }"<<endl<<
            ".column:after { content: ''; display: table; clear: both;}"<<endl<<
            ".content { background-color: #4CAF50;padding: 10px; }"<<endl<<
            "</style>"<<endl<<
            "</head>"<<endl<<
            "<body>"<<endl<<
            "  <ul>"<<endl<<
                "<li style='float:left'><a href='/home' method='get'>Home</a></li>"<<endl<<
                "<li style = 'float:left'><a href='/wallet' method='get'>Wallet</a></li>"<<endl<<endl<<
                "<li style='float:right'><a href='/signout' method='get'>Sign out</a></li>"<<endl<<
            "  </ul>"<<endl;
        utrip->show_specific_hotel(request->getQueryParam(ID),body);
        body<<
            "</body>"<<endl<<
            "</html>"<<endl;
        response->setBody(body.str());
        utrip->logout();
        return response;
    }
    catch(exception& except){
        errorhandler->activate_not_signed_in_error();
        return Response::redirect(SIGNIN_PATH);
    }
}
WalletPageHandler::WalletPageHandler(Utrip* utrip_,ErrorHandler* handler){
    errorhandler = handler;
    utrip = utrip_;
}
Response* WalletPageHandler::callback(Request* request){
    try{
        utrip->login(request->getSessionId());
        Response* response = new Response;
        ostringstream body;
        response->setHeader("Content-Type","text/html");
        body <<
            "<!DOCTYPE html>"<<endl<<
            "<html>"<<endl<<
            "<head>"<<endl<<
            "<meta name='viewport' content='width=device-width, initial-scale=1'>"<<endl<<
            "<link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css'>"<<endl<<
            "<style>"<<endl<<
            "* {box-sizing: border-box;}"<<endl<<
            "ul { list-style-type: none; margin: 0; padding: 0; overflow: hidden; background-color: #333; }"<<endl<<
            "body { background-color: white; padding: 20px; font-family: Arial;}"<<endl<<
            "li { float: left; }"<<endl<<
            "li a { display: block; color: white; text-align: center; padding: 14px 16px; text-decoration: none; }"<<endl<<
            "li a:hover:not(.active) { background-color: #111; }"<<endl<<
            "input[type=text] { width: 10%; padding: 12px 20px; margin: 8px 0; display: inline-block; border: 1px solid #ccc; box-sizing: border-box; }"<<endl<<
            "button { background-color: #4CAF50; color: white; padding: 14px 20px; margin: 8px 0; border: none; cursor: pointer; width: 10% }"<<endl<<
            ".active { background-color: #4CAF50; }"<<endl<<
            ".container { padding: 16px; }"<<endl<<
            "button:hover { opacity: 0.8; }"<<endl<<
            "</style>"<<endl<<
            "</head>"<<endl<<
            "<body>"<<endl<<
            "   <ul>"<<endl<<
            "<li><a  href='/home' method='get'>Home</a></li>"<<endl<<
            "<li><a class='active' href='/wallet' method='get'>Wallet</a></li>"<<endl<<
            "<li style='float:right'><a href='/signout' method='get'>Sign out</a></li>"<<endl<<
            "</ul>"<<endl<<
            "<form action='/wallet' method='post'>"<<endl<<
            "<div class='container'>"<<endl<<
            "<input type='text' placeholder='Value' name='value'>"<<endl<<
            "<button type='submit'>Increase! <i class='fa fa-money'></i></button>"<<endl;
        utrip->show_wallet(body);
        body <<
            "</div>"<<endl<<
            "</form>"<<endl<<
            "</body>"<<endl<<
            "</html>"<<endl;
        response->setBody(body.str());
        utrip->logout();
        return response;
    }
    catch(exception& except){
        errorhandler->activate_not_signed_in_error();
        return Response::redirect(SIGNIN_PATH);
    }
}
WalletHandler::WalletHandler(Utrip* utrip_,ErrorHandler* handler){
    utrip = utrip_;
    errorhandler = handler;
}
Response* WalletHandler::callback(Request* request){
    try{
        utrip->login(request->getSessionId());
        utrip->increase_wallet_value(stod(request->getBodyParam(VALUE)));
        utrip->logout();
        return Response::redirect(WALLET_PATH);
    }
    catch(exception& except){
        errorhandler->activate_not_signed_in_error();
        return Response::redirect(SIGNIN_PATH);
    }
}