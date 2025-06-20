#pragma once

#include <string>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sstream>

class EmailSender {
public:
    EmailSender(const std::string& smtp_server, int smtp_port, 
                const std::string& username, const std::string& password);
    
    bool sendVerificationEmail(const std::string& to_email, const std::string& username, 
                              const std::string& verification_code);

private:
    std::string smtp_server_;
    int smtp_port_;
    std::string username_;
    std::string password_;
    
    bool connectToSMTP();
    bool sendCommand(const std::string& command, const std::string& expected_response);
    bool sendData(const std::string& data);
    std::string readResponse();
    void closeConnection();
    
    int sock_fd_;
}; 