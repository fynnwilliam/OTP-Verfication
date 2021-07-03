## otp-with-smtp
Performing OTP verification over SMTP


##### install curl and libcurl

[download latest version](https://curl.se/docs/install.html) and select TLS backend when configuring

or, `sudo apt install curl libcurl4-openssl-dev`

##### compile

* `mkdir build`
* `cd build`
* `cmake ..`
* `make`


##### run

`./otp`