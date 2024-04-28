// Main function of the C++ program.
#include <iostream>
#include <string.h>
#include "HT_chat.h"
#include "sha1.h"

int main()
{
 
    setlocale(LC_CTYPE, "rus");
    HashTable_chat chat_test;
    std::cout << "Тест 1: в чате регистрируют трёх пользователей (задают 3 пары значений  логин-пароль)" << endl;
        chat_test.reg((char*)"Igor", (char*)"rtyq54321", sizeof("rtyq54321"));
        chat_test.reg((char*)"Elena", (char*)"dsagd", sizeof("dsagd"));
        chat_test.reg((char*)"Anton", (char*)"artsdrt", sizeof("artsdrt"));
        std::cout << endl;
    std::cout << "Тест 2: Четыре запроса на отмену регистрации пользователя по логину (2 запроса на несуществующие логин-пароль)" << endl << endl;
        chat_test.unreg((char*)"Igor");
        chat_test.unreg((char*)"Misha");
        chat_test.unreg((char*)"Anton");
        chat_test.unreg((char*)"03us");
 
    return 0;
}
