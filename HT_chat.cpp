#include "HT_chat.h"
#include "iostream"

HashTable_chat::HashTable_chat() {
    //создаЄт новый экземпл€р класса HashTable_chat и инициализирует его переменные
    data_count = 0;
    data = nullptr;

    allocNewMem(8);
}
void HashTable_chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {//регистрирует нового пользовател€
    uint* digest = sha1(_pass, pass_length);
    addinner(_login, digest);
    std::cout << " + зарегистрирован новый пользователь с логином: " << _login << std::endl;
}
void HashTable_chat::unreg(char _login[LOGINLENGTH]) {//отмен€ет регистрацию пользовател€
    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(_login, i * i);
        if (data[index].status == enPairStatus::free)
        {
            std::cout << endl << "!-попытка отмены регистрации пользовател€ с логином: " << _login << endl
             << "--> пользователь с логином: " << _login << " не обнаружен !" << endl << endl;
            return;   
        }
        else if (data[index].status == enPairStatus::engaged
            && !memcmp(_login, data[index].login, LOGINLENGTH))
            break;
    }
    
    if (i >= mem_size)
    {
             return;
    }

    data[index].status = enPairStatus::deleted;
    std::cout << "отмен€етc€ регистраци€ пользовател€ с логином: " << _login << " - ¬ыполнено-" << std::endl;
}
bool HashTable_chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    //‘ункци€ провер€ет, есть ли совпадение между паролем и хешем парол€, сохранЄнным в €чейке массива data. 
    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(_login, i * i);
        if (data[index].status == enPairStatus::free)
            return false;
        else if (data[index].status == enPairStatus::engaged
            && !memcmp(_login, data[index].login, LOGINLENGTH))
            break;
    }
    if (i >= mem_size) return false;
    std::cout << "Propbs count: " << i + 1 << std::endl;

    uint* digest = sha1(_pass, pass_length);

    bool cmpHashes = !memcmp(
        data[index].pass_sha1_hash,
        digest,
        SHA1HASHLENGTHBYTES);
    delete[] digest;

    return cmpHashes;
}
//добавл€ет информацию о пользователе в список €чеек пам€ти. 
void HashTable_chat::addinner(char login[LOGINLENGTH], uint* digest) {
    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(login, i * i);
        if (data[index].status == enPairStatus::free)
            break;
    }
    if (i >= mem_size)
    {
        resize();
        addinner(login, digest);
    }
    else {
        data[index] = Pair(login, digest);
        data_count++;
    }
}
//выдел€ет новую пам€ть дл€ хранени€ данных типа AuthData.
void HashTable_chat::allocNewMem(int newMemSize) {
    mem_size = newMemSize;
    data = new Pair[mem_size];
}
//создаЄт хеш-функцию дл€ логина login, котора€ использует переменную step
int HashTable_chat::hash_func(char login[LOGINLENGTH], int step) {
    long sum = 0;
    for (int i = 0; i < LOGINLENGTH; i++) {
        sum += login[i];
    }
    return (hf_multiply(sum) + step) % mem_size;
}

int HashTable_chat::hf_multiply(int val) {
    const double A = 0.7;
    return int(mem_size * (A * val - int(A * val)));
}

void HashTable_chat::resize() {//измен€ет размер пам€ти, используемой дл€ хранени€ данных
    std::cout << "resize()" << endl;
    Pair* save = data;
    int save_ms = mem_size;

    allocNewMem(mem_size * 2);
    data_count = 0;

    for (int i = 0; i < save_ms; i++) {
        Pair& old_data = save[i];
        if (old_data.status == enPairStatus::engaged) {

            uint* sha_hash_copy = new uint[SHA1HASHLENGTHUINTS];
            memcpy(sha_hash_copy, old_data.pass_sha1_hash, SHA1HASHLENGTHBYTES);

            addinner(old_data.login, sha_hash_copy);
        }
    }

    delete[] save;
}
