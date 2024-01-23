// testing random c++ syntax, library etc
#include <iostream>
#include <ctime>
#include <chrono>
#include "date/tz.h"
#include <sstream>

void check_ctime() {
    char start_time[] = "20240118 06:50:00";
    std::tm tm{};
    strptime(start_time, "%Y%m%d %H:%M:%S", &tm);
    // mktime convert to LOCAL time 20240118 06:50:00 to start (epoch time)
    auto start = mktime(&tm);
    std::cout << start << std::endl;
    time_t time = start;
//    time_t time = std::time(nullptr);
    char time_str[std::size("yyyymmdd-hh:mm:ss")];
    std::cout << time << std::endl;
    // std::localtime will convert time back to local time 20240118 06:50:00
    // std::gmtime will convert time to UTC+0 time
    std::tm *ptm = std::gmtime(&time);
    std::strftime(time_str, std::size(time_str), "%Y%m%d-%H:%M:%S", ptm);
    std::cout << time_str << std::endl;
}

void check_time_zone() {
    auto utc_time = std::chrono::system_clock::now();
    auto berlin = date::make_zoned("Europe/Berlin", utc_time);
    auto eastern = date::make_zoned("America/New_York", utc_time);
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp;
    std::istringstream in{"Thu, 9 Jan 2014 12:35:34.032 +0000"};
    in >> date::parse("%a, %d %b %Y %T %z", tp);
    std::cout << date::format("%F %T %Z", utc_time) << std::endl;
    std::cout << date::format("%F %T %Z", berlin) << std::endl;
    std::cout << date::format("%F %T %Z", eastern) << std::endl;
    std::cout << date::format("%F %T %Z", tp) << std::endl;
    auto t = std::chrono::system_clock::from_time_t(1705992493);
    std::cout << date::format("%F %T %Z", t) << std::endl;
}


int main() {
//    check_ctime();
    check_time_zone();
    return 0;

}