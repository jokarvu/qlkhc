// #include "Model.h"
// #include "Auth.h"

class Bill : public Model {
    public:
        Auth *auth;
        Bill() {
            this->auth = new Auth;
            this->table = "bills";
        }
        ~Bill() {
            delete this->auth;
        }

        std::string getType(int type) {
            if(type == 1) {
                return "HDN";
            }
            return "HDX";
        }

        void menu() {
            int menu;
            while(true) {
                std::cout << "===== Menu Hoa Don Xuat/Nhap =====" << std::endl;
                std::cout << "\t1. In hoa don\n\t2. Tim hoa don\n\t3. Tao hoa don\n\t4. Sua hoa don\n\t5. Xoa man hinh\n\t6. Ve menu chinh" << std::endl;
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
                        return;
                    default:
                        std::cout << "Lenh khong hop le. Moi ban nhap lai!" << std::endl;
                        goto enter_command;
                        break;
                }
            }
        }

        void all() {
            sql::ResultSet *res;
            res = this->select("bills.*, users.username as user_name, products.code as product_code", "LEFT JOIN users ON users.id = bills.user_id LEFT JOIN products ON products.id = bills.product_id");
            std::cout << "===== DANH SACH HOA HON =====" << std::endl;
            while(res->next()) {
                std::cout << "ID: " << res->getInt("id") << std::endl;
                std::cout << "Ma hang hoa: " << res->getString("product_code") << std::endl;
                std::cout << "Nguoi tao: " << res->getString("user_name") << std::endl;
                std::cout << "So luong: " << res->getInt("amount") << std::endl;
                std::cout << "Loai hoa don: " << this->getType(res->getInt("type")) << std::endl;
                std::cout << "Thoi gian tao: " << res->getString("created_at") << std::endl;
                std::cout << "==============================================" << std::endl;
            }
            delete res;
        }

        void find() {
            sql::ResultSet *res;
            std::string tmp;
            do {
                if(tmp != "") {
                    std::cout << "Co loi xay ra. Vui long thu lai sau!" << std::endl;
                }
                std::cout << "Nhap tu khoa tim kiem: ";
                fflush(stdin);
                getline(std::cin, tmp);
            } while(tmp == "");
            res = this->select("bills.*, users.username as user_name, products.code as product_code", "LEFT JOIN users ON users.id = bills.user_id LEFT JOIN products ON products.id = bills.product_id WHERE users.username = '" + tmp + "' OR products.code = '" + tmp + "' OR bills.amount = '" + tmp + "'");
            std::cout << "Ket qua tim kiem: " << res->rowsCount() << std::endl;
            std::cout << "===============================================" << std::endl;
            while(res->next()) {
                std::cout << "ID: " << res->getInt("id") << std::endl;
                std::cout << "Ma hang hoa: " << res->getString("product_code") << std::endl;
                std::cout << "Nguoi tao: " << res->getString("user_name") << std::endl;
                std::cout << "So luong: " << res->getInt("amount") << std::endl;
                std::cout << "Loai hoa don: " << this->getType(res->getInt("type")) << std::endl;
                std::cout << "Thoi gian tao: " << res->getString("created_at") << std::endl;
                std::cout << "==============================================" << std::endl;
            }
            delete res;
        }

        void newItem() {
            sql::ResultSet *res;
            std::string product_code;
            int amount;
            int type;
            std::cout << "===== Them moi hoa don =====" << std::endl;
            do {
                enter_type:
                std::cout << "Nhap loai hoa don (1 - Hoa don nhap, 2 - Hoa don xuat): ";
                std::cin >> type;
                if(std::cin.fail()) {
                    std::cout << "Ban phai nhap so!" << std::endl;
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    goto enter_type;
                }
                std::cin.ignore();
            } while (type != 1 && type != 2);
            do {
                std::cout << "Nhap ma hang hoa: ";
                fflush(stdin);
                getline(std::cin, product_code);
            } while (product_code == "");
            res = this->select("products", "*", "WHERE code ='" + product_code + "'");
            if(!res->rowsCount()) {
                // Neu ma hang hoa khong ton tai thi hoi nguoi dung co muon tao moi khong
                char choice;
                std::cout << "Ma hang hoa khong ton tai. Ban co muon tao moi khong (y/n)?: ";
                std::cin >> choice;
                switch(choice) {
                    case 'y':
                        break;
                    case 'n':
                        return;
                    default:
                        return;
                }
                // this->product->newProduct(product_code);
                delete res;
            } else {
                res->first();
                // Neu hang hoa trong kho khong du de xuat hang
                int amount_left;
                do {
                    if(amount_left < 0) {
                        std::cout << "Hang trong kho khong du so luong yeu cau! (Con " << res->getInt("amount") << ")" << std::endl;
                    }
                    enter_amount:
                    std::cout << "Nhap so luong: ";
                    std::cin >> amount;
                    if(std::cin.fail()) {
                        std::cout << "Ban phai nhap so!" << std::endl;
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        goto enter_amount;
                    }
                    std::cin.ignore();
                    if(type == 1) {
                        amount_left = res->getInt("amount") + amount;
                    } else {
                        amount_left = res->getInt("amount") - amount;
                    }
                } while(amount_left < 0);
                if(this->create("(product_id, user_id, amount, type) VALUES('" + std::to_string(res->getInt("id")) + "', '" + std::to_string(this->auth->getId()) + "', '" + std::to_string(amount) + "', '" + std::to_string(type) + "')")) {
                    this->update("products", "amount = '" + std::to_string(amount_left) + "'", "id = " + std::to_string(res->getInt("id")));
                    std::cout << "Tao hoa don thanh cong!" << std::endl;
                } else {
                    std::cout << "Tao hoa don khong thanh cong. Vui long thu lai sau!" << std::endl; 
                }
                delete res;
            }
        }

        void edit() {
            int bill_id;
            enter_id:
            std::cout << ">> Nhap ID cua hoa don: ";
            std::cin >> bill_id;
            if(std::cin.fail()) {
                std::cout << "Ban phai nhap so!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                goto enter_id;
            }
            std::cin.ignore();
            // Lay ra du lieu cua don hang co vua nhap
            sql::ResultSet *res;
            res = this->select("bills.*, users.username as user_name, products.code as product_code, products.name as product_name, products.amount as amount_left", "LEFT JOIN products ON products.id = bills.product_id LEFT JOIN users ON users.id = bills.user_id WHERE bills.id = '" + std::to_string(bill_id) + "'");
            // Neu khong co don hang nao co id vua nhap thi thong bao loi va ket thuc
            if(!res->rowsCount()) {
                std::cout << "Hoan don khong ton tai!" << std::endl;
                return;
            }
            // Neu co hoa don co id vua nhap thi in ra
            res->first();
            std::cout << "===== Hoa Don Dang Duoc Chon =====" << std::endl;
            std::cout << "ID: " << res->getInt("id") << std::endl;
            std::cout << "Nguoi tao: " << res->getString("user_name") << std::endl;
            std::cout << "Ma hang hoa: " << res->getString("product_code") << std::endl;
            std::cout << "Ten hang hoa: " << res->getString("product_name") << std::endl;
            std::cout << "So luong: " << res->getInt("amount") << std::endl;
            std::cout << "Ngay tao:" << res->getString("created_at") << std::endl;
            std::cout << "===========================================" << std::endl;
            // Hoi xem co tiep tuc chinh sua hoa don khong
            enter_command:
            char choice;
            std::cout << ">> Ban co muon tiep tuc chinh sua hoa don khong? (y/n) : ";
            std::cin >> choice;
            std::cin.ignore();
            switch(choice) {
                case 'y':
                    break;
                case 'n':
                    delete res;
                    return;
                default:
                    goto enter_command;
                    break;
            }
            // Neu nguoi dung chon tiep tuc thi tien hanh qua trinh sua
            std::string product_code;
            int type;
            int amount;
            std::cout << "Nhap ma hang hoa (de trong neu khong thay doi): ";
            fflush(stdin);
            getline(std::cin, product_code);
            sql::ResultSet *newres;
            // Neu khong thay doi ma don hang
            if(product_code == "") {
                product_code = res->getString("product_code");
            }
            // Neu thay doi thi lay ra thong tin cua hang hoa moi
            newres = this->select("products", "*", "WHERE code = '" + product_code + "'");
            // Neu hang hoa moi khong ton tai
            if(!newres->rowsCount()) {
                std::cout << "Hang hoa khong ton tai!" << std::endl;
                delete newres;
                goto enter_command;
            }
            newres->first();
            // Nhap vao loai hoa don
            enter_type:
            std::cout << "Nhap loai hoa don ( 1 - Hoa don nhap, 2 - Hoa don xuat): ";
            std::cin >> type;
            if(std::cin.fail()) {
                std::cout << "Ban phai nhap so!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                goto enter_type;
            }
            std::cin.ignore();
            // Nhap vao so luong
            enter_amount:
            std::cout << "Nhap vao so luong (con trong kho: " << newres->getInt("amount") << "): ";
            std::cin >> amount;
            if(std::cin.fail()) {
                std::cout << "Ban phai nhap so!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                goto enter_amount;
            }
            std::cin.ignore();
            // Reset so luong hang hoa
            int tmp_amount;
            if(res->getInt("type") == 1) {
                tmp_amount = res->getInt("amount_left") - res->getInt("amount");
                if(tmp_amount < 0) {
                    std::cout << "So luong hang cu trong kho khong du do thay doi loai hoa don!" << std::endl;
                    goto enter_type;
                }
            } else {
                tmp_amount = res->getInt("amount_left") + res->getInt("amount");
            }
            this->update("products", "amount = '" + std::to_string(tmp_amount) + "'", "code = '" + res->getString("product_code") + "'");
            delete newres;
            newres = this->select("products", "*", "WHERE code = '" + product_code + "'");
            newres->first();
            // Cap nhat hoa don va san pham
            this->update("product_id = '" + std::to_string(newres->getInt("id")) + "', amount = '" + std::to_string(amount) + "', type = '" + std::to_string(type) + "'", "id = '" + std::to_string(bill_id) + "'");
            if(type == 1) {
                tmp_amount = newres->getInt("amount") + amount;
            } else {
                tmp_amount = newres->getInt("amount") - amount;
                if(tmp_amount < 0) {
                    std::cout << "So luong hang trong kho khong du!" << std::endl;
                    goto enter_amount;
                }
            }
            // Cap nhat hang moi thay doi
            std::cout << product_code << "\t" << tmp_amount << std::endl;
            if(this->update("products", "amount = '" + std::to_string(tmp_amount) + "'", "code = '" + product_code + "'")) {
                std::cout << "Sua hoa don thanh cong!" << std::endl;
            } else {
                std::cout << "Sua hoa don that bai. Vui long thu lai sau!" << std::endl;
            }
            delete res;
            delete newres;
        }

        // Xoa hang hoas
        void deleteItem() {
            int bill_id;
            // Nhap ma hang hoa de xoa
            enter_id:
            std::cout << ">> Nhap ma hoa don: ";
            std::cin >> bill_id;
            if(std::cin.fail()) {
                std::cout << "Ma hoa don chi bao gom so!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                goto enter_id;
            }
            std::cin.ignore();
            sql::ResultSet *res;
            res = this->select("bills.*, users.username as user_name, products.code as product_code, products.name as product_name, products.amount as amount_left", "LEFT JOIN products ON products.id = bills.product_id LEFT JOIN users ON users.id = bills.user_id WHERE bills.id = '" + std::to_string(bill_id) + "'");
            if(!res->rowsCount()) {
                // Neu hoa don khong ton tai thi bao loi
                std::cout << "** Hoa don khong ton tai!" << std::endl;
                return;
            }
            // Neu hang hoa ton tai thi dua ra thong tin hoa don
            res->first();
            std::cout << "===== Hoa Don Dang Duoc Chon =====" << std::endl;
            std::cout << "ID: " << res->getInt("id") << std::endl;
            std::cout << "Nguoi tao: " << res->getString("user_name") << std::endl;
            std::cout << "Ma hang hoa: " << res->getString("product_code") << std::endl;
            std::cout << "Ten hang hoa: " << res->getString("product_name") << std::endl;
            std::cout << "So luong: " << res->getInt("amount") << std::endl;
            std::cout << "Ngay tao:" << res->getString("created_at") << std::endl;
            std::cout << "===========================================" << std::endl;
            char choice;
            // Nguoi dung chon xoa hoa don hay khong
            enter_choice:
            std::cout << "Ban co chac muon xoa hoa don dang duoc chon khong (y/n)?: ";
            std::cin >> choice;
            if(std::cin.fail()) {
                std::cout << "** Lenh khong hop le. Nhap y (yes) hoac n (no)!" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                goto enter_choice;
            }
            std::cin.ignore();
            switch(choice) {
                case 'y':
                    // Yes thi tien hanh xoa hoa don
                    break;
                case 'n':
                    // No thi thoat khoi qua trinh xoa
                    return;
                default:
                    std::cout << "** Lenh khong hop le!" << std::endl;
                    goto enter_choice;
                    break;
            }
            // Tien hanh xoa hoa don
            // Update lai thong tin hang hoa trong kho
            int reset_amount = 0;
            if(res->getInt("type") == 1) {
                reset_amount = res->getInt("amount_left") - res->getInt("amount");
                if(reset_amount <= 0) {
                    std::cout << "Warning: So luong hanh trong kho khong du (con lai: " << std::to_string(res->getInt("amount_left")) << ")" << std::endl;
                    return;
                }
            } else {
                reset_amount = res->getInt("amount_left") + res->getInt("amount");
            }
            if(this->update("products", "amount = '" + std::to_string(reset_amount) + "'", "WHERE id = '" + std::to_string(res->getInt("product_id")) + "'")) {
                this->forceDelete("WHERE id = '" + std::to_string(res->getInt("id")) + "'");
                std::cout << "Xoa hoa don thanh cong thanh cong!" << std::endl;
            } else {
                std::cout << "Co loi xay ra vui long thu lai sau!" << std::endl;
            }
            delete res;
        }
};