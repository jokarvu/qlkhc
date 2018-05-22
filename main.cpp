#include <iostream>
#include "Model.h"
#include "Auth.h"
#include "Product.h"
#include "Bill.h"
#include "User.h"
#include "Controller.h"

int main() {
    Controller *controller = new Controller;
    controller->login();
    controller->menu();
    return 0;
}