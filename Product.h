// #include "Model.h"
// #include "Auth.h"

class Product : public Model {
    public:
        Auth *auth;
        Product() {
            this->table = "products";
            this->auth = new Auth;
        }
        ~Product() {
            delete this->auth;
        }

        void menu() {
            int menu;
            while(true) {
                std::cout << std::endl;
                std::cout << "====== Menu Hang Hoa ======" << std::endl;
                std::cout << "\t1. Danh sach hang hoa\n\t2. Tim kiem hang hoa\n\t3. Them hang hoa\n\t4. Sua hang hoa\n\t5. Xoa man hinh\n\t6. Quay lai" << std::endl;
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
                switch(menu) {
                    case 1:
                        this->all();
                        break;
                    case 2:
                        this->find();
                        break;
                    case 3:
                        this->newItem();
                        break;
                    case 4:
                        this->edit();
                        break;
                    case 5:
                        system("clear");
                        break;
                    case 6:
                        system("clear");
                        return;
                    default:
                        std::cout << "Lenh khong hop le!" << std::endl;
                        goto enter_command;
                        break;
                }
            }
        }

        void all() {
            sql::ResultSet *res;
            res = this->select("*", "");
            system("clear");
            std::cout << "===== Danh Sach Hang Hoa =====" << std::endl;
            while(res->next()) {
                std::cout << "ID: " << res->getInt("id") << std::endl;
                std::cout << "Ma hang hoa: " << res->getString("code") << std::endl;
                std::cout << "Tem hang hoa: " << res->getString("name") << std::endl;
                std::cout << "So luong: " << res->getInt("amount") << std::endl;
                std::cout << "Mo ta: " << res->getString("description") << std::endl;
                std::cout << "=========================================" << std::endl;
            }
            delete res;
        }

        void find() {
            std::string product_code;
            do {
                std::cout << ">> Nhap ma hang hoa: ";
                fflush(stdin);
                getline(std::cin, product_code);
            } while (product_code == "");
            sql::ResultSet *res;
            res = this->select("*", "WHERE code = '" + product_code + "'");
            if(res->rowsCount()) {
                std::cout << "Tim thay: " << res->rowsCount() << std::endl;
                std::cout << "===== Ket Qua Tim Kiem =====" << std::endl;
                while(res->next()) {
                    std::cout << "ID: " << res->getInt("id") << std::endl;
                    std::cout << "Ma hang hoa: " << res->getString("code") << std::endl;
                    std::cout << "Ten hang hoa: " << res->getString("name") << std::endl;
                    std::cout << "So luong: " << res->getInt("amount") << std:: endl;
                    std::cout << "========================" << std::endl;
                }
            } else {
                std::cout << "Khong tim thay hang hoa co ma: " << product_code << std::endl;
            }
        }

        void newItem() {
            if(this->auth->getLevel() != 1) {
                std::cout << "** Chi quan ly moi co quyen them hang hoa!" << std::endl;
                return;
            } 
            sql::ResultSet *res;
            std::string product_code;
            std::string name;
            int amount;
            std::string des;
            std::cout << "===== Them Hang Hoa Moi =====" << std::endl;
            do {
                if(product_code != "") {
                    std::cout << "Ma hang hoa da ton tai! Moi ban su dung chuc hang hoa don de nhap/xuat hang hoa!" << std::endl;
                }
                std::cout << ">> Nhap ma hang hoa: ";
                fflush(stdin);
                getline(std::cin, product_code);
                res = this->select("*", "WHERE code = '" + product_code + "'");
            } while(product_code == "" || res->rowsCount());
            do {
                std::cout << ">> Nhap ten hang hoa: ";
                fflush(stdin);
                getline(std::cin, name);
            } while(name == "");
            do {
                if(amount != 0 && amount < 0) {
                    std::cout << "So luong hang hoa phai lon hon 0!" << std::endl;
                }
                enter_amount:
                std::cout << ">> Nhap so luong: ";
                std::cin >> amount;
                if(std::cin.fail()) {
                    std::cout << "Ban phai nhap so!" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto enter_amount;
                }
                std::cin.ignore();
            } while(amount < 0);
            std::cout << ">> Nhap mo ta hang hoa: ";
            fflush(stdin);
            getline(std::cin, des);
            if(this->create("(code, name, amount, description) VALUES('" + product_code + "', '" + name + "', '" + std::to_string(amount) + "', '" + des + "')")) {
                // Tao hoa don nhap hang
                res = this->select("*", "WHERE code = '" + product_code + "'");
                res->first();
                // int product_id = res->getInt("id");
                int type = 1;
                // this->bill->newBill(product_id, amount, 1);  Tao hoa don moi
                if(this->create("bills", "(product_id, user_id, amount, type) VALUES('" + std::to_string(res->getInt("id")) + "', '" + std::to_string(this->auth->getId()) + "', '" + std::to_string(amount) + "', '" + std::to_string(type) + "')")) {
                    std::cout << "Tao hoa don thanh cong!" << std::endl;
                } else {
                    std::cout << "Tao hoa don that bai" << std::endl;
                }
                std::cout << "Tao moi hang hoa thanh cong!" << std::endl;
            } else {
                std::cout << "Co loi xay ra. Vui long thu lai sau" << std::endl;
            }
            delete res;
        }

        void edit() {

        }

        void deleteItem() {
            if(this->auth->getId() != 1) {
                std::cout << "** Chi quan ly moi co quyen xoa hang hoa!" << std::endl;
                return;
            }
            std::string product_code;
            std::cout << ">> Nhap ma hang hoa: ";
            fflush(stdin);
            getline(std::cin, product_code);
            sql::ResultSet *res;
            res = this->select("*", "WHERE code = '" + product_code + "'");
            if(!res->rowsCount()) {
                std::cout << "** Hang hoa chua ton tai trong co so du lieu!" << std::endl;
                delete res;
                return;
            }
            std::cout << "Tim thay: " << res->rowsCount() << std::endl;
            std::cout << "===== Ket Qua Tim Kiem =====" << std::endl;
            while(res->next()) {
                std::cout << "ID: " << res->getInt("id") << std::endl;
                std::cout << "Ma hang hoa: " << res->getString("code") << std::endl;
                std::cout << "Ten hang hoa: " << res->getString("name") << std::endl;
                std::cout << "So luong: " << res->getInt("amount") << std:: endl;
                std::cout << "========================" << std::endl;
            }
            char choice;
            enter_choice:
            std::cout << ">> Ban cho chac chan muon xoa hang hoa tren va tat ca thong tin don hang cua hang hoa khong (y/n) : ";
            std::cin >> choice;
            if(std::cin.fail()) {
                std::cout << "Ban phai nhap so!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                goto enter_choice;
            }
            std::cin.ignore();
            switch(choice) {
                case 'y':
                    break;
                case 'n':
                    delete res;
                    return;
                default:
                    std::cout << "Lenh khong hop le. Nhap y(yes) hoac n(no)!" << std::endl;
                    goto enter_choice;
                    break;
            }
            if(this->forceDelete("bills", "WHERE product_id = '" + std::to_string(res->getInt("id")) + "'")) {
                this->forceDelete("WHERE id = '" + std::to_string(res->getInt("id")) + "'");
                std::cout << "Xoa hang hoa thanh cong!" << std::endl;
            } else {
                std::cout << "** Co loi xay ra vui long thu lai sau!" << std::endl;
            }
            delete res;
        }
};