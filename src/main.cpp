#include "otp.h"

int main()
{
    otp test{};

    try
    {
        test.recipient_email();
        test.authenticate_email_r();
    }

    catch (std::exception const& e)
    {
        std::cout << "please check your ISP or contact developer.\n";
    }
}
