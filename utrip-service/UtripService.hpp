#ifndef UTRIP_H
#define UTRIP_H "UTRIP_H"
#include"../utrip-details/PriceFilter.hpp"
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<functional>
class Hotel;
class User;
class Filter;
class Utrip{
public:
    Utrip();
    ~Utrip();
    void save_hotels_informations(std::string hotels_file_path);
    void save_ratings_informations(std::string ratings_file_path);
    void apply_new_sort(std::string property,std::string order);
    bool is_unique(std::string user_data,std::string data_type);
    void check_weights_validity(const std::vector<double>& weights);
    void logout();
    std::string show_username();
    void increase_wallet_value(double amount);
    void show_wallet(std::ostringstream& response_body);
    void sort_hotels_by_default();
    void clear_filters();
    void add_new_filter(Filter* new_filter);
    void show_total_hotels(std::ostringstream& response_body);
    void show_reservations();
    void show_specific_hotel(std::string hotel_id,std::ostringstream& response_body);
    int signup(std::string user_email,std::string username,std::string password);
    int signin(std::string username,std::string user_password);
    void login(std::string client_session_id);
    void signout(std::string client_session_id);
private:
    int session_id;
    int generate_session_id();
    User* logged_in_user;
    User* find_user(std::string username,std::string user_password);
    Hotel* find_hotel(std::string hotel_id);
    void make_reservation(std::string hotel_id,std::string room_type,int quantity,int check_in,int check_out);
    void cancel_reservation(int reserve_id);
    void set_orders();
    std::vector<Hotel*> total_hotels;
    std::vector<Filter*> total_filters;
    std::vector<User*> total_users;
    std::vector<int> save_rooms_numbers(std::stringstream& hotel_info);
    std::vector<int> save_rooms_prices(std::stringstream& hotel_info);
    std::map<std::string,User*> active_session_ids;
    std::map<std::string,std::function<bool(int , int)>> int_orders;
    std::map<std::string,std::function<bool(std::string,std::string)>> string_orders;
    std::map<std::string,std::function<bool(double,double)>> double_orders;
};
#endif