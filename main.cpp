#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <limits>
using namespace std;

/*
    FOOD DONATION AND REDISTRIBUTION SYSTEM
    OOPs Mini Project | SDG 2 - Zero Hunger

    Default admin login:
    User ID: admin
    Password: admin123

    Data files are created automatically in the same folder as the program.
*/

const string USERS_FILE = "users.txt";
const string DONATIONS_FILE = "donations.txt";
const string REQUESTS_FILE = "requests.txt";
const string COMPLETED_FILE = "completed.txt";

string cleanField(string value) {
    // The pipe character separates fields in the text files.
    replace(value.begin(), value.end(), '|', '/');
    return value;
}

string inputLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return cleanField(value);
}

int inputInt(const string& prompt, int minimum, int maximum) {
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);
        stringstream ss(line);
        int value;
        char extra;
        if (ss >> value && !(ss >> extra) && value >= minimum && value <= maximum)
            return value;
        cout << "Invalid input. Enter a number from " << minimum
             << " to " << maximum << ".\n";
    }
}

bool isDigits(const string& value) {
    if (value.empty()) return false;
    for (char c : value) if (!isdigit(static_cast<unsigned char>(c))) return false;
    return true;
}

string currentDateTime() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    stringstream ss;
    ss << put_time(local, "%Y-%m-%d %H:%M");
    return ss.str();
}

vector<string> split(const string& line, char delimiter = '|') {
    vector<string> fields;
    string field;
    stringstream ss(line);
    while (getline(ss, field, delimiter)) fields.push_back(field);
    return fields;
}

enum class Role { DONOR, NGO, RECEIVER, ADMIN };

string roleToString(Role role) {
    switch (role) {
        case Role::DONOR: return "Donor";
        case Role::NGO: return "NGO";
        case Role::RECEIVER: return "Receiver";
        default: return "Admin";
    }
}

class User {
private:
    string id, name, password, phone, address;
protected:
    void setCommonDetails(const string& uid, const string& n, const string& p,
                          const string& ph, const string& a) {
        id = uid; name = n; password = p; phone = ph; address = a;
    }
public:
    User() {}
    User(const string& uid, const string& n, const string& pass,
         const string& ph, const string& addr) {
        setCommonDetails(uid, n, pass, ph, addr);
    }
    virtual ~User() {}

    string getId() const { return id; }
    string getName() const { return name; }
    string getPassword() const { return password; }
    string getPhone() const { return phone; }
    string getAddress() const { return address; }

    virtual Role getRole() const = 0;
    virtual void displayRole() const = 0;

    void displayProfile() const {
        cout << "\nUser ID   : " << id
             << "\nName      : " << name
             << "\nRole      : " << roleToString(getRole())
             << "\nContact   : " << phone
             << "\nAddress   : " << address << "\n";
    }

    string toFileRecord() const {
        return id + "|" + name + "|" + password + "|" + phone + "|" +
               address + "|" + roleToString(getRole());
    }
};

class Donor : public User {
public:
    Donor() {}
    Donor(const string& id, const string& n, const string& p,
          const string& ph, const string& a) : User(id, n, p, ph, a) {}
    Role getRole() const override { return Role::DONOR; }
    void displayRole() const override { cout << "Logged in as Donor\n"; }
};

class NGO : public User {
public:
    NGO() {}
    NGO(const string& id, const string& n, const string& p,
        const string& ph, const string& a) : User(id, n, p, ph, a) {}
    Role getRole() const override { return Role::NGO; }
    void displayRole() const override { cout << "Logged in as NGO\n"; }
};

class Receiver : public User {
public:
    Receiver() {}
    Receiver(const string& id, const string& n, const string& p,
             const string& ph, const string& a) : User(id, n, p, ph, a) {}
    Role getRole() const override { return Role::RECEIVER; }
    void displayRole() const override { cout << "Logged in as Receiver\n"; }
};

class Admin : public User {
public:
    Admin() {}
    Admin(const string& id, const string& n, const string& p,
          const string& ph, const string& a) : User(id, n, p, ph, a) {}
    Role getRole() const override { return Role::ADMIN; }
    void displayRole() const override { cout << "Administrator access\n"; }
};

class FoodDonation {
private:
    int id;
    string donorId, foodName, category, preparationDate, expiryDateTime, location, status;
    double quantity;
public:
    FoodDonation() : id(0), quantity(0) {}
    FoodDonation(int i, const string& did, const string& food, double qty,
                 const string& cat, const string& prep, const string& expiry,
                 const string& loc, const string& st = "Available")
        : id(i), donorId(did), foodName(food), category(cat),
          preparationDate(prep), expiryDateTime(expiry), location(loc), status(st) { quantity = qty; }
    int getId() const { return id; }
    string getDonorId() const { return donorId; }
    string getFoodName() const { return foodName; }
    string getCategory() const { return category; }
    string getExpiry() const { return expiryDateTime; }
    string getLocation() const { return location; }
    string getStatus() const { return status; }
    double getQuantity() const { return quantity; }
    void setQuantity(double q) { quantity = q; }
    void setFoodName(const string& v) { foodName = v; }
    void setCategory(const string& v) { category = v; }
    void setPreparationDate(const string& v) { preparationDate = v; }
    void setExpiry(const string& v) { expiryDateTime = v; }
    void setLocation(const string& v) { location = v; }
    void setStatus(const string& v) { status = v; }

    void display() const {
        cout << left << setw(5) << id << setw(22) << foodName
             << setw(9) << quantity << setw(25) << category
             << setw(19) << expiryDateTime << setw(14) << status
             << location << "\n";
    }
    string toFileRecord() const {
        stringstream ss;
        ss << id << "|" << donorId << "|" << foodName << "|" << quantity << "|"
           << category << "|" << preparationDate << "|" << expiryDateTime
           << "|" << location << "|" << status;
        return ss.str();
    }
};

class DonationRequest {
private:
    int id, donationId;
    string requesterId, ngoId, requestDate, status;
public:
    DonationRequest() : id(0), donationId(0) {}
    DonationRequest(int i, int d, const string& requester, const string& ngo,
                    const string& date, const string& st = "Pending")
        : id(i), donationId(d), requesterId(requester), ngoId(ngo),
          requestDate(date), status(st) {}
    int getId() const { return id; }
    int getDonationId() const { return donationId; }
    string getRequesterId() const { return requesterId; }
    string getNgoId() const { return ngoId; }
    string getStatus() const { return status; }
    void setStatus(const string& s) { status = s; }
    void display() const {
        cout << left << setw(7) << id << setw(12) << donationId
             << setw(16) << requesterId << setw(14) << ngoId
             << setw(19) << requestDate << status << "\n";
    }
    string toFileRecord() const {
        return to_string(id) + "|" + to_string(donationId) + "|" +
               requesterId + "|" + ngoId + "|" + requestDate + "|" + status;
    }
};

bool isRegisteredNGO(const string& id);

class FoodManager {
private:
    vector<FoodDonation> donations;
    vector<DonationRequest> requests;
    vector<string> completedRecords;
    int nextDonationId = 1, nextRequestId = 1;

    bool isExpired(const string& expiry) const {
        // Expected format: YYYY-MM-DD HH:MM. Lexicographic comparison works
        // for this fixed-width date/time format.
        return expiry.size() == 16 && expiry < currentDateTime();
    }

    void saveDonations() const {
        ofstream out(DONATIONS_FILE);
        for (const auto& d : donations) out << d.toFileRecord() << "\n";
    }
    void saveRequests() const {
        ofstream out(REQUESTS_FILE);
        for (const auto& r : requests) out << r.toFileRecord() << "\n";
    }
    void saveCompleted() const {
        ofstream out(COMPLETED_FILE);
        for (const auto& line : completedRecords) out << line << "\n";
    }
public:
    FoodManager() { loadData(); }
    ~FoodManager() { saveAll(); }

    void loadData() {
        string line;
        ifstream din(DONATIONS_FILE);
        while (getline(din, line)) {
            auto f = split(line);
            if (f.size() == 9) {
                try {
                    int id = stoi(f[0]);
                    FoodDonation d(id, f[1], f[2], stod(f[3]), f[4], f[5], f[6], f[7], f[8]);
                    donations.push_back(d);
                    nextDonationId = max(nextDonationId, id + 1);
                } catch (...) {}
            }
        }
        ifstream rin(REQUESTS_FILE);
        while (getline(rin, line)) {
            auto f = split(line);
            if (f.size() == 6) {
                try {
                    int id = stoi(f[0]), did = stoi(f[1]);
                    requests.emplace_back(id, did, f[2], f[3], f[4], f[5]);
                    nextRequestId = max(nextRequestId, id + 1);
                } catch (...) {}
            }
        }
        ifstream cin(COMPLETED_FILE);
        while (getline(cin, line)) if (!line.empty()) completedRecords.push_back(line);
    }

    void saveAll() const { saveDonations(); saveRequests(); saveCompleted(); }

    // Function overloading: search by category or by location.
    void search(const string& category) const {
        bool found = false;
        for (const auto& d : donations)
            if (d.getCategory() == category && d.getStatus() == "Available" && !isExpired(d.getExpiry())) {
                if (!found) printDonationHeader();
                d.display(); found = true;
            }
        if (!found) cout << "No available, non-expired donations found in this category.\n";
    }
    void search(const string& category, const string& location) const {
        bool found = false;
        for (const auto& d : donations)
            if (d.getCategory() == category && d.getLocation() == location &&
                d.getStatus() == "Available" && !isExpired(d.getExpiry())) {
                if (!found) printDonationHeader();
                d.display(); found = true;
            }
        if (!found) cout << "No matching donations found.\n";
    }
    static void printDonationHeader() {
        cout << left << setw(5) << "ID" << setw(22) << "Food"
             << setw(9) << "Qty" << setw(25) << "Category"
             << setw(19) << "Expiry" << setw(14) << "Status" << "Location\n";
        cout << string(110, '-') << "\n";
    }

    void addDonation(const string& donorId) {
        cout << "\n--- Add Food Donation ---\n";
        string food = inputLine("Food name: ");
        if (food.empty()) { cout << "Food name cannot be empty.\n"; return; }
        double qty = 0;
        while (true) {
            cout << "Quantity (in servings/kg): ";
            string s; getline(cin, s); stringstream ss(s); char extra;
            if (ss >> qty && !(ss >> extra) && qty > 0) break;
            cout << "Enter a valid quantity greater than zero.\n";
        }
        cout << "Category:\n1. Cooked Food\n2. Packaged Food\n3. Fresh Fruits and Vegetables\n4. Bakery Items\n";
        int c = inputInt("Choose category: ", 1, 4);
        const vector<string> cats = {"Cooked Food", "Packaged Food",
                                     "Fresh Fruits and Vegetables", "Bakery Items"};
        string prep = inputLine("Preparation date (YYYY-MM-DD): ");
        string expiry = inputLine("Expiry date/time (YYYY-MM-DD HH:MM): ");
        if (prep.size() != 10 || expiry.size() != 16 ||
            expiry.substr(0, 10) < prep || expiry < currentDateTime()) {
            cout << "Invalid date format or the food is already expired. Donation not added.\n";
            return;
        }
        string loc = inputLine("Pickup location: ");
        if (loc.empty()) { cout << "Location cannot be empty.\n"; return; }
        FoodDonation d(nextDonationId++, donorId, food, qty, cats[c-1], prep, expiry, loc);
        d.setQuantity(qty);
        donations.push_back(d);
        saveDonations();
        cout << "Donation added successfully. Donation ID: " << d.getId() << "\n";
    }

    void viewAvailable() const {
        cout << "\n--- Available Food Donations ---\n";
        bool found = false;
        for (const auto& d : donations) {
            if (d.getStatus() == "Available" && !isExpired(d.getExpiry())) {
                if (!found) printDonationHeader();
                d.display(); found = true;
            }
        }
        if (!found) cout << "There are currently no available donations.\n";
    }

    void viewDonorDonations(const string& donorId) const {
        cout << "\n--- Your Donations ---\n";
        bool found = false;
        for (const auto& d : donations) if (d.getDonorId() == donorId) {
            if (!found) printDonationHeader();
            d.display(); found = true;
        }
        if (!found) cout << "You have not added any donations yet.\n";
    }

    void updateDonation(const string& donorId) {
        viewDonorDonations(donorId);
        int id = inputInt("Enter donation ID to update: ", 1, 1000000000);
        for (auto& d : donations) {
            if (d.getId() == id && d.getDonorId() == donorId && d.getStatus() == "Available") {
                cout << "1. Food name  2. Quantity  3. Category  4. Expiry  5. Location\n";
                int ch = inputInt("Select field: ", 1, 5);
                if (ch == 1) {
                    string v = inputLine("New food name: "); if (!v.empty()) d.setFoodName(v);
                } else if (ch == 2) {
                    string s; double q;
                    while (true) {
                        cout << "New quantity: "; getline(cin, s); stringstream ss(s); char x;
                        if (ss >> q && !(ss >> x) && q > 0) break;
                        cout << "Enter a positive number.\n";
                    }
                    d.setQuantity(q);
                } else if (ch == 3) {
                    cout << "1. Cooked Food  2. Packaged Food  3. Fresh Fruits and Vegetables  4. Bakery Items\n";
                    int c = inputInt("New category: ", 1, 4);
                    vector<string> cats = {"Cooked Food", "Packaged Food", "Fresh Fruits and Vegetables", "Bakery Items"};
                    d.setCategory(cats[c-1]);
                } else if (ch == 4) {
                    string v = inputLine("New expiry (YYYY-MM-DD HH:MM): ");
                    if (v.size() != 16 || v < currentDateTime()) { cout << "Invalid/expired date.\n"; return; }
                    d.setExpiry(v);
                } else {
                    string v = inputLine("New pickup location: "); if (!v.empty()) d.setLocation(v);
                }
                saveDonations(); cout << "Donation updated successfully.\n"; return;
            }
        }
        cout << "Donation not found, not yours, or no longer editable.\n";
    }

    void removeDonation(const string& donorId) {
        viewDonorDonations(donorId);
        int id = inputInt("Enter donation ID to remove: ", 1, 1000000000);
        for (auto& d : donations) {
            if (d.getId() == id && d.getDonorId() == donorId && d.getStatus() == "Available") {
                d.setStatus("Removed");
                saveDonations();
                cout << "Donation marked as removed.\n"; return;
            }
        }
        cout << "Donation not found or cannot be removed.\n";
    }

    void requestDonation(const string& requesterId, bool isReceiver) {
        viewAvailable();
        int did = inputInt("Enter donation ID to request: ", 1, 1000000000);
        FoodDonation* selected = nullptr;
        for (auto& d : donations)
            if (d.getId() == did && d.getStatus() == "Available" && !isExpired(d.getExpiry()))
                selected = &d;
        if (!selected) { cout << "That donation is unavailable or expired.\n"; return; }

        string ngoId = requesterId;
        if (isReceiver) {
            cout << "Enter the registered NGO user ID that will coordinate collection.\n";
            ngoId = inputLine("NGO ID: ");
            if (!isRegisteredNGO(ngoId)) {
                cout << "The NGO ID is not registered. Request not created.\n"; return;
            }
        }
        for (const auto& r : requests)
            if (r.getDonationId() == did && r.getStatus() == "Pending") {
                cout << "A pending request already exists for this donation.\n"; return;
            }
        requests.emplace_back(nextRequestId++, did, requesterId, ngoId, currentDateTime());
        saveRequests();
        cout << "Request submitted successfully. The donor must approve it.\n";
    }

    void viewRequestsForUser(const string& uid, Role role) const {
        bool found = false;
        cout << "\n--- Donation Requests ---\n";
        cout << left << setw(7) << "Req ID" << setw(12) << "Donation"
             << setw(16) << "Requester" << setw(14) << "NGO ID"
             << setw(19) << "Date" << "Status\n";
        for (const auto& r : requests) {
            if ((role == Role::NGO && r.getNgoId() == uid) ||
                (role == Role::RECEIVER && r.getRequesterId() == uid) ||
                (role == Role::DONOR && donationBelongsTo(r.getDonationId(), uid))) {
                r.display(); found = true;
            }
        }
        if (!found) cout << "No requests found.\n";
    }

    bool donationBelongsTo(int donationId, const string& donorId) const {
        for (const auto& d : donations)
            if (d.getId() == donationId && d.getDonorId() == donorId) return true;
        return false;
    }

    void donorManageRequests(const string& donorId) {
        viewRequestsForUser(donorId, Role::DONOR);
        int id = inputInt("Enter request ID to process (0 to cancel): ", 0, 1000000000);
        if (id == 0) return;
        for (auto& r : requests) {
            if (r.getId() == id && donationBelongsTo(r.getDonationId(), donorId) &&
                r.getStatus() == "Pending") {
                cout << "1. Approve  2. Reject\n";
                int c = inputInt("Choose: ", 1, 2);
                if (c == 1) {
                    r.setStatus("Approved");
                    cout << "Request approved. NGO can now collect the food.\n";
                } else r.setStatus("Rejected");
                saveRequests(); return;
            }
        }
        cout << "Pending request not found for your donation.\n";
    }

    void markCollected(const string& ngoId) {
        viewRequestsForUser(ngoId, Role::NGO);
        int id = inputInt("Enter approved request ID to mark collected: ", 1, 1000000000);
        for (auto& r : requests) {
            if (r.getId() == id && r.getNgoId() == ngoId && r.getStatus() == "Approved") {
                for (auto& d : donations) if (d.getId() == r.getDonationId()) {
                    if (isExpired(d.getExpiry())) {
                        cout << "This food has expired. It cannot be collected.\n"; return;
                    }
                    d.setStatus("Collected");
                    r.setStatus("Collected");
                    completedRecords.push_back(d.toFileRecord() + "|Collected by NGO " + ngoId +
                                               "|At " + currentDateTime());
                    saveAll();
                    cout << "Collection recorded successfully. Thank you for preventing food waste!\n";
                    return;
                }
            }
        }
        cout << "No approved request found, or it is not assigned to your NGO.\n";
    }

    void adminViewDonations() const {
        cout << "\n--- All Donations ---\n";
        if (donations.empty()) { cout << "No donation records.\n"; return; }
        printDonationHeader();
        for (const auto& d : donations) d.display();
    }
    void adminViewRequests() const {
        cout << "\n--- All Requests ---\n";
        if (requests.empty()) { cout << "No request records.\n"; return; }
        cout << left << setw(7) << "Req ID" << setw(12) << "Donation"
             << setw(16) << "Requester" << setw(14) << "NGO ID"
             << setw(19) << "Date" << "Status\n";
        for (const auto& r : requests) r.display();
    }
    void adminViewCompleted() const {
        cout << "\n--- Completed Donation Records ---\n";
        if (completedRecords.empty()) { cout << "No completed donations yet.\n"; return; }
        for (const auto& line : completedRecords) cout << line << "\n";
    }
    void showStatistics() const {
        double total = 0, redistributed = 0;
        int active = 0;
        for (const auto& d : donations) {
            total += d.getQuantity();
            if (d.getStatus() == "Collected") redistributed += d.getQuantity();
            if (d.getStatus() == "Available" && !isExpired(d.getExpiry())) active++;
        }
        double percent = total > 0 ? (redistributed / total) * 100.0 : 0.0;
        cout << fixed << setprecision(2);
        cout << "\n========== SYSTEM STATISTICS ==========\n"
             << "Total food donated       : " << total << " units\n"
             << "Total food redistributed : " << redistributed << " units\n"
             << "Active donations         : " << active << "\n"
             << "Redistribution percentage: " << percent << "%\n"
             << "Requests submitted       : " << requests.size() << "\n"
             << "Completed donation count : " << completedRecords.size() << "\n"
             << "=======================================\n";
        cout << "Every meal shared is a step towards SDG 2 - Zero Hunger.\n";
    }
};

// UserStore owns registered user objects and handles persistence/login.
class UserStore {
private:
    vector<User*> users;
    int nextUserNumber = 1001;
public:
    UserStore() { loadUsers(); }
    ~UserStore() {
        for (User* u : users) delete u;
        users.clear();
    }


    void loadUsers() {
        ifstream in(USERS_FILE);
        string line;
        while (getline(in, line)) {
            auto f = split(line);
            if (f.size() != 6) continue;
            User* u = nullptr;
            if (f[5] == "Donor") u = new Donor(f[0], f[1], f[2], f[3], f[4]);
            else if (f[5] == "NGO") u = new NGO(f[0], f[1], f[2], f[3], f[4]);
            else if (f[5] == "Receiver") u = new Receiver(f[0], f[1], f[2], f[3], f[4]);
            if (u) {
                users.push_back(u);
                try { nextUserNumber = max(nextUserNumber, stoi(f[0].substr(1)) + 1); }
                catch (...) {}
            }
        }
    }
    void saveUsers() const {
        ofstream out(USERS_FILE);
        if (!out) { cout << "Error: unable to save users file.\n"; return; }
        for (const User* u : users) out << u->toFileRecord() << "\n";
    }
    void registerUser() {
        cout << "\n========== USER REGISTRATION ==========\n";
        cout << "1. Donor\n2. NGO\n3. Receiver\n";
        int role = inputInt("Register as: ", 1, 3);
        string name = inputLine("Full name / organisation name: ");
        string phone;
        while (true) {
            phone = inputLine("Contact number (digits only, 10-15 digits): ");
            if (phone.size() >= 10 && phone.size() <= 15 && isDigits(phone)) break;
            cout << "Please enter 10 to 15 digits.\n";
        }
        string address = inputLine("Address / location: ");
        string password = inputLine("Create password: ");
        if (name.empty() || address.empty() || password.empty()) {
            cout << "All fields are required.\n"; return;
        }
        string id = "U" + to_string(nextUserNumber++);
        User* u = nullptr;
        if (role == 1) u = new Donor(id, name, password, phone, address);
        else if (role == 2) u = new NGO(id, name, password, phone, address);
        else u = new Receiver(id, name, password, phone, address);
        users.push_back(u);
        saveUsers();
        cout << "Registration successful!\nYour User ID is: " << id
             << "\nPlease keep this ID for login.\n";
    }
    User* login() const {
        string id = inputLine("User ID: ");
        string password = inputLine("Password: ");
        for (User* u : users)
            if (u->getId() == id && u->getPassword() == password) return u;
        cout << "Invalid User ID or password.\n";
        return nullptr;
    }
    Admin* adminLogin() const {
        string id = inputLine("Admin ID: ");
        string pass = inputLine("Admin password: ");
        if (id == "admin" && pass == "admin123")
            return new Admin("admin", "System Administrator", pass, "-", "System");
        cout << "Invalid admin credentials.\n";
        return nullptr;
    }
    void displayAllUsers() const {
        cout << "\n--- Registered Users ---\n";
        if (users.empty()) { cout << "No registered users.\n"; return; }
        cout << left << setw(12) << "User ID" << setw(28) << "Name"
             << setw(14) << "Role" << setw(17) << "Contact" << "Address\n";
        for (const User* u : users)
            cout << left << setw(12) << u->getId() << setw(28) << u->getName()
                 << setw(14) << roleToString(u->getRole()) << setw(17)
                 << u->getPhone() << u->getAddress() << "\n";
    }
};

bool isRegisteredNGO(const string& id) {
    ifstream in(USERS_FILE);
    string line;
    while (getline(in, line)) {
        auto f = split(line);
        if (f.size() == 6 && f[0] == id && f[5] == "NGO") return true;
    }
    return false;
}

void donorMenu(User* user, FoodManager& manager) {
    user->displayRole();
    int choice;
    do {
        cout << "\n========== DONOR MENU ==========\n"
             << "1. Add food donation\n2. View my donations\n3. Update donation\n"
             << "4. Remove donation\n5. View/manage requests\n6. View all available food\n0. Logout\n";
        choice = inputInt("Enter choice: ", 0, 6);
        switch (choice) {
            case 1: manager.addDonation(user->getId()); break;
            case 2: manager.viewDonorDonations(user->getId()); break;
            case 3: manager.updateDonation(user->getId()); break;
            case 4: manager.removeDonation(user->getId()); break;
            case 5: manager.donorManageRequests(user->getId()); break;
            case 6: manager.viewAvailable(); break;
        }
    } while (choice != 0);
}
void ngoMenu(User* user, FoodManager& manager) {
    user->displayRole();
    int choice;
    do {
        cout << "\n============ NGO MENU ============\n"
             << "1. View available donations\n2. Search by category\n"
             << "3. Search by category and location\n4. Request a donation\n"
             << "5. View my requests\n6. Mark approved request as collected\n0. Logout\n";
        choice = inputInt("Enter choice: ", 0, 6);
        switch (choice) {
            case 1: manager.viewAvailable(); break;
            case 2: {
                cout << "1. Cooked Food  2. Packaged Food  3. Fresh Fruits and Vegetables  4. Bakery Items\n";
                int c = inputInt("Category: ", 1, 4);
                vector<string> cats = {"Cooked Food", "Packaged Food", "Fresh Fruits and Vegetables", "Bakery Items"};
                manager.search(cats[c-1]); break;
            }
            case 3: {
                cout << "1. Cooked Food  2. Packaged Food  3. Fresh Fruits and Vegetables  4. Bakery Items\n";
                int c = inputInt("Category: ", 1, 4);
                vector<string> cats = {"Cooked Food", "Packaged Food", "Fresh Fruits and Vegetables", "Bakery Items"};
                string loc = inputLine("Exact pickup location: ");
                manager.search(cats[c-1], loc); break;
            }
            case 4: manager.requestDonation(user->getId(), false); break;
            case 5: manager.viewRequestsForUser(user->getId(), Role::NGO); break;
            case 6: manager.markCollected(user->getId()); break;
        }
    } while (choice != 0);
}
void receiverMenu(User* user, FoodManager& manager) {
    user->displayRole();
    int choice;
    do {
        cout << "\n========== RECEIVER MENU ==========\n"
             << "1. View available donations\n2. Request food through an NGO\n"
             << "3. View my request status\n0. Logout\n";
        choice = inputInt("Enter choice: ", 0, 3);
        switch (choice) {
            case 1: manager.viewAvailable(); break;
            case 2: manager.requestDonation(user->getId(), true); break;
            case 3: manager.viewRequestsForUser(user->getId(), Role::RECEIVER); break;
        }
    } while (choice != 0);
}
void adminMenu(Admin* admin, UserStore& store, FoodManager& manager) {
    admin->displayRole();
    int choice;
    do {
        cout << "\n============ ADMIN MENU ============\n"
             << "1. View all registered users\n2. View all donations\n"
             << "3. Monitor all requests\n4. View completed donations\n"
             << "5. View system statistics\n0. Logout\n";
        choice = inputInt("Enter choice: ", 0, 5);
        switch (choice) {
            case 1: store.displayAllUsers(); break;
            case 2: manager.adminViewDonations(); break;
            case 3: manager.adminViewRequests(); break;
            case 4: manager.adminViewCompleted(); break;
            case 5: manager.showStatistics(); break;
        }
    } while (choice != 0);
}
void projectInformation() {
    cout << "\n========== PROJECT INFORMATION ==========\n"
         << "Project : Food Donation and Redistribution System\n"
         << "SDG     : SDG 2 - Zero Hunger\n"
         << "Purpose : Connect food donors with NGOs and receivers.\n"
         << "Impact  : Reduce edible food waste and improve food access.\n"
         << "Roles   : Donor, NGO, Receiver and Admin.\n"
         << "Storage : Local text files; no external database required.\n"
         << "=========================================\n";
}
int main() {
    FoodManager manager;
    UserStore userStore;
    cout << "====================================================\n"
         << "   WELCOME TO FOOD DONATION AND REDISTRIBUTION SYSTEM\n"
         << "             SDG 2 - ZERO HUNGER\n"
         << "====================================================\n";
    int choice;
    do {
        cout << "\n================ MAIN MENU ================\n"
             << "1. Register\n2. Login\n3. Admin Login\n"
             << "4. View Project Information\n5. Exit\n";
        choice = inputInt("Enter your choice: ", 1, 5);
        if (choice == 1) userStore.registerUser();
        else if (choice == 2) {
            User* user = userStore.login();
            if (user) {
                if (user->getRole() == Role::DONOR) donorMenu(user, manager);
                else if (user->getRole() == Role::NGO) ngoMenu(user, manager);
                else if (user->getRole() == Role::RECEIVER) receiverMenu(user, manager);
            }
        } else if (choice == 3) {
            Admin* admin = userStore.adminLogin();
            if (admin) { adminMenu(admin, userStore, manager); delete admin; }
        } else if (choice == 4) projectInformation();
        else cout << "\nThank you for using the system!\nShare food, spread hope, and support Zero Hunger.\n";
    } while (choice != 5);
    return 0;
}
