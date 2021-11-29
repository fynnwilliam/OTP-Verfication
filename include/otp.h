#pragma once

#include <vector>
#include <string>

class otp
{
private:
    std::string recipient_;
    std::string code_;

    void generate_code_r() noexcept;
    void inquire() const noexcept;
    void to_lower() noexcept;
    void trim_email() noexcept;
    bool is_recipient_valid() const noexcept;
    std::vector<char> generate_characters() const noexcept;
    int retry() const noexcept;
    int verify_recipient() const noexcept;
    void remove_leading_spaces() noexcept;
    void remove_trailing_spaces() noexcept;
    void generate_code_s() noexcept;
    void display_info() const noexcept;
    auto submit_code() const;
    void certify() const;
    int declare(auto const&) const noexcept;

    template <typename T>
    int verify_input(T const&, std::string const&) const noexcept;

    template <typename T>
    void verify_code(T const&) const noexcept;

public:
    otp() = default;

    void recipient_email();
    void authenticate_email_s();
    void authenticate_email_r();
};