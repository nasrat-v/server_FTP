# server_FTP
- Simple FTP server
- Protocol compliant RFC959
- Written in C

# Compile
compiling:
    
    make
    
cleaning obj files:

    make clean
    
cleaning binary files:

    make fclean
    
    
cleaning all and compile:

    make re

# How to use it
Launch the server:

    ./server PORT PATH_TO_STORE_DATA

# Client
To connect your client, you can use Netcat or Telnet client (both are supported).
