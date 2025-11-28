#pragma once
#include "Client.h"
#include <string>

namespace Banking {

    class PremiumClient : public Client {
    private:
        std::string premium_level; // "Silver", "Gold", "Platinum"
        double discount_percentage; // процент скидки

    public:
        PremiumClient(int id_value, const std::string& name_value, const std::string& surname_value, const Address& address_value, const Date& date_value, const std::string& level = "Silver", double discount = 5.0);
        virtual ~PremiumClient();

        // Геттеры для дополнительных атрибутов
        std::string getPremiumLevel() const { return premium_level; }
        double getDiscountPercentage() const { return discount_percentage; }

        // Сеттеры для дополнительных атрибутов
        void setPremiumLevel(const std::string& level);
        void setDiscountPercentage(double discount);

        // Переопределение виртуальных методов
        virtual void displayinfo() const override;

        // Дополнительные методы, специфичные для премиум-клиентов
        void applyDiscount(double& amount) const; // применяет скидку к сумме
        void upgradeLevel(); // повышает уровень премиума
    };

}
