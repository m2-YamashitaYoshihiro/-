#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <tuple>
#include <sstream>
using namespace std;

//�p�X���[�h�����v���O�����p�̊֐�//

string generateRandomString(size_t size) {
    const string p = "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    random_device random;
    mt19937 mt(random()); 
    uniform_int_distribution<> dist(0, p.size() - 1); 

    string randomString;
    for (size_t i = 0; i < size; ++i) {
        randomString += p[dist(mt)];
    }

    return randomString;
}


std::tuple<string, string, string> MakePassword() {
    cout << "�T�C�g������͂��āCEnter�L�[�������Ă��������D\n";
    string site;
    cin >> site;
    cout << "ID����͂��āCEnter�L�[�������Ă��������D\n";
    string id;
    cin >> id;
    cout << "��������p�X���[�h�̕���������͂��āCEnter�L�[�������Ă��������D\n";
    int n;
    cin >> n;

    string pass = generateRandomString(n);
    cout << "\n�������ꂽ�p�X���[�h:\n" << pass << endl;

    return std::forward_as_tuple(site, id, pass);
}

void SavePassword(string site, string id, string pass) {
    ofstream outfile("Password.txt", ios_base::app);
    outfile << site << " " << id << " " << pass << endl;
    outfile.close();
    cout << "\n�p�X���[�h��ۑ����܂���.\n";
}

//�p�X���[�h�����v���O�����p�̊֐�//

class Password {
    string site;
    string id;
    string pass;

public:
    Password(const string& newSite, string newId, string newPass) : site(newSite), id(newId), pass(newPass) {}

    void setSite(const string& newSite) { site = newSite; }
    string getSite() const { return site; }
    void setId(const string& newId) { id = newId; }
    string getId() const { return id; }
    void setPass(const string& newPass) { pass = newPass; }
    string getPass() const { return pass; }

    bool contains(const string& sitename)const {
        return site.find(sitename) != string::npos ||
               id.find(sitename) != string::npos ||
               pass.find(sitename) != string::npos;
    }
};

string AskSitename() {

    cout << "��������T�C�g������͂��CEnter�L�[�������Ă�������\n";
    string sitename;
    cin >> sitename;

    return sitename;
}

vector<Password> SearchPass(const string& filename, const string& searchString) {
    ifstream file(filename);
    vector<Password> passwords;
    string line;

    if (!file.is_open()) {
        cerr << "�t�@�C�����J�����Ƃ��ł��܂���ł����B" << endl;
        return passwords;
    }

    while (getline(file, line)) {
        istringstream iss(line);
        string site, id, pass;
        if (iss >> site >> id >> pass) {
            passwords.emplace_back(site, id, pass);
        }
    }

    file.close();

    vector<Password> result;
    
    for (auto it = passwords.begin(); it != passwords.end(); ++it) {
        if (it->contains(searchString)) {
            result.push_back(*it);
        }
    }
    if (result.empty()) {
        throw runtime_error("��v����p�X���[�h������܂���D");
    }
    return result;
}

//main�֐�//

int main() {
    cout << "�p�X���[�h������make�C�p�X���[�h������find�Ɠ��͂��Ă��������D\n";
    string how;
    cin >> how;
    if (how == "make") {

        //�p�X���[�h�����v���O����

        string site, id, pass;
        tie(site, id, pass) = MakePassword();

        SavePassword(site, id, pass);
    }

    else if (how == "find") {

        //�p�X���[�h�����v���O����

        string sitename;
        vector<Password> passwords;
        sitename = AskSitename();

        string filename = "Password.txt";

        try {
            passwords = SearchPass(filename, sitename);

            for (const auto& password : passwords)
                cout << "\n[�ۑ����ꂽ�p�X���[�h]" << "\n�T�C�g��:" << password.getSite() << "\nID:" << password.getId() << "\n�p�X���[�h:" << password.getPass() << endl;
        }
        catch (exception& e) {
            cerr << endl << e.what() << endl;
        }
        

    }

    //make�Efind�ȊO�̕����񂪓��͂��ꂽ�ꍇ

    else {
        cout << "���͂��s���ł��D\n";
    }
}
