#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <tuple>
#include <sstream>
using namespace std;

//パスワード生成プログラム用の関数//

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
    cout << "サイト名を入力して，Enterキーを押してください．\n";
    string site;
    cin >> site;
    cout << "IDを入力して，Enterキーを押してください．\n";
    string id;
    cin >> id;
    cout << "生成するパスワードの文字数を入力して，Enterキーを押してください．\n";
    int n;
    cin >> n;

    string pass = generateRandomString(n);
    cout << "\n生成されたパスワード:\n" << pass << endl;

    return std::forward_as_tuple(site, id, pass);
}

void SavePassword(string site, string id, string pass) {
    ofstream outfile("Password.txt", ios_base::app);
    outfile << site << " " << id << " " << pass << endl;
    outfile.close();
    cout << "\nパスワードを保存しました.\n";
}

//パスワード検索プログラム用の関数//

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

    cout << "検索するサイト名を入力し，Enterキーを押してください\n";
    string sitename;
    cin >> sitename;

    return sitename;
}

vector<Password> SearchPass(const string& filename, const string& searchString) {
    ifstream file(filename);
    vector<Password> passwords;
    string line;

    if (!file.is_open()) {
        cerr << "ファイルを開くことができませんでした。" << endl;
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
        throw runtime_error("一致するパスワードがありません．");
    }
    return result;
}

//main関数//

int main() {
    cout << "パスワード生成はmake，パスワード検索はfindと入力してください．\n";
    string how;
    cin >> how;
    if (how == "make") {

        //パスワード生成プログラム

        string site, id, pass;
        tie(site, id, pass) = MakePassword();

        SavePassword(site, id, pass);
    }

    else if (how == "find") {

        //パスワード検索プログラム

        string sitename;
        vector<Password> passwords;
        sitename = AskSitename();

        string filename = "Password.txt";

        try {
            passwords = SearchPass(filename, sitename);

            for (const auto& password : passwords)
                cout << "\n[保存されたパスワード]" << "\nサイト名:" << password.getSite() << "\nID:" << password.getId() << "\nパスワード:" << password.getPass() << endl;
        }
        catch (exception& e) {
            cerr << endl << e.what() << endl;
        }
        

    }

    //make・find以外の文字列が入力された場合

    else {
        cout << "入力が不正です．\n";
    }
}
