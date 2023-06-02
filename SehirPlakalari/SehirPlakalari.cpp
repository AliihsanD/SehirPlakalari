
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

struct TreeNode {
    int plakaKod;
    string sehir;
    TreeNode* sol;
    TreeNode* sag;
};

void ekle(TreeNode*& root, int plakaKodu, const string& sehir) {
    if (root == nullptr) {
        root = new TreeNode;
        root->plakaKod = plakaKodu;
        root->sehir = sehir;
        root->sol = nullptr;
        root->sag = nullptr;
    }
    else if (plakaKodu < root->plakaKod) {
        ekle(root->sol, plakaKodu, sehir);
    }
    else if (plakaKodu > root->plakaKod) {
        ekle(root->sag, plakaKodu, sehir);
    }
}

void inorder(TreeNode* root) {      //Orta
    if (root != nullptr) {
        inorder(root->sol);
        cout << root->plakaKod << " ";
        inorder(root->sag);
    }
}

void preorder(TreeNode* root) {     //Baş
    if (root != nullptr) {
        cout << root->plakaKod << " ";
        preorder(root->sol);
        preorder(root->sag);
    }
}

void postorder(TreeNode* root) {        //Son
    if (root != nullptr) {
        postorder(root->sol);
        postorder(root->sag);
        cout << root->plakaKod << " ";
    }
}

int derinlik(TreeNode* root) {      //DerinlikAçma
    if (root == nullptr)
        return 0;
    else {
        int solDerinlik = derinlik(root->sol);
        int sagDerinlik = derinlik(root->sag);
        return max(solDerinlik, sagDerinlik) + 1;
    }
}

bool denge(TreeNode* root) {
    if (root == nullptr)
        return true;

    int solDerinlik = derinlik(root->sol);
    int sagDerinlik = derinlik(root->sag);

    if (abs(solDerinlik - sagDerinlik) <= 1 && denge(root->sol) && denge(root->sag))
        return true;

    return false;
}

int sehirFark(TreeNode* root, int plaka, int plaka2) {
    if (root == nullptr)
        return -1;

    if (plaka < root->plakaKod && plaka2 < root->plakaKod)
        return sehirFark(root->sol, plaka, plaka2);

    if (plaka > root->plakaKod && plaka2 > root->plakaKod)
        return sehirFark(root->sag, plaka, plaka2);

    int plaka1Duzey = derinlik(root) - derinlik(root->sol);
    int plaka2Duzey = derinlik(root) - derinlik(root->sag);

    return abs(plaka1Duzey - plaka2Duzey);
}

void Yazdir(TreeNode* root, int bosluk = 0) {       //indent = 0 parametre olarak yazınca hata vermiyor.
    if (root != nullptr) {
        Yazdir(root->sag, bosluk + 4);
        cout << string(bosluk, ' ');
        cout << root->plakaKod << endl;
        Yazdir(root->sol, bosluk + 4);
    }
}

int main() {
    TreeNode* root = nullptr;

    int plakaKod;
    string sehir;
    
    fstream dosya;
    dosya.open("C:\\sehir_plaka_kodlari.txt");

    if (!dosya)
    {
        cerr << "dosya acilamadi..." << endl;
        return 1;
    }

    string satir;
    while (getline(dosya, satir)) {
        stringstream ss(satir);
        string plaka;

        getline(ss, plaka, ','); // Virgülle ayrılan plaka bilgisini alır
        getline(ss, sehir, ' '); // Boşlukla ayrılan şehir bilgisini alır

        plakaKod = stoi(plaka);  //String veri tipini int veri tipine dönüştürür.

        ekle(root, plakaKod, sehir);
    }
    dosya.close();

    cout << "Agac: " << endl;
    Yazdir(root);

    cout << "Agacın derinligi: " << derinlik(root) << endl;

    if (denge(root))
        cout << "agac dengeli..." << endl;
    else
        cout << "agac dengesiz..." << endl;

    int plaka1, plaka2;
    cout << "1. plaka kodunu girin: ";
    cin >> plaka1;
    cout << "2. plaka kodunu girin: ";
    cin >> plaka2;

    cout << "1. plakanin sehri: ";
    TreeNode* plaka1Node = root;
    while (plaka1Node != nullptr && plaka1Node->plakaKod != plaka1) {
        if (plaka1 < plaka1Node->plakaKod)
            plaka1Node = plaka1Node->sol;
        else
            plaka1Node = plaka1Node->sag;
    }
    if (plaka1Node != nullptr)
        cout << plaka1Node->sehir << endl;
    else
        cout << "plaka kodu bulunamadi..." << endl;

    cout << "2. plakanin sehri: ";
    TreeNode* plaka2Node = root;
    while (plaka2Node != nullptr && plaka2Node->plakaKod != plaka2) {
        if (plaka2 < plaka2Node->plakaKod)
            plaka2Node = plaka2Node->sol;
        else
            plaka2Node = plaka2Node->sag;
    }
    if (plaka2Node != nullptr)
        cout << plaka2Node->sehir << endl;
    else
        cout << "plaka kodu bulunamadi..." << endl;

    int fark = sehirFark(root, plaka1, plaka2);
    if (fark != -1)
        cout << "Duzey farki: " << fark << endl;
    else
        cout << "plaka kodu bulunamadi..." << endl;

    cout << "Preorder siralamasi: ";
    preorder(root);
    cout << endl;

    cout << "Inorder siralamasi: ";
    inorder(root);
    cout << endl;

    cout << "Postorder siralamasi: ";
    postorder(root);
    cout << endl;

    return 0;
}