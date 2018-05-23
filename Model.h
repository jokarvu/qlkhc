#include <iostream>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/connection.h>
#include <cppconn/resultset.h>
#include <cppconn/prepared_statement.h>

class Model {
    protected:
        sql::Driver *driver;
        sql::Connection *con;
        std::string table;
    public:
        Model() {
            this->driver = get_driver_instance();
            this->con = this->driver->connect("tcp://127.0.0.1:3306", "root", "b15dcpt082");
            this->con->setSchema("test");
        }
        ~Model() {
            delete this->con;
        }


        int create(std::string query) {
            try {
                sql::PreparedStatement *pt;
                pt = this->con->prepareStatement("INSERT INTO " + this->table + query);
                int tmp = pt->executeUpdate();
                delete pt;
                return tmp;
            } catch (sql::SQLException &e) {
                return 0;
            }
        }

        int create(std::string table_name, std::string query) {
            try {
                sql::PreparedStatement *pt;
                pt = this->con->prepareStatement("INSERT INTO " + table_name + query);
                int tmp = pt->executeUpdate();
                delete pt;
                return tmp;
            } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line "
                    << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() <<
                    " )" << std::endl;
                return 0;
            }
        }

        int update(std::string value, std::string cdtn) {
            try {
                sql::PreparedStatement *pt;
                pt = this->con->prepareStatement("UPDATE " + this->table + " SET " + value + " WHERE " + cdtn);
                int tmp = pt->executeUpdate();
                delete pt;
                return tmp;
            } catch (sql::SQLException &e) {
                return 0;
            }
        }

        int update(std::string table_name, std::string value, std::string cdtn) {
            try {
                sql::PreparedStatement *pt;
                pt = this->con->prepareStatement("UPDATE " + table_name + " SET " + value + " WHERE " + cdtn);
                int tmp = pt->executeUpdate();
                delete pt;
                return tmp;
            } catch (sql::SQLException &e) {
                return 0;
            }
        }

        sql::ResultSet *select(std::string column, std::string cdtn) {
            sql::PreparedStatement *pt;
            pt = this->con->prepareStatement("SELECT " + column + " FROM " + this->table + " " + cdtn);
            return pt->executeQuery();
        }

        sql::ResultSet *select(std::string table_name, std::string column, std::string cdtn) {
            sql::PreparedStatement *pt;
            pt = this->con->prepareStatement("SELECT " + column + " FROM " + table_name + " " + cdtn);
            return pt->executeQuery();
        }

        int forceDelete(std::string query) {
            try {
                sql::PreparedStatement *pt;
                pt = this->con->prepareStatement("DELETE FROM " + this->table + " WHERE " + query);
                int tmp = pt->executeUpdate();
                delete pt;
                return tmp;
            } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line "
                    << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() <<
                    " )" << std::endl;
                return 0;
            }
        }

        int forceDelete(std::string table_name, std::string query) {
            try {
                sql::PreparedStatement *pt;
                pt = this->con->prepareStatement("DELETE FROM " + table_name + " WHERE " + query);
                int tmp = pt->executeUpdate();
                delete pt;
                return tmp;
            } catch (sql::SQLException &e) {
                std::cout << "# ERR: SQLException in " << __FILE__;
                std::cout << "(" << __FUNCTION__ << ") on line "
                    << __LINE__ << std::endl;
                std::cout << "# ERR: " << e.what();
                std::cout << " (MySQL error code: " << e.getErrorCode();
                std::cout << ", SQLState: " << e.getSQLState() <<
                    " )" << std::endl;
                return 0;
            }
        }
};