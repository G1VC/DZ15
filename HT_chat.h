#pragma once
#include "sha1.h"
#include "string.h"
#define SIZE 10
#define LOGINLENGTH 10

class HashTable_chat {
public:
    HashTable_chat();

    void reg(char _login[LOGINLENGTH], char _pass[], int pass_length);
    bool login(char _login[LOGINLENGTH], char _pass[], int pass_length);
    void unreg(char _login[LOGINLENGTH]);

    friend void test(HashTable_chat& c);
private:

    enum enPairStatus {//статус пары ключ-значение
        free,// свободен
        engaged, // занят
        deleted//удален
    };

    struct Pair { //структура : пара ключ-значение


        Pair() :
            login(""),
            pass_sha1_hash(0),
            status(enPairStatus::free) {
        }
        ~Pair() {
            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
        }
        // этот код создаёт структуру AuthData и инициализирует её поля. 
        Pair(char _login[LOGINLENGTH], uint* sh1) {
            memcpy(login, _login, LOGINLENGTH);
            pass_sha1_hash = sh1;
            status = enPairStatus::engaged;
        }
        // операция присваивания для объекта типа AuthData:данные для аутентификации AuthData в виде пары логин/хеш от пароля.
        Pair& operator = (const Pair& other) {
            memcpy(login, other.login, LOGINLENGTH);

            if (pass_sha1_hash != 0)
                delete[] pass_sha1_hash;
            pass_sha1_hash = new uint[SHA1HASHLENGTHUINTS];

            memcpy(pass_sha1_hash, other.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            status = other.status;

            return *this;
        }
        char login[LOGINLENGTH];
        uint* pass_sha1_hash;

        enPairStatus status;
    };

    void resize();
    void allocNewMem(int newMemSize);
    int hash_func(char login[LOGINLENGTH], int step);
    int hf_multiply(int val);
    void addinner(char login[LOGINLENGTH], uint* digest);

    Pair* data;
    int data_count;
    int mem_size;
};
