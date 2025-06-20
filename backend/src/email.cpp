#include "email.hpp"
#include <cstring>
#include <algorithm>

EmailSender::EmailSender(const std::string& smtp_server, int smtp_port, 
                         const std::string& username, const std::string& password)
    : smtp_server_(smtp_server), smtp_port_(smtp_port), 
      username_(username), password_(password), sock_fd_(-1) {}

bool EmailSender::sendVerificationEmail(const std::string& to_email, const std::string& username, 
                                       const std::string& verification_code) {
    if (!connectToSMTP()) {
        std::cerr << "Failed to connect to SMTP server" << std::endl;
        return false;
    }

    // SMTP conversation
    if (!sendCommand("EHLO localhost", "250")) {
        closeConnection();
        return false;
    }

    if (!sendCommand("AUTH LOGIN", "334")) {
        closeConnection();
        return false;
    }

    // Send base64 encoded username
    std::string encoded_username = username_; // In real implementation, encode to base64
    if (!sendCommand(encoded_username, "334")) {
        closeConnection();
        return false;
    }

    // Send base64 encoded password
    std::string encoded_password = password_; // In real implementation, encode to base64
    if (!sendCommand(encoded_password, "235")) {
        closeConnection();
        return false;
    }

    if (!sendCommand("MAIL FROM:<" + username_ + ">", "250")) {
        closeConnection();
        return false;
    }

    if (!sendCommand("RCPT TO:<" + to_email + ">", "250")) {
        closeConnection();
        return false;
    }

    if (!sendCommand("DATA", "354")) {
        closeConnection();
        return false;
    }

    // Email content
    std::string email_content = 
        "From: " + username_ + "\r\n"
        "To: " + to_email + "\r\n"
        "Subject: Welcome to SoulSpeaks - Verify Your Account\r\n"
        "Content-Type: text/html; charset=UTF-8\r\n"
        "\r\n"
        "<html><body>"
        "<h2>Welcome to SoulSpeaks Cloud PBX!</h2>"
        "<p>Hello " + username + ",</p>"
        "<p>Thank you for signing up! Please verify your account by clicking the link below:</p>"
        "<p><a href='http://localhost:8081/verify?code=" + verification_code + "'>Verify Account</a></p>"
        "<p>Or copy and paste this verification code: <strong>" + verification_code + "</strong></p>"
        "<p>If you didn't create this account, please ignore this email.</p>"
        "<p>Best regards,<br>The SoulSpeaks Team</p>"
        "</body></html>"
        "\r\n.\r\n";

    if (!sendData(email_content)) {
        closeConnection();
        return false;
    }

    if (!sendCommand("QUIT", "221")) {
        closeConnection();
        return false;
    }

    closeConnection();
    std::cout << "Verification email sent to: " << to_email << std::endl;
    return true;
}

bool EmailSender::connectToSMTP() {
    sock_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_fd_ < 0) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    struct hostent* server = gethostbyname(smtp_server_.c_str());
    if (server == NULL) {
        std::cerr << "Failed to resolve hostname: " << smtp_server_ << std::endl;
        close(sock_fd_);
        return false;
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(smtp_port_);
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sock_fd_, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to connect to SMTP server" << std::endl;
        close(sock_fd_);
        return false;
    }

    // Read initial greeting
    std::string response = readResponse();
    if (response.find("220") != 0) {
        std::cerr << "Unexpected SMTP greeting: " << response << std::endl;
        close(sock_fd_);
        return false;
    }

    return true;
}

bool EmailSender::sendCommand(const std::string& command, const std::string& expected_response) {
    std::string full_command = command + "\r\n";
    if (send(sock_fd_, full_command.c_str(), full_command.length(), 0) < 0) {
        std::cerr << "Failed to send command: " << command << std::endl;
        return false;
    }

    std::string response = readResponse();
    if (response.find(expected_response) != 0) {
        std::cerr << "Unexpected response for command '" << command << "': " << response << std::endl;
        return false;
    }

    return true;
}

bool EmailSender::sendData(const std::string& data) {
    if (send(sock_fd_, data.c_str(), data.length(), 0) < 0) {
        std::cerr << "Failed to send email data" << std::endl;
        return false;
    }

    std::string response = readResponse();
    if (response.find("250") != 0) {
        std::cerr << "Failed to send email data. Response: " << response << std::endl;
        return false;
    }

    return true;
}

std::string EmailSender::readResponse() {
    char buffer[1024];
    int bytes_read = recv(sock_fd_, buffer, sizeof(buffer) - 1, 0);
    if (bytes_read < 0) {
        return "";
    }
    buffer[bytes_read] = '\0';
    return std::string(buffer);
}

void EmailSender::closeConnection() {
    if (sock_fd_ >= 0) {
        close(sock_fd_);
        sock_fd_ = -1;
    }
} 