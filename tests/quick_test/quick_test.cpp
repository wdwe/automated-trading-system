// testing random c++ syntax, library etc
#include <iostream>
#include <ctime>

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
    std::tm *ptm = std::gmtime(&time);
    std::strftime(time_str, std::size(time_str), "%Y%m%d-%H:%M:%S", ptm);
    std::cout << time_str << std::endl;
}


int main() {
    check_ctime();
    return 0;
}