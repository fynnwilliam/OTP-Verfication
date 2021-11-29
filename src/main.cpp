#include "otp.h"

#include <iostream>

int main()
{
    try
    {
        otp{}.authenticate_email_r();
    }

    catch (std::exception const& e)
    {
        std::cout << "please check your ISP or contact developer.\n";
    }
}