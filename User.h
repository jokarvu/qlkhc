// #include "Model.h"
// #include "Auth.h"

class User : public Model {
    public:
        Auth *auth;
        User() {
            this->auth = new Auth;
            this->table = "users";
        }
        ~User() {
            delete this->auth;
        }
        void menu() {
            int menu;
            while(true) {
                std::cout << std::endl;
                std::cout << "===== Menu Nhan Vien =====" << std::endl;
                std::cout << "\t1. Xem thong tin\n\t2. Doi mat khau\n\t3. Them nhan vien\n\t4. Xem danh sach nhan vien\n\t5. Xoa man hinh\n\t6. Ve menu chinh" << std::endl;
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
                system("clear");
                switch(menu) {
                    case 1:
                        this->profile(this->auth);
                        break;
                    case 2:
                        this->changePassword();
                        break;
                    case 3:
                        this->newItem();
                        break;
                    case 4:
                        this->all();
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
        
        void profile(Auth *auth) {
            // In ra thong tin nhan vien dang su dung he thong
            system("clear");
            std::cout << "===== Thong Tin Nhan Vien =====" << std::endl;
            std::cout << "Ten dang nhap: " << auth->getUsername() << std::endl;
            std::cout << "Mat khau: " << auth->getPassword() << std::endl;
            std::cout << "Loai tai khoan: " << auth->getType() << std::endl;
        }

        void newItem() {
            if(this->auth->getLevel() != 1) {
                // Neu la nhan vien thi khong cho phep tao
                std::cout << "Ban khong co quyen tao nhan vien moi!" << std::endl;
                return; 
            }
            // Neu la quan ly thi cho phep tao nhan vien
            std::string username;
            std::string password;
            std::string re_password;
            int level;
            // sql::ResultSet *res;
            std::cout << "===== Tao nhan vien moi =====" << std::endl;
            do {
                if(username != "") {
                    std::cout << "Nhan vien da ton tai!" << std::endl;
                }
                do {
                    std::cout << ">> Nhap ten dang nhap: ";
                    fflush(stdin);
                    getline(std::cin, username);
                } while (username == "");
                do {
                    std::cout << ">> Nhap mat khau: ";
                    fflush(stdin);
                    getline(std::cin, password);
                    std::cout << ">> Nhap lai mat khau: ";
                    fflush(stdin);
                    getline(std::cin, re_password);
                } while (password == "" || password != re_password);
                do {
                    enter_level:
                    std::cout << ">> Nhap loai tai khoan (1/2) (1: Quan ly, 2: Nhan vien): ";
                    std::cin >> level;
                    if(std::cin.fail()) {
                        std::cout << "Ban phai nhap so!" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        goto enter_level;
                    }
                    std::cin.ignore();
                } while(level != 1 && level != 2);
            } while(!this->create("(username, password, level) VALUES('" + username + "', '" + password + "' , '" + std::to_string(level) +"')"));
            system("clear");
            std::cout << "Tao tai khoan moi thanh cong!" << std::endl;
        }

        void changePassword() {
            std::string password;
            std::string newpassword;
            std::string re_password;
            std::cout << "===== Doi mat khau =====" << std::endl;
            do {
                // Meu mat khau hien tai khong trung nhau thi yeu cau nhap lai
                if(password != "") {
                    std::cout << "Sai mat khau!" << std::endl;
                }
                std::cout << ">> Nhap mat khau hien tai: ";
                fflush(stdin);
                getline(std::cin, password);
            } while (password != this->auth->getPassword());
            do {
                if(newpassword != "") {
                    // Neu mat khau moi khong trung nhau thi thong bao loi va bat nhap lai
                    std::cout << "Mat khau moi khong trung khop!" << std::endl;
                }
                do {
                    // Neu mat khau moi rong thi bat nhap lai
                    std::cout << ">> Nhap mat khau moi: ";
                    fflush(stdin);
                    getline(std::cin, newpassword);
                } while (newpassword == "");
                std::cout << ">> Nhap lai mat khau moi: ";
                fflush(stdin);
                getline(std::cin, re_password);
            } while (newpassword != re_password);
            system("clear");
            if(this->update("password = '" + newpassword + "'", "id = '" + std::to_string(this->auth->getId()) + "'")) {
                std::cout << "Doi mat khau thanh cong!" << std::endl;
                this->auth->setPassword(newpassword);
            } else {
                std::cout << "Co loi xay ra. Vui long thu lai sau!" << std::endl;
            }
        }

        void all() {
            system("clear");
            sql::ResultSet *res;
            res = this->select("*", "");
            while(res->next()) {
                std::cout << "===== Thong tin nhan vien =====" << std::endl;
                std::cout << "ID: " << res->getInt("id") << std::endl;
                std::cout << "Ten dang nhap: " << res->getString("username") << std::endl;
                std::cout << "Loai tai khoan: ";
                if(res->getInt("level") == 1) {
                    std::cout << "Quan Ly";
                } else {
                    std::cout << "Nhan Vien";
                }
                std::cout << std::endl;
            }
            delete res;
        }
};