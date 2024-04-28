#include "HT_chat.h"
#include "iostream"

HashTable_chat::HashTable_chat() {
    //������ ����� ��������� ������ HashTable_chat � �������������� ��� ����������
    data_count = 0;
    data = nullptr;

    allocNewMem(8);
}
void HashTable_chat::reg(char _login[LOGINLENGTH], char _pass[], int pass_length) {//������������ ������ ������������
    uint* digest = sha1(_pass, pass_length);
    addinner(_login, digest);
    std::cout << " + ��������������� ����� ������������ � �������: " << _login << std::endl;
}
void HashTable_chat::unreg(char _login[LOGINLENGTH]) {//�������� ����������� ������������
    int index, i = 0;
    for (; i < mem_size; i++) {
        index = hash_func(_login, i * i);
        if (data[index].status == enPairStatus::free)
        {
            std::cout << endl << "!-������� ������ ����������� ������������ � �������: " << _login << endl
             << "--> ������������ � �������: " << _login << " �� ��������� !" << endl << endl;
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
    std::cout << "��������c� ����������� ������������ � �������: " << _login << " - ���������-" << std::endl;
}
bool HashTable_chat::login(char _login[LOGINLENGTH], char _pass[], int pass_length) {
    //������� ���������, ���� �� ���������� ����� ������� � ����� ������, ���������� � ������ ������� data. 
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
//��������� ���������� � ������������ � ������ ����� ������. 
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
//�������� ����� ������ ��� �������� ������ ���� AuthData.
void HashTable_chat::allocNewMem(int newMemSize) {
    mem_size = newMemSize;
    data = new Pair[mem_size];
}
//������ ���-������� ��� ������ login, ������� ���������� ���������� step
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

void HashTable_chat::resize() {//�������� ������ ������, ������������ ��� �������� ������
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
