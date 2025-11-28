#pragma once
#include <string>

namespace Banking {
    struct Address {
        std::string street;
        std::string city;
        std::string country;
        int post_id;

        Address(const std::string& street_value, const std::string& city_value, const std::string& country_value, int post_id_value)
            : street(street_value), city(city_value), country(country_value), post_id(post_id_value) {
        }

        // Геттеры для Address
        std::string getStreet() const { return street; }
        std::string getCity() const { return city; }
        std::string getCountry() const { return country; }
        int getPostId() const { return post_id; }

        // Сеттеры для Address
        void setStreet(const std::string& street_value) { street = street_value; }
        void setCity(const std::string& city_value) { city = city_value; }
        void setCountry(const std::string& country_value) { country = country_value; }
        void setPostId(int post_id_value) { post_id = post_id_value; }

        // Метод для отображения адреса
        std::string getFullAddress() const {
            return street + ", " + city + ", " + country + ", " + std::to_string(post_id);
        }
    };

    struct Date {
        int day;
        int month;
        int year;

        Date(int day_value, int month_value, int year_value)
            : day(day_value), month(month_value), year(year_value) {
        }

        // Геттеры для Date
        int getDay() const { return day; }
        int getMonth() const { return month; }
        int getYear() const { return year; }

        // Сеттеры для Date
        void setDay(int day_value) { day = day_value; }
        void setMonth(int month_value) { month = month_value; }
        void setYear(int year_value) { year = year_value; }

        // Метод для отображения даты
        std::string getFormattedDate() const {
            return std::to_string(day) + "." + std::to_string(month) + "." + std::to_string(year);
        }

        // Проверка валидности даты
        bool isValid() const {
            return day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 1900;
        }
    };
}
