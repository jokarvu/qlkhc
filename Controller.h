#include <iostream>
// #include "Model.h"
// #include "User.h"
// #include "Product.h"
// #include "Bill.h"

class Controller {
    protected:
        User *user;
        Product *product;
        Bill *bill;
    public:
        Controller() {
            this->user = new User;
            this->product = new Product;
            this->bill = new Bill;
            // this->login();
            // this->menu();
        }

        ~Controller() {
            delete this->user;
            delete this->product;
            delete this->bill;
        }

        // Hien thi menu chinh cua ung dung
        void menu() {
            int menu;
            while(true) {
                std::cout << "===== Main Menu =====" << std::endl;
                std::cout << "\t1. Ca nhan\n\t2. Hang hoa\n\t3. Hoa don\n\t4. Thong ke\n\t5. Xoa man hinh\n\t6. Thoat" << std::endl;
                enter_command:
                std::cout << ">> Nhap lenh: ";
                std::cin >> menu;
                if(std::cin.fail()) {
                    std::cout << "Lenh phai la so!" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto enter_command;
                }
                std::cin.ignore();
                // system("clear");
                switch (menu)
                {
                    case 1:
                        system("clear");
                        this->user->menu();
                        break;
                    case 2:
                        system("clear");
                        this->product->menu();
                        break;
                    case 3:
                        system("clear");
                        this->bill->menu();
                        break;
                    case 4:
                        this->dashboard();
                        break;
                    case 5:
                        system("clear");
                        break;
                    case 6:
                        return;
                    default:
                        std::cout << "Lenh khong hop le!" << std::endl;
                        goto enter_command;
                        break;
                }
            }
        }

        // Dang nhap truoc khi su dung he thong
        void login() {
            std::string username;
            std::string password;
            sql::ResultSet *res;
            // Neu khong nhap gi thi bat nhap lai
            do {
                if(username.length()) {
                    std::cout << "Dang nhap khong thanh cong!" << std::endl;
                }
                do {
                std::cout << ">> Nhap ten dang nhap: ";
                fflush(stdin);
                getline(std::cin, username);
                } while(username == "");
                // Neu khong nhap gi thi bat nhap lai
                do {
                    std::cout << ">> Nhap mat khau: ";
                    fflush(stdin);
                    getline(std::cin, password);
                } while(password == "");
                res = this->user->select("*","WHERE username = '" + username + "' && password = '" + password + "'");
            } while (!res->rowsCount());
            res->first();
            system("clear");
            // Cai dat nguoi dung
            this->user->auth->setUsername(username);
            this->user->auth->setPassword(password);
            this->user->auth->setId(res->getInt("id"));
            this->user->auth->setLevel(res->getInt("level"));
            // Cai dat nguoi dung
            this->bill->auth->setUsername(username);
            this->bill->auth->setPassword(password);
            this->bill->auth->setId(res->getInt("id"));
            this->bill->auth->setLevel(res->getInt("level"));
            // Cai dat nguoi dung
            this->product->auth->setUsername(username);
            this->product->auth->setPassword(password);
            this->product->auth->setId(res->getInt("id"));
            this->product->auth->setLevel(res->getInt("level"));
            std::cout << "Dang nhap thanh cong!" << std::endl;
            delete res;
        }

        void dashboard() {
            sql::ResultSet *res;
            res = this->product->select("id", "");
            int product = res->rowsCount();
            res = this->bill->select("id", "WHERE type = 1");
            int inbill = res->rowsCount();
            res =  this->bill->select("id", "WHERE type = 2");
            int outbill = res->rowsCount();
            std::cout << "====== Thong Ke ======" << std::endl;
            std::cout << "So hang hoa co trong kho: " << product << std::endl;
            std::cout << "So hoa don xuat: " << outbill << std::endl;
            std::cout << "So hoa don nhap: " << inbill << std::endl;
            delete res;
        }
};