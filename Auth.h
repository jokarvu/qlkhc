#include <iostream>

class Auth {
    protected:
        int id;
        int level;
        std::string username;
        std::string password;
    public:
        int getId() {
            return this->id;
        }

        std::string getType() {
            if(this->id == 1) {
                return "Quan Ly";
            }
            return "Nhan Vien";
        }

        int getLevel() {
            return this->level;
        }

        std::string getUsername() {
            return this->username;
        }

        std::string getPassword() {
            return this->password;
        }

        void setId(int id) {
            this->id = id;
        }

        void setUsername(std::string username) {
            this->username = username;
        }

        void setPassword(std::string password) {
            this->password = password;
        }

        void setLevel(int level) {
            this->level = level;
        }
};