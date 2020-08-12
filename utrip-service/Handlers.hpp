#ifndef HANDLERS_H
#define HANDLERS_H "HANDLERS_H"
#include "../server/server.hpp"
class Utrip;
class ErrorHandler{
public:
    ErrorHandler(){ deactivate_errors();}
    void deactivate_errors();
    void activate_signin_error(){ has_activated_signin_error = true;}
    void activate_signup_error(){ has_activated_signup_error = true;}
    void activate_wallet_error(){ has_activatd_wallet_error = true;}
    void activate_password_error(){ has_activated_password_error = true;}
    void activate_not_signed_in_error(){ hasnot_signedin_error = true;}
    bool has_signin_error(){ return has_activated_signin_error;}
    bool has_signup_error(){ return has_activated_signup_error;}
    bool has_wallet_error(){ return has_activatd_wallet_error;}
    bool has_password_error(){ return has_activated_password_error;}
    bool hasnot_signed_in(){ return hasnot_signedin_error;}    
private:
    bool has_activated_signin_error;
    bool has_activated_signup_error;
    bool has_activatd_wallet_error;
    bool has_activated_password_error;
    bool hasnot_signedin_error;
};
class SigninHandler : public RequestHandler{
public:
    SigninHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
class SigninPageHandler : public TemplateHandler{
public:
    SigninPageHandler(std::string filepath,ErrorHandler* handler);
    std::map<std::string,std::string> handle(Request* request);
private:
    ErrorHandler* errorhandler;
};
class SignupPageHandler : public TemplateHandler{
public:
    SignupPageHandler(std::string filepath,ErrorHandler* handler);
    std::map<std::string,std::string> handle(Request* request);
private:
    ErrorHandler* errorhandler;
};
class SignupHandler : public RequestHandler{
public:
    SignupHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
class SignoutHandler : public RequestHandler{
public:
    SignoutHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
class HomeHandler : public RequestHandler{
public:
    HomeHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
class FilterHandler : public RequestHandler{
public:
    FilterHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
class DetailsHandler : public RequestHandler{
public:
    DetailsHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
class WalletPageHandler : public RequestHandler{
public:
    WalletPageHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
class WalletHandler : public RequestHandler{
public:
    WalletHandler(Utrip* utrip_,ErrorHandler* handler);
    Response* callback(Request* request);
private:
    Utrip* utrip;
    ErrorHandler* errorhandler;
};
#endif