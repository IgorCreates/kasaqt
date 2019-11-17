# This Python file uses the following encoding: utf-8

# if__name__ == "__main__":
#     pass
import fisk
import lxml.etree as et

#As we did not set environment with FiskInit default environment is DEMO. This
#works just with EchoRequest as it does not require key (with password) and certificate.

#test echo
echo = fisk.EchoRequest("Proba echo poruke")

#send request and print server reply
echo_reply = echo.execute()
if(echo_reply != False):
    print(echo_reply)
else:
    errors = echo.get_last_error()
    print("EchoRequest errors:")
    for error in errors:
        print(error)
