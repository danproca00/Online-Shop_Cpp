#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#include <windows.h>

#include "order.hpp"

#include "product.hpp"
#include "alimentaryproduct.hpp"
#include "nonalimentaryproduct.hpp"

/// VARS
/// mai jos sunt variabilele globale pentru tot programul
std::vector<Product*> produse_magazin; /// produsele din magazin (care nu sunt nici alimentare nici nealimentare)
std::vector<Product*> produse_alimentare_magazin; /// produsele alimentare din magazin
std::vector<Product*> produse_nealimentare_magazin; /// produsele nealimentare din magazin

std::vector<Product*> produse_cos; /// produsele din cos

std::vector<Order*> comenzi; /// comenzile existente

std::map<std::string, std::string> users; /// o mapare de username spre parola
std::string customer; /// clientul curent
/// END VARS

enum Color {
    RED, GREEN, WHITE, BLUE, YELLOW
}; /// enumerare pentru culorile folosite in consola

/// HELPER FUNCTIONS
template<typename T>
void display_vector(std::vector<T> vec) {
    for (const auto& it : vec) {
        std::cout << it << std::endl;
    }
}

void set_color(Color color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int desired_color;
    switch (color) {
    case Color::RED:
        desired_color = 12;
        break;
    case Color::GREEN:
        desired_color = 10;
        break;
    case Color::BLUE:
        desired_color = 11;
        break;
    case Color::YELLOW:
        desired_color = 14;
        break;
    default:
        desired_color = 15;
    }
    SetConsoleTextAttribute(hConsole, desired_color);
}

bool is_number(std::string str) {
    if (str.size() == 0)
        return false;
    for (const auto& it : str) {
        if (it - '0' < 0 || it - '0' > 9) {
            return false;
        }
    }
    return true;
}

bool does_product_exist(std::string barcode, std::vector<Product*> products) {
    return std::find_if(products.begin(), products.end(), [barcode] (Product* p) {
        return p->getBarcode() == barcode;
    }) != products.end();
}

Product* get_product_by_barcode(std::string barcode, std::vector<Product*> products) {
    return *std::find_if(products.begin(), products.end(), [barcode] (Product* p) {
        return p->getBarcode() == barcode;
    });
}

void remove_product_by_barcode(std::string barcode, std::vector<Product*>& products) {
    products.erase(std::remove_if(products.begin(), products.end(), [barcode] (Product* p) {
        return p->getBarcode() == barcode;
    }), products.end());
}

std::vector<Product*> get_toate_produsele_magazin() {
    std::vector<Product*> produse_mag(produse_magazin.size() +
                                      produse_alimentare_magazin.size() +
                                      produse_nealimentare_magazin.size());
    int current_index = 0;
    for (int i = 0; i < produse_magazin.size(); i++, current_index++) {
        produse_mag[current_index] = produse_magazin[i];
    }
    for (int i = 0; i < produse_alimentare_magazin.size(); i++, current_index++) {
        produse_mag[current_index] = produse_alimentare_magazin[i];
    }
    for (int i = 0; i < produse_nealimentare_magazin.size(); i++, current_index++) {
        produse_mag[current_index] = produse_nealimentare_magazin[i];
    }
    return produse_mag;
}

void top_border(std::string text) {
    for (int i = 0; i < (60 - text.size()) / 2; i++) {
        std::cout << '=';
    }
    set_color(Color::RED);
    std::cout << ' ' << text << ' ';
    set_color(Color::WHITE);
    for (int i = 0; i < (60 - text.size()) / 2; i++) {
        std::cout << '=';
    }
    std::cout << std::endl;
}
/// END HELPER FUNCTIONS

/// SAVES
void save_products_in_cos() { /// functie pentru salvare produse din cos
    std::ofstream fout("files/cos.txt");
    for (const auto& product : produse_cos) { /// pentru fiecare produs, afisam detaliile in functie de tipul acestuia
        if(AlimentaryProduct* p = dynamic_cast<AlimentaryProduct*>(product)) {
            fout << "alimentaryproduct\n";
            fout << p->getName() << std::endl;
            fout << p->getBarcode() << std::endl;
            fout << p->getPrice() << ' ' << p->getAmount() << ' ';
            fout << p->getCalories() << ' ' << p->getExpiration().getDay()
                << ' ' << p->getExpiration().getMonth() << ' ' << p->getExpiration().getYear() << std::endl;
        } else if (NonalimentaryProduct* p = dynamic_cast<NonalimentaryProduct*>(product)) {
            fout << "nonalimentaryproduct\n";
            fout << p->getName() << std::endl;
            fout << p->getBarcode() << std::endl;
            fout << p->getPrice() << ' ' << p->getAmount() << std::endl;
            fout << p->getInstructions() << std::endl;
        } else {
            fout << "product\n";
            fout << product->getName() << std::endl;
            fout << product->getBarcode() << std::endl;
            fout << product->getPrice() << ' ' << product->getAmount() << std::endl;
        }
    }
    fout.close();
}

void save_products_in_stoc() { /// functie pentru salvare produse din stoc magazin, similar cu cos
    std::ofstream fout("files/magazin.txt");
    for (const auto& product : get_toate_produsele_magazin()) {
        if(AlimentaryProduct* p = dynamic_cast<AlimentaryProduct*>(product)) {
            fout << "alimentaryproduct\n";
            fout << p->getName() << std::endl;
            fout << p->getBarcode() << std::endl;
            fout << p->getPrice() << ' ' << p->getAmount() << ' ';
            fout << p->getCalories() << ' ' << p->getExpiration().getDay()
                << ' ' << p->getExpiration().getMonth() << ' ' << p->getExpiration().getYear() << std::endl;
        } else if (NonalimentaryProduct* p = dynamic_cast<NonalimentaryProduct*>(product)) {
            fout << "nonalimentaryproduct\n";
            fout << p->getName() << std::endl;
            fout << p->getBarcode() << std::endl;
            fout << p->getPrice() << ' ' << p->getAmount() << std::endl;
            fout << p->getInstructions() << std::endl;
        } else {
            fout << "product\n";
            fout << product->getName() << std::endl;
            fout << product->getBarcode() << std::endl;
            fout << product->getPrice() << ' ' << product->getAmount() << std::endl;
        }
    }
    fout.close();
}

void save_comenzi() { /// functie pentru salvarea comenzilor existente in fisier
    std::ofstream fout("files/comenzi.txt");
    for (const auto& o : comenzi) {
        fout << "order" << std::endl;
        fout << o->getCustomer() << std::endl;
        fout << o->getDate().getDay() << ' ' << o->getDate().getMonth() << ' ' << o->getDate().getYear() << std::endl;
        fout << o->getProducts().size() << std::endl;
        for (const auto& product : o->getProducts()) {
            if(AlimentaryProduct* p = dynamic_cast<AlimentaryProduct*>(product)) {
                fout << "alimentaryproduct\n";
                fout << p->getName() << std::endl;
                fout << p->getBarcode() << std::endl;
                fout << p->getPrice() << ' ' << p->getAmount() << ' ';
                fout << p->getCalories() << ' ' << p->getExpiration().getDay()
                    << ' ' << p->getExpiration().getMonth() << ' ' << p->getExpiration().getYear() << std::endl;
            } else if (NonalimentaryProduct* p = dynamic_cast<NonalimentaryProduct*>(product)) {
                fout << "nonalimentaryproduct\n";
                fout << p->getName() << std::endl;
                fout << p->getBarcode() << std::endl;
                fout << p->getPrice() << ' ' << p->getAmount() << std::endl;
                fout << p->getInstructions() << std::endl;
            } else {
                fout << "product\n";
                fout << product->getName() << std::endl;
                fout << product->getBarcode() << std::endl;
                fout << product->getPrice() << ' ' << product->getAmount() << std::endl;
            }
        }
    }
    fout.close();
}
/// END SAVES

/// STOC MAGAZIN
void load_products_in_stoc() { /// incarcarea produselor existente in stoc magazin
    std::ifstream fin("files/magazin.txt");
    std::ofstream log("files/log.txt", std::ios::app);
    std::string type;
    std::set<std::string> existing_products;
    while (std::getline(fin, type)) {
        if (type == "product") {
            Product* p = new Product();
            fin >> *p;
            if (existing_products.find(p->getBarcode()) == existing_products.end()) {
                produse_magazin.push_back(p);
                existing_products.insert(p->getBarcode());
            } else {
                log << "Couldn't load *" << *p << "* because a product with the same barcode exists." << std::endl;
            }
        } else if (type == "alimentaryproduct") {
            AlimentaryProduct* p = new AlimentaryProduct();
            fin >> *p;
            if (existing_products.find(p->getBarcode()) == existing_products.end()) {
                produse_alimentare_magazin.push_back(p);
                existing_products.insert(p->getBarcode());
            } else {
                log << "Couldn't load *" << *p << "* because a product with the same barcode exists." << std::endl;
            }
        } else if (type == "nonalimentaryproduct") {
            NonalimentaryProduct* p = new NonalimentaryProduct();
            fin >> *p;
            if (existing_products.find(p->getBarcode()) == existing_products.end()) {
                produse_nealimentare_magazin.push_back(p);
                existing_products.insert(p->getBarcode());
            } else {
                log << "Couldn't load *" << *p << "* because a product with the same barcode exists." << std::endl;
            }
        }
    }
    fin.close();
    log.close();
}

void meniu_stoc_magazin_produse(std::vector<Product*> produse, std::string title) {
    while (true) {
        std::string choice;
        while (choice != "a" && choice != "q") {
            system("cls");
            top_border(title);
            for (const auto& it : produse) {
                std::cout << *it << std::endl;
            }
            std::cout << "* ";
            set_color(Color::GREEN);
            std::cout << "a";
            set_color(Color::WHITE);
            std::cout << " - adaugare in cos *, *";
            set_color(Color::GREEN);
            std::cout << "q";
            set_color(Color::WHITE);
            std::cout << " - inapoi*" << std::endl;
            std::cout << "Introdu optiunea dorita: ";
            std::getline(std::cin, choice);
        }
        if (choice == "a") { /// adaugare in cos
            bool product_exists = false;
            std::string barcode;

            /// citim codul de bare al unui produs pana acesta este corect
            while (!product_exists && barcode != "exit") {
                std::cout << "Introdu codul de bare al produsului dorit [sau exit]: ";
                std::getline(std::cin, barcode);

                product_exists = does_product_exist(barcode, produse);
            }

            if (barcode != "exit") {
                Product* p = get_product_by_barcode(barcode, produse);

                /// in cazul in care cantitatea este 0, afisam un mesaj ca nu mai e pe stoc
                /// produsele vandute nu se sterg ci au cantitate 0
                if (p->getAmount() == 0) {
                    std::cout << "Produsul ";
                    set_color(Color::YELLOW);
                    std::cout << p->getName();
                    set_color(Color::WHITE);
                    std::cout << " nu mai este pe stoc!" << std::endl;
                    system("pause");
                    continue;
                }

                /// citim cantitatea produsului pe care vrem sa o adaugam in cos
                int amount;
                do {
                    std::cout << "Introdu cantitatea dorita pentru ";
                    set_color(Color::YELLOW);
                    std::cout << p->getName() << " [ 1 - " << p->getAmount() << " ]";
                    set_color(Color::WHITE);
                    std::cout << ": ";
                    std::cin >> amount;
                } while (amount <= 0 || amount > p->getAmount());

                p->setAmount(p->getAmount() - amount);

                /// daca produsul nu exista deja in cos
                if (!does_product_exist(barcode, produse_cos)) {
                    /// creem unul nou si-l adaugam
                    produse_cos.push_back(p->make_copy());
                    produse_cos[produse_cos.size() - 1]->setAmount(amount);
                } else {
                    /// altfel actualizam cantitatea produsului din cos
                    Product* pcos = get_product_by_barcode(barcode, produse_cos);
                    pcos->setAmount(pcos->getAmount() + amount);
                }
            }
        } else if (choice == "q") {
            return;
        }
        save_products_in_cos();
        save_products_in_stoc();
    }
}

void meniu_stoc_magazin() {
    std::vector<std::string> mprincipal = {
        "Produse",
        "Produse alimentare",
        "Produse nealimentare",
        "Inapoi"
    };
    while (true) {
        std::string choice;
        do {
            system("cls");
            top_border("MENIU STOC MAGAZIN");
            display_vector(mprincipal);
            std::cout << "Introdu optiunea dorita [1 - " << mprincipal.size() << "]: ";
            std::getline(std::cin, choice);
        } while(!is_number(choice) ||
                std::stoi(choice) == 0 ||
                std::stoi(choice) > mprincipal.size());
        switch (std::stoi(choice)) {
        case 1:
            meniu_stoc_magazin_produse(produse_magazin, "PRODUSE");
            break;
        case 2:
            meniu_stoc_magazin_produse(produse_alimentare_magazin, "PRODUSE ALIMENTARE");
            break;
        case 3:
            meniu_stoc_magazin_produse(produse_nealimentare_magazin, "PRODUSE NEALIMENTARE");
            break;
        case 4:
            return;
        }
    }
}
/// END STOC MAGAZIN

/// COS CURENT
void load_products_in_cos() { /// incarcarea produselor existente in cos
    std::ifstream fin("files/cos.txt");
    std::ofstream log("files/log.txt", std::ios::app);
    std::string type;
    std::set<std::string> existing_products;
    while (std::getline(fin, type)) {
        if (type == "product") {
            Product* p = new Product();
            fin >> *p;
            if (existing_products.find(p->getBarcode()) == existing_products.end()) {
                produse_cos.push_back(p);
                existing_products.insert(p->getBarcode());
            } else {
                log << "Couldn't load *" << *p << "* because a product with the same barcode exists." << std::endl;
            }
        } else if (type == "alimentaryproduct") {
            AlimentaryProduct* p = new AlimentaryProduct();
            fin >> *p;
            if (existing_products.find(p->getBarcode()) == existing_products.end()) {
                produse_cos.push_back(p);
                existing_products.insert(p->getBarcode());
            } else {
                log << "Couldn't load *" << *p << "* because a product with the same barcode exists." << std::endl;
            }
        } else if (type == "nonalimentaryproduct") {
            NonalimentaryProduct* p = new NonalimentaryProduct();
            fin >> *p;
            if (existing_products.find(p->getBarcode()) == existing_products.end()) {
                produse_cos.push_back(p);
                existing_products.insert(p->getBarcode());
            } else {
                log << "Couldn't load *" << *p << "* because a product with the same barcode exists." << std::endl;
            }
        }
    }
    fin.close();
    log.close();
}

void meniu_cos_curent() {
    while (true) {
        std::string choice;
        while (choice != "a" && choice != "q" && choice != "c" && choice != "d") {
            system("cls");
            top_border("COS CURENT");
            for (const auto& it : produse_cos) {
                std::cout << *it << std::endl;
            }
            std::cout << std::endl;
            std::cout << "Total produse: ";
            set_color(Color::BLUE);
            std::cout << produse_cos.size() << std::endl;
            set_color(Color::WHITE);
            std::cout << "Total cos: ";
            set_color(Color::BLUE);
            std::cout << std::accumulate(
                produse_cos.begin(),
                produse_cos.end(),
                0,
                [](double acc, Product* p){ return acc + p->getPrice() * p->getAmount();}
            );
            set_color(Color::WHITE);
            std::cout << " RON" << std::endl << std::endl;

            std::cout << "* ";
            set_color(Color::GREEN);
            std::cout << "a";
            set_color(Color::WHITE);
            std::cout << " - actualizare cantitate *, * ";
            set_color(Color::GREEN);
            std::cout << "q";
            set_color(Color::WHITE);
            std::cout << " - inapoi*" << std::endl;
            std::cout << "* ";
            set_color(Color::GREEN);
            std::cout << "c";
            set_color(Color::WHITE);
            std::cout << " - plaseaza comanda *, * ";
            set_color(Color::GREEN);
            std::cout << "d";
            set_color(Color::WHITE);
            std::cout << " - sterge produs *" << std::endl;
            std::cout << "Introdu optiunea dorita: ";
            std::getline(std::cin, choice);
        }
        if (choice == "a") { /// actualizare cantitate
            bool product_exists = false;
            std::string barcode;

            /// citim codul de bare al produsului dorit pana acesta e corect
            while (!product_exists && barcode != "exit") {
                std::cout << "Introdu codul de bare al produsului dorit [sau exit]: ";
                std::getline(std::cin, barcode);

                product_exists = does_product_exist(barcode, produse_cos);
            }

            if (barcode != "exit") {
                Product* p = get_product_by_barcode(barcode, produse_cos);
                Product* pm = get_product_by_barcode(barcode, get_toate_produsele_magazin());

                /// citm cantitatea noua dorita pana cand aceasta e una valida
                int amount;
                do {
                    std::cout << "Introdu cantitatea actualizata pentru ";
                    set_color(Color::YELLOW);
                    std::cout << p->getName() << " [ 1 - " << p->getAmount() + pm->getAmount() << " ]";
                    set_color(Color::WHITE);
                    std::cout << ": ";
                    std::cin >> amount;
                } while (amount <= 0 || amount > p->getAmount() + pm->getAmount());

                /// pm -> produs din magazin
                /// p -> produs din cos
                pm->setAmount(pm->getAmount() + (p->getAmount() - amount)); /// calculam noua cantitate din magazin
                p->setAmount(amount); /// setam cantitatea nou dorita
            }
        } else if (choice == "c") { /// cazul in care dorim sa plasam comanda
            if (produse_cos.size() != 0) {
                time_t now = time(0);
                tm *ltm = localtime(&now); /// preluam data curenta de pe calculator

                Date d(ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
                Order* o = new Order(produse_cos, d, customer); /// si creem comanda pentru data respectiva
                comenzi.push_back(o); /// si o adaugam in vectorul de comenzi
                produse_cos.clear(); /// odata plasata comanda, cosul se goleste
                save_comenzi(); /// salvam noua comanda
                system("cls");
                std::cout << "Comanda a fost plasata cu succes!" << std::endl << std::endl;
                std::cout << *o << std::endl; /// afisam comanda plasata
                system("pause");
            }
        } else if (choice == "d") { /// daca dorim sa stergem un produs din cos
            bool product_exists = false;
            std::string barcode;

            while (!product_exists && barcode != "exit") {
                std::cout << "Introdu codul de bare al produsului dorit [sau exit]: ";
                std::getline(std::cin, barcode);

                product_exists = does_product_exist(barcode, produse_cos);
            } /// asteptam sa se introduca codul de bare corect

            if (barcode != "exit") {
                Product* p = get_product_by_barcode(barcode, produse_cos);
                Product* pm = get_product_by_barcode(barcode, get_toate_produsele_magazin());

                /// pm -> produs din magazin
                /// p -> produs din cos
                pm->setAmount(pm->getAmount() + p->getAmount()); /// dupa care adaugam in magazin cantitatea stearsa
                remove_product_by_barcode(barcode, produse_cos); /// si stergem produsul din cos
            }
        } else if (choice == "q") {
            return;
        }
        save_products_in_cos();
        save_products_in_stoc();
    }
}
/// END COS CURENT

/// COMENZI PRECEDENTE
void load_existing_commands() { /// incarcarea comenzilor existente
    std::ifstream fin("files/comenzi.txt");
    std::string order_tag;
    while (std::getline(fin, order_tag)) {
        Order* o = new Order();
        fin >> *o;
        comenzi.push_back(o);
    }
    fin.close();
}

void vizualizare_toate_comenzi() {
    system("cls");
    top_border("TOATE COMENZILE EXISTENTE");
    for (const auto& it : comenzi) {
        if (it->getCustomer() == customer) { /// se vizualizeaza toate comenzile clientului curent
            std::cout << *it << std::endl;
        }
    }
    system("pause");
}

void vizualizare_comanda_dupa_data() {
    system("cls");
    top_border("VIZUALIZARE COMENZI EXISTENTE INTR-O ZI");
    std::string date;
    std::cout << "Introdu data pentru a vizualiza comenzile [ZZ/LL/AAAA]: ";
    std::cin >> date; /// introducem data
    bool there_was_at_least_one_order_this_day = false;
    for (const auto& o : comenzi) { /// parcurgem toate comenzile
        std::string cdate = std::to_string(o->getDate().getDay()) + '/' +
                            std::to_string(o->getDate().getMonth()) + '/' +
                            std::to_string(o->getDate().getYear()); /// formatam data precum cea introdusa
        if (cdate == date && o->getCustomer() == customer) { /// daca gasim o comanda a clientului in data respectiva
            std::cout << *o << std::endl; /// o afisam
            there_was_at_least_one_order_this_day = true;
        }
    }
    if (!there_was_at_least_one_order_this_day) {
        std::cout << "Pentru data introdusa nu exista nicio comanda efectuata!" << std::endl;
    }
    system("pause");
}

std::string get_day_with_most_orders() {
    std::map<std::string, int> orders_on_day; /// o mapare de data per numar de comenzi in data respectiva
    for (const auto& o : comenzi) { /// parcurgem comenzile
        std::string cdate = std::to_string(o->getDate().getDay()) + '/' +
                    std::to_string(o->getDate().getMonth()) + '/' +
                    std::to_string(o->getDate().getYear());
        if (orders_on_day.find(cdate) != orders_on_day.end()) {
            orders_on_day[cdate]++; /// pentru data respectiva, incrementam numarul de comenzi
        } else {
            orders_on_day.insert({ cdate, 1 });
        }
    }
    int max_orders = 0;
    std::string max_date;
    for (const auto& entry : orders_on_day) { /// parcurgem maparea
        if (entry.second > max_orders) { /// si calculam ziua maxima
            max_orders = entry.second;
            max_date = entry.first;
        }
    }
    return max_date;
}

std::string best_sold_product() {
    /// similar cu ziua cu cele mai multe comenzi
    std::map<std::string, int> product_sold_times;
    std::map<std::string, std::string> product_name_barcode;
    for (const auto& o : comenzi) {
        for (const auto& p : o->getProducts()) {
            if (product_sold_times.find(p->getBarcode()) != product_sold_times.end()) {
                product_sold_times[p->getBarcode()]++;
            } else {
                product_sold_times.insert({ p->getBarcode(), 1 });
                product_name_barcode.insert({ p->getBarcode(), p->getName() });
            }
        }
    }
    int max_orders = 0;
    std::string max_prod;
    for (const auto& entry : product_sold_times) {
        if (entry.second > max_orders) {
            max_orders = entry.second;
            max_prod = entry.first;
        }
    }
    return product_name_barcode[max_prod] + " [ " + max_prod + " ] vandut de " + std::to_string(max_orders) + " ori";
}

std::string best_dinero_product() {
    /// similar cu ziua cu cele mai multe comenzi
    std::map<std::string, int> product_sold_times;
    std::map<std::string, std::string> product_name_barcode;
    for (const auto& o : comenzi) {
        for (const auto& p : o->getProducts()) {
            if (product_sold_times.find(p->getBarcode()) != product_sold_times.end()) {
                product_sold_times[p->getBarcode()] += p->getAmount() * p->getPrice();
            } else {
                product_sold_times.insert({ p->getBarcode(), p->getAmount() * p->getPrice() });
                product_name_barcode.insert({ p->getBarcode(), p->getName() });
            }
        }
    }
    int max_dinero = 0;
    std::string max_prod;
    for (const auto& entry : product_sold_times) {
        if (entry.second > max_dinero) {
            max_dinero = entry.second;
            max_prod = entry.first;
        }
    }
    return product_name_barcode[max_prod] + " [ " + max_prod + " ] castig: " + std::to_string(max_dinero);
}

void vizualizare_statistici_comenzi() {
    system("cls");

    top_border("STATISTICI COMENZI");

    std::cout << "Numar comenzi: " << comenzi.size() << std::endl;
    std::cout << "Ziua cu cele mai multe comenzi: " << get_day_with_most_orders() << std::endl;
    std::cout << "Produsul vandut de cele mai multe ori: " << best_sold_product() << std::endl;
    std::cout << "Produsul de pe care s-au castigat cei mai multi dinero: " << best_dinero_product() << std::endl;

    system("pause");
}

void meniu_comenzi_precedente() {
    std::vector<std::string> mprincipal = {
        "Vizualizarea tuturor comenzilor",
        "Cautare dupa data",
        "Vizualizare statistici comenzi",
        "Inapoi"
    };
    while (true) {
        std::string choice;
        do {
            system("cls");
            top_border("MENIU COMENZI PRECEDENTE");
            display_vector(mprincipal);
            std::cout << "Introdu optiunea dorita [1 - " << mprincipal.size() << "]: ";
            std::getline(std::cin, choice);
        } while(!is_number(choice) ||
                std::stoi(choice) == 0 ||
                std::stoi(choice) > mprincipal.size());
        switch (std::stoi(choice)) {
        case 1:
            vizualizare_toate_comenzi();
            break;
        case 2:
            vizualizare_comanda_dupa_data();
            break;
        case 3:
            vizualizare_statistici_comenzi();
            break;
        case 4:
            return;
        }
    }
}
/// END COMENZI PRECEDENTE

/// MENIU PRINCIPAL
void meniu_principal() {
    std::vector<std::string> mprincipal = {
        "Stoc magazin",
        "Cos curent",
        "Comenzi precedente",
        "Iesire"
    };
    while (true) {
        std::string choice;
        do {
            system("cls");
            top_border("MENIU PRINCIPAL");
            display_vector(mprincipal);
            std::cout << "Introdu optiunea dorita [1 - " << mprincipal.size() << "]: ";
            std::getline(std::cin, choice);
        } while(!is_number(choice) ||
                std::stoi(choice) == 0 ||
                std::stoi(choice) > mprincipal.size());
        switch (std::stoi(choice)) {
        case 1:
            meniu_stoc_magazin();
            break;
        case 2:
            meniu_cos_curent();
            break;
        case 3:
            meniu_comenzi_precedente();
            break;
        case 4:
            std::cout << "Va multumim pentru utilizarea aplicatiei!" << std::endl;
            exit(0);
        }
    }
}
/// END MENIU PRINCIPAL

void login() {
    std::ifstream fin("files/users.txt"); /// citim toti utilizatorii si parolele
    std::string name, password;
    while (std::getline(fin, name)) {
        std::getline(fin, password);
        users.insert({ name, password });
    }
    fin.close();

    std::string choice;
    while (choice != "login" && choice != "register") {
        system("cls");
        top_border("LOGIN");
        std::cout << "Introdu optiunea dorita [login / register]: "; /// cerem logare sau inregistrare
        set_color(Color::BLUE);
        std::getline(std::cin, choice);
        set_color(Color::WHITE);
    }
    if (choice == "login") { /// in cazul logarii
        while (users.find(customer) == users.end()) { /// cat timp nu se introduce un nume corect
            std::cout << "Introdu username-ul: ";
            set_color(Color::BLUE);
            std::getline(std::cin, customer); /// asteptam introducerea acestuia
            set_color(Color::WHITE);
        }
        std::string customer_password;
        int wrong_attempts_remaining = 3;
        while (users[customer] != customer_password) {
            std::cout << "Introdu parola: ";
            set_color(Color::BLUE);
            std::getline(std::cin, customer_password); /// asteptam introducerea unei parole corecte
            set_color(Color::WHITE);
            wrong_attempts_remaining--;
            if (wrong_attempts_remaining == 0) { /// sau ne oprim dupa 3 incercari esuate
                std::cout << "Ati introdus parola gresit de prea multe ori! La revedere!" << std::endl;
                exit(1);
            }
        }
    } else if (choice == "register") { /// in cazul inregistrarii
        std::string customer_password;
        std::cout << "Introdu username-ul: ";
        set_color(Color::BLUE);
        std::getline(std::cin, customer); /// citim numele
        set_color(Color::WHITE);
        std::cout << "Introdu parola: ";
        set_color(Color::BLUE);
        std::getline(std::cin, customer_password); /// si parola
        set_color(Color::WHITE);
        std::ofstream fout("files/users.txt", std::ios::app); /// si le salvam in fisier
        fout << customer << std::endl;
        fout << customer_password << std::endl;
    }
}

int main() {
    set_color(Color::WHITE);

    login();

    load_products_in_stoc();
    load_products_in_cos();
    load_existing_commands();

    save_comenzi();

    meniu_principal();
    return 0;
}
