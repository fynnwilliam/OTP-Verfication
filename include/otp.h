#pragma once

#include <algorithm>
#include <iterator>
#include <iostream>
#include <exception>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <cctype>
#include <regex>

class otp
{
private:
    std::vector<char> v_;
    std::string recipient_;
    std::string code_;
    std::string input_;
    std::string const sender_;
    std::string const password_;
    std::string const mailserver_;
    std::regex const pattern{"(\\w+)(\\.|_)?(\\w+)@(\\w+)(\\.(\\w+))+"};
    int status_{-1};

    void generate_code_r();
    void inquire() const;
    void to_lower();
    void trim_email();
    bool is_recipient_valid() const;
    void generate_characters();
    int retry() const;
    int verify_recipient() const;
    void remove_leading_spaces();
    void remove_trailing_spaces();
    void generate_code_s();
    void display_info() const;
    auto submit_code();
    void certify();
    int declare(auto const&);

    template <typename T>
    int verify_input(T const&);

    template <typename T>
    void verify_code(T const&);

public:
    otp(std::string, std::string, std::string);

    void recipient_email();
    void authenticate_email_s();
    void authenticate_email_r();
};