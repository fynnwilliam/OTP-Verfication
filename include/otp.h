#pragma once

#include <string>
#include <vector>

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
    void retry() const noexcept;
    bool invalid_recipient() const noexcept;
    bool verify_recipient() const noexcept;
    void remove_leading_spaces() noexcept;
    void remove_trailing_spaces() noexcept;
    void generate_code_s() noexcept;
    void display_info() const noexcept;
    auto submit_code() const;
    void error_msg() const noexcept;
    void certify() const;
    void recipient_email();
    void success() const noexcept;
    void make_new_request() const noexcept;
    void declare(auto const&) const noexcept;
    std::string const& recipient() const noexcept;
    std::string& recipient() noexcept;
    std::string& code() noexcept;
    std::string const& code() const noexcept;

    template <typename T>
    void verify_code(T const&) const noexcept;

public:
    otp() = default;

    void authenticate_email_s();
    void authenticate_email_r();
};