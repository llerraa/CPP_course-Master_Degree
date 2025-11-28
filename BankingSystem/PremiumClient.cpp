#include "PremiumClient.h"
#include <stdexcept>
#include <iostream>
#include "Account.h"

namespace Banking {

    // Конструктор
    PremiumClient::PremiumClient(int id_value, const std::string& name_value, const std::string& surname_value,
        const Address& address_value, const Date& date_value,
        const std::string& level, double discount)
        : Client(id_value, name_value, surname_value, address_value, date_value) {

        setPremiumLevel(level); // сеттер для установки уровня
        std::cout << "PremiumClient constructor called for: " << getSurname() << " with level: " << premium_level << std::endl;
    }

    PremiumClient::~PremiumClient() {
        std::cout << "PremiumClient destructor called for: " << getSurname() << std::endl;
    }

    // Сеттер для уровня премиума с проверкой
    void PremiumClient::setPremiumLevel(const std::string& level) {
        if (level != "Silver" && level != "Gold" && level != "Platinum") {
            throw std::invalid_argument("Invalid premium level. Must be Silver, Gold, or Platinum");
        }
        premium_level = level;
        
        if (premium_level == "Silver") {
            discount_percentage = 5.0;}
        else if (premium_level == "Gold") {
            discount_percentage = 10.0;}
        else if (premium_level == "Platinum") {
            discount_percentage = 15.0;}

        std::cout << "Premium level set to: " << premium_level << " with " << discount_percentage << "% discount" << std::endl;
    }

    // Сеттер для процента скидки с проверкой
    void PremiumClient::setDiscountPercentage(double discount) {
        if (discount < 0 || discount > 50) {
            throw std::invalid_argument("Discount percentage must be between 0 and 50");
        }
        discount_percentage = discount;
        std::cout << "Discount percentage updated to: " << discount_percentage
            << "% for client: " << getSurname() << std::endl;
    }

    // Переопределенный метод отображения информации
    void PremiumClient::displayinfo() const {
        Client::displayinfo();
        std::cout << "Client Type: Premium" << std::endl;
        std::cout << "Premium Level: " << premium_level << std::endl;
        std::cout << "Discount Percentage: " << discount_percentage << "%" << std::endl;
    }

    // Применяет скидку к сумме
    void PremiumClient::applyDiscount(double& amount) const {
        double discount_amount = amount * (discount_percentage / 100.0);
        amount -= discount_amount;
        std::cout << "Applied " << discount_percentage << "% discount. New amount: " << amount
            << " (saved: " << discount_amount << ")" << std::endl;
    }

    // Повышает уровень премиума
    void PremiumClient::upgradeLevel() {
        if (premium_level == "Silver") {
            premium_level = "Gold";
            discount_percentage = 10.0;
        }
        else if (premium_level == "Gold") {
            premium_level = "Platinum";
            discount_percentage = 15.0;
        }
        else {
            std::cout << "Client " << getSurname() << " already has the highest premium level." << std::endl;
            return;
        }
        std::cout << "Client " << getSurname() << " upgraded to " << premium_level
            << " level with " << discount_percentage << "% discount." << std::endl;
    }
}