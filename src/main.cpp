#include "otp.h"

int main()
{
    otp test("sender's_email", "passwd", "mailserver");

    try
    {
        test.recipient_email();
        test.authenticate_email_s();

        test.recipient_email();
        test.authenticate_email_r();
    }

    catch (std::exception const& e)
    {
        std::cout << "please check your ISP or contact developer.\n";
    }
}
