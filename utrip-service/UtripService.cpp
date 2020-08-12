#include"UtripService.hpp"
using namespace std;
#include<fstream>
#include<iostream>
#include<algorithm>
#include<functional>
#include"../utrip-details/Hotel.hpp"
#include"../utrip-details/User.hpp"
#include"../utrip-details/Comment.hpp"
#include"../utrip-details/NotFound.hpp"
#include"../utrip-details/StarFilter.hpp"
#include"../utrip-details/SameType.hpp"
#include"../utrip-details/BadRequest.hpp"
#define COMMA_SEPERATOR ','
#define QUESTION_MARK "?"
#define EMAIL "email"
#define SPACE ' '
#define USERNAME "username"
#define PASSWORD "password"
#define INITIAL_SESSION_ID 0
#define MIN_WALLET_INCREASEMENT 0
#define MIN_NUMS 0
#define FIRST_FILTER_INDEX 0
#define EMPTY "Empty"
#define OK "OK"
#define POST "POST"
#define GET "GET"
#define DELETE "DELETE"
#define SIGNUP "signup"
#define LOGIN "login"
#define LOGOUT "logout"
#define WALLET "wallet"
#define HOTELS "hotels"
#define FILTERS "filters"
#define RESERVES "reserves"
#define COMMENTS "comments"
#define RATINGS "ratings"
#define ID "id"
#define COUNT "count"
#define AMOUNT "amount"
#define HOTEL "hotel"
#define TYPE "type"
#define QUANTITY "quantity"
#define CHECK_IN "check_in"
#define CHECK_OUT "check_out"
#define CITY "city"
#define MIN_PRICE_ "min_price"
#define MIN_PRICE 0
#define MAX_PRICE "max_price"
#define MIN_STAR_ "min_star"
#define MIN_STAR 1
#define MAX_STAR_ "max_star"
#define MAX_STAR 5
#define COMMENT "comment"
#define LOCATION "location"
#define CLEANLINESS "cleanliness"
#define STAFF "staff"
#define FACILITIES "facilities"
#define VALUE_FOR_MONEY "value_for_money"
#define OVERALL_RATING "overall_rating"
#define DEFAULT_PRICE_FILTER "default_price_filter"
#define ACTIVE "active"
#define TRUE "true"
#define FALSE "false"
#define ASCENDING "ascending"
#define DESCENDING "descending"
#define SORT "sort"
#define PROPERTY "property"
#define ORDER "order"
#define NAME "name"
#define STAR_RATING "star_rating"
#define STANDARD_ROOM_PRICE "standard_room_price"
#define DELUXE_ROOM_PRICE "deluxe_room_price"
#define LUXURY_ROOM_PRICE "luxury_room_price"
#define PREMIUM_ROOM_PRICE "premium_room_price"
#define AVERAGE_ROOM_PRICE "average_room_price"
#define RATING_PERSONAL "rating_personal"
#define MANUAL_WEIGHTS "manual_weights"
#define ESTIMATED_WEIGHTS "estimated_weights"
#define ROOM_KEY_INDEX 0
#define MIN_WEIGHTS 1.0
#define MAX_WEIGHTS 5.0
Utrip::Utrip(){
    logged_in_user = NULL;
    session_id = INITIAL_SESSION_ID;
    set_orders();
}
int Utrip::generate_session_id(){ 
    return session_id++;
}
void Utrip::login(string client_session_id){
    auto client = active_session_ids.find(client_session_id);
    if(client == active_session_ids.end()){
        throw BadRequest();
    }
    logged_in_user = client->second;
}
void Utrip::signout(string client_session_id){
    active_session_ids.erase(client_session_id);
    logout();
}
void Utrip::set_orders(){
    int_orders[ASCENDING] = less<int>();
    int_orders[DESCENDING] = greater<int>();
    string_orders[ASCENDING] = less<string>();
    string_orders[DESCENDING] = greater<string>();
    double_orders[ASCENDING] = less<double>();
    double_orders[DESCENDING] = greater<double>();
}
void Utrip::sort_hotels_by_default(){
    sort(total_hotels.begin(),total_hotels.end(),[](Hotel* first,Hotel* second){ return (first->get_id() < second->get_id());});
}
void Utrip::save_hotels_informations(string hotels_file_path){
    ifstream hotels_file(hotels_file_path);
    string temperory,hotels_info,unique_id,property_name,hotel_star_rating,hotel_overview,property_amenities,city,latitude,longitude,image_url;
    getline(hotels_file,temperory);
    while(getline(hotels_file,hotels_info)){
        stringstream hotel_info(hotels_info);
        getline(hotel_info,unique_id,COMMA_SEPERATOR);
        getline(hotel_info,property_name,COMMA_SEPERATOR);
        getline(hotel_info,hotel_star_rating,COMMA_SEPERATOR);
        getline(hotel_info,hotel_overview,COMMA_SEPERATOR);
        getline(hotel_info,property_amenities,COMMA_SEPERATOR);
        getline(hotel_info,city,COMMA_SEPERATOR);
        getline(hotel_info,latitude,COMMA_SEPERATOR);
        getline(hotel_info,longitude,COMMA_SEPERATOR);
        getline(hotel_info,image_url,COMMA_SEPERATOR);
        vector<int> rooms_numbers = save_rooms_numbers(hotel_info);
        vector<int> rooms_prices = save_rooms_prices(hotel_info);
        total_hotels.push_back(new Hotel(unique_id,property_name,stoi(hotel_star_rating),hotel_overview,property_amenities,city,stod(latitude),stod(longitude),image_url,rooms_numbers,rooms_prices));        
    }
    sort_hotels_by_default();
}
vector<int> Utrip::save_rooms_numbers(stringstream& hotel_info){
    vector<int> hotel_rooms_numbers;
    string room_numbers;
    getline(hotel_info,room_numbers,COMMA_SEPERATOR);
    hotel_rooms_numbers.push_back(stoi(room_numbers));
    getline(hotel_info,room_numbers,COMMA_SEPERATOR);
    hotel_rooms_numbers.push_back(stoi(room_numbers));
    getline(hotel_info,room_numbers,COMMA_SEPERATOR);
    hotel_rooms_numbers.push_back(stoi(room_numbers));
    getline(hotel_info,room_numbers,COMMA_SEPERATOR);
    hotel_rooms_numbers.push_back(stoi(room_numbers));
    return hotel_rooms_numbers;
}
vector<int> Utrip::save_rooms_prices(stringstream& hotel_info){
    vector<int> hotel_rooms_prices;
    string room_price;
    getline(hotel_info,room_price,COMMA_SEPERATOR);
    hotel_rooms_prices.push_back(stoi(room_price));
    getline(hotel_info,room_price,COMMA_SEPERATOR);
    hotel_rooms_prices.push_back(stoi(room_price));
    getline(hotel_info,room_price,COMMA_SEPERATOR);
    hotel_rooms_prices.push_back(stoi(room_price));
    getline(hotel_info,room_price,COMMA_SEPERATOR);
    hotel_rooms_prices.push_back(stoi(room_price));
    return hotel_rooms_prices;
}
bool Utrip::is_unique(string user_data,string data_type){
    if(data_type == EMAIL){
        for(int i = 0;i<total_users.size();i++){
            if(user_data == total_users[i]->get_email()){
                return false;
            }
        }
        return true;
    }
    else if(data_type == USERNAME){
        for(int i=0;i<total_users.size();i++){
            if(user_data == total_users[i]->get_username()){
                return false;
            }
        }
        return true;
    }
}
int Utrip::signup(string user_email,string username,string password){
    if(!is_unique(user_email,EMAIL) || !is_unique(username,USERNAME)){
        throw BadRequest();
    }
    total_users.push_back(new User(user_email,password,username));
    int new_session_id = generate_session_id();
    active_session_ids[to_string(new_session_id)] = total_users.back();
    return new_session_id;
}
User* Utrip::find_user(string username,string user_password){
    hash<string> hashed_password;
    auto selected_user = find_if(total_users.begin(),total_users.end(),[=](User* user){return user->get_username() == username 
    && user->get_password() == hashed_password(user_password);});
    if(selected_user != total_users.end()){
        return *selected_user;
    }
    return NULL;
}
int Utrip::signin(string username,string user_password){
    User* chosen_user = find_user(username,user_password);
    if(chosen_user == NULL){
        throw BadRequest();
    }
    int new_session_id = generate_session_id();
    active_session_ids[to_string(new_session_id)] = chosen_user;
    return new_session_id;
}
void Utrip::logout(){
    clear_filters();
    logged_in_user = NULL;
}
void Utrip::increase_wallet_value(double amount){
    if(amount <= MIN_WALLET_INCREASEMENT){
        throw BadRequest();
    }
    logged_in_user->increase_wallet_value(amount);
}
void Utrip::show_wallet(ostringstream& response_body){
    logged_in_user->show_wallet(response_body);
}
void Utrip::show_total_hotels(ostringstream& response_body){
    vector<Hotel*> to_be_shown_hotels;
    for(int i=0;i<total_filters.size();i++){
        if(i==FIRST_FILTER_INDEX){
            total_filters[i]->apply(total_hotels);
        }
        else{
            total_filters[i]->apply(total_filters[i-1]->get_filtered_hotels());
        }
    }
    if(total_filters.size()){
        to_be_shown_hotels = total_filters.back()->get_filtered_hotels();
    }
    else{
        to_be_shown_hotels = total_hotels;
    }
    for(int i=0;i<to_be_shown_hotels.size();i++){
        to_be_shown_hotels[i]->print_general_informations(response_body);
    }
}
Hotel* Utrip::find_hotel(string hotel_id){
    auto selected_hotel = find_if(total_hotels.begin(),total_hotels.end(),[=](Hotel* hotel){ return hotel->get_id() == hotel_id;});
    if(selected_hotel != total_hotels.end()){
        return *selected_hotel;
    }
    return NULL;
}
void Utrip::show_specific_hotel(string hotel_id,ostringstream& response_body){
    Hotel* chosen_hotel =find_hotel(hotel_id);
    if(chosen_hotel == NULL){
        throw NotFound();
    }
    chosen_hotel->print_specific_informations(response_body);
}
void Utrip::make_reservation(string hotel_id,string room_type,int quantity,int check_in,int check_out){
    Hotel* chosen_hotel = find_hotel(hotel_id);
    if(chosen_hotel == NULL){
        throw NotFound();
    }
    logged_in_user->make_reservation(chosen_hotel->make_reservation(room_type,quantity,check_in,check_out,logged_in_user->get_wallet()));
}
void Utrip::cancel_reservation(int reserve_id){
    logged_in_user->cancel_reservation(reserve_id);
}
void Utrip::clear_filters(){
    for(int i=0;i<total_filters.size();i++){
        delete total_filters[i];
    }
    total_filters.clear();
}
void Utrip::show_reservations(){
    logged_in_user->show_reservations();
}
void Utrip::apply_new_sort(string property,string order){
    if(property == ID){
        auto comparison = string_orders[order];
        stable_sort(total_hotels.begin(),total_hotels.end(),[=](Hotel* first,Hotel* second){ return comparison(first->get_id(),second->get_id()); });
    }
    else if(property == NAME){
        auto comparison = string_orders[order];
        stable_sort(total_hotels.begin(),total_hotels.end(),[=](Hotel* first,Hotel* second){ if(first->get_name() == second->get_name()){ return first->get_id() < second->get_id();}
            return comparison(first->get_name(),second->get_name()); });
    }
    else if(property == CITY){
        auto comparison = string_orders[order];
        stable_sort(total_hotels.begin(),total_hotels.end(),[=](Hotel*first,Hotel* second){ if(first->get_city() == second->get_city()){ return first->get_id() < second->get_id();}
            return comparison(first->get_city(),second->get_city()); });
    }
    else if(property == STAR_RATING){
        auto comparison = int_orders[order];
        stable_sort(total_hotels.begin(),total_hotels.end(),[=](Hotel*first , Hotel* second){ if(first->get_star() == second->get_star()) { return first->get_id() < second->get_id();}
            return comparison(first->get_star(),second->get_star()); });
    }
    else if(property == STANDARD_ROOM_PRICE || property == DELUXE_ROOM_PRICE || property == LUXURY_ROOM_PRICE || property == PREMIUM_ROOM_PRICE){
        auto comparison = int_orders[order];
        stable_sort(total_hotels.begin(),total_hotels.end(),[=](Hotel* first,Hotel* second){ if(first->get_room_price(property[ROOM_KEY_INDEX])==second->get_room_price(property[ROOM_KEY_INDEX])){
            return first->get_id() < second->get_id();
        }
            return comparison(first->get_room_price(property[ROOM_KEY_INDEX]),second->get_room_price(property[ROOM_KEY_INDEX])); });
    }
    else if(property == AVERAGE_ROOM_PRICE){
        auto comparison = double_orders[order];
        stable_sort(total_hotels.begin(),total_hotels.end(),[=](Hotel* first, Hotel* second){ if(first->get_average_price() == second->get_average_price()){ return first->get_id() < second->get_id();}
            return comparison(first->get_average_price(),second->get_average_price()); });
    }
    else{
        throw BadRequest();
    }
}
void Utrip::check_weights_validity(const vector<double>& weights){
    auto invalid_weight_iterator = find_if(weights.begin(),weights.end(),[](double weight) { return weight > MAX_WEIGHTS || weight < MIN_WEIGHTS;});
    if(invalid_weight_iterator != weights.end()){
        throw BadRequest();
    }
}
Utrip::~Utrip(){
    for(int i=0;i<total_users.size();i++){
        delete total_users[i];
    }
    for(int i=0;i<total_hotels.size();i++){
        delete total_hotels[i];
    }
    clear_filters();
}
void Utrip::add_new_filter(Filter* new_filter){
    auto same_filter_iterator = find_if(total_filters.begin(),total_filters.end(),SameFilterType(new_filter));
    if(same_filter_iterator != total_filters.end()){
        delete (*same_filter_iterator);
        total_filters.erase(same_filter_iterator);
    }
    total_filters.push_back(new_filter);
}
void Utrip::save_ratings_informations(string ratings_file_path){
    ifstream ratings_file(ratings_file_path);
    string temperory,ratings_info,hotel_id;
    getline(ratings_file,temperory);
    while(getline(ratings_file,ratings_info)){
        stringstream rating_info(ratings_info);
        getline(rating_info,hotel_id,COMMA_SEPERATOR);
        Hotel* selected_hotel = find_hotel(hotel_id);
        if(selected_hotel == NULL){
            throw NotFound();
        }
        selected_hotel->save_average_rating(rating_info);
    }
}
string Utrip::show_username(){
    return logged_in_user->get_username();
}