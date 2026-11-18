# MINI PROJECT REPORT
## Food Donation and Redistribution System
**Subject:** Object-Oriented Programming using C++  
**Domain:** Sustainable Development Goals  
**SDG:** 2 – Zero Hunger

## 1. Introduction
Food wastage and hunger exist at the same time in many communities. Restaurants, hotels, shops and households may have safe surplus food, while NGOs and people in need may struggle to obtain meals. The Food Donation and Redistribution System is a console-based application that provides a simple way to record surplus food, request it and track its collection. It demonstrates core Object-Oriented Programming concepts using standard C++ and text-file storage.

## 2. Problem Statement
Manual coordination of surplus food donations can be slow and unorganised. Donors may not know which organisation needs food, while NGOs and receivers may not know what food is available nearby. Paper records are difficult to update and monitor. A basic computerised system is therefore proposed to maintain users, donations, requests and completed collections.

## 3. Objectives
- Register donors, NGOs and receivers and provide a basic login facility.
- Allow donors to add, view, update and remove available donations.
- Allow NGOs to search and request food donations.
- Allow receivers to submit requests through registered NGOs.
- Prevent requests and collection of expired food.
- Track request states from Pending to Approved/Rejected and Collected.
- Store records permanently using text files.
- Display statistics for donated and redistributed food.
- Demonstrate OOP principles in a beginner-friendly C++ program.

## 4. Scope of the Project
The project is designed for a small college-level demonstration. It supports local registration, donation management, NGO coordination, receiver requests, donor approval, collection tracking and administrator monitoring. It does not provide online payments, GPS routing, identity verification, automated food inspection, cloud hosting or multi-computer synchronisation.

## 5. Proposed System
The application starts with a main menu. A user can register, log in, view project information or exit. After login, the role-specific menu is displayed:
- **Donor:** manages food donations and approves/rejects requests for their donations.
- **NGO:** views and searches available donations, requests food and records collection after approval.
- **Receiver:** views available food, requests it through a registered NGO and checks request status.
- **Admin:** monitors registered users, all donations, requests, completed records and system statistics.

A donation is eligible for a request only when its status is Available and its expiry date/time has not passed. An NGO can mark an approved request as Collected. The donation and request statuses are then updated and a completion record is written.

## 6. Software and Hardware Requirements
### Software
- Operating system: Windows 10/11, Linux or macOS
- Compiler: GCC/MinGW g++ supporting C++17
- Editor/IDE: VS Code, Code::Blocks, Dev-C++ or any text editor
- Libraries: Standard C++ library only

### Hardware
- Basic desktop or laptop
- 2 GB RAM or higher recommended
- A small amount of disk space for the executable and text files

## 7. System Design
### 7.1 Architecture
The application has four logical parts:
1. **User management:** registration, login and role-specific user objects.
2. **Donation management:** creation, update, removal, listing, searching and expiry checks.
3. **Request management:** request creation, donor approval/rejection and collection.
4. **Persistence and reporting:** text-file loading/saving and statistics.

### 7.2 Class Diagram
```text
                      +----------------------+
                      |      User (abstract) |
                      |----------------------|
                      | - id, name, password |
                      | - phone, address     |
                      |----------------------|
                      | + displayRole()      |
                      | + getRole()          |
                      | + displayProfile()   |
                      +----------+-----------+
                                 |
             +-------------------+-------------------+
             |           |             |             |
          +------+    +------+     +----------+   +-------+
          | Donor|    | NGO  |     | Receiver |   | Admin |
          +------+    +------+     +----------+   +-------+

 +-------------------+       +----------------------+
 |   FoodDonation    |       |   DonationRequest    |
 |-------------------|       |----------------------|
 | id, donorId       |       | id, donationId       |
 | food, quantity    |       | requesterId, ngoId  |
 | category, dates   |       | date, status         |
 | location, status  |       +----------------------+
 +-------------------+                  |
             \                          /
              \                        /
               +----------------------+
               |     FoodManager      |
               |----------------------|
               | donations, requests  |
               | file load/save       |
               | search, statistics   |
               +----------------------+

               +----------------------+
               |      UserStore       |
               |----------------------|
               | users, registration   |
               | login, user file I/O  |
               +----------------------+
```

### 7.3 Flowchart
```text
          +-------+
          | Start |
          +---+---+
              |
       +------v-------+
       | Load text    |
       | files        |
       +------+-------+
              |
       +------v-------+
       | Display main |
       | menu         |
       +------+-------+
              |
       +------v-------+
       | Register /   |
       | Login / Admin|
       +------+-------+
              |
       +------v-------+
       | Identify role|
       +------+-------+
              |
       +------v-------------------+
       | Show role-specific menu  |
       +------+------------------+
              |
       +------v-------------------+
       | Perform selected action  |
       | and save changed records |
       +------+------------------+
              |
       +------v-------+
       | Logout or    |
       | continue?    |
       +---+-------+--+
           |       |
        Continue  Exit
           |       |
           +       v
                 +------+
                 | End  |
                 +------+
```

## 8. Implementation
The application is implemented in `main.cpp` using C++17. The `User` class is an abstract base class. `Donor`, `NGO`, `Receiver` and `Admin` inherit from it. `FoodDonation` and `DonationRequest` represent the main records. `FoodManager` controls donations, requests, expiry validation, file operations and statistics. `UserStore` controls registration, login and user persistence.

Four text files are used:
- `users.txt`: user ID, name, password, contact, address and role.
- `donations.txt`: donation ID, donor ID, food name, quantity, category, preparation date, expiry date/time, location and status.
- `requests.txt`: request ID, donation ID, requester ID, coordinating NGO ID, request date and status.
- `completed.txt`: a history line for each collected donation.

The files are created automatically when the program saves data. Records are separated by the `|` character. User-entered pipe characters are replaced to protect the file format.

## 9. OOP Concepts Used
1. **Classes and Objects:** `User`, `Donor`, `NGO`, `Receiver`, `Admin`, `FoodDonation`, `DonationRequest`, `FoodManager` and `UserStore` are classes. Runtime objects are created during registration and operation.
2. **Encapsulation:** User and record data members are private. Public getter/setter functions provide controlled access.
3. **Inheritance:** The four role classes derive from the common `User` base class and reuse common user details and functions.
4. **Runtime Polymorphism:** `getRole()` and `displayRole()` are virtual functions overridden by each derived class. The program calls them through `User*` pointers.
5. **Function Overloading:** `FoodManager::search(category)` searches by category, while `search(category, location)` searches by both category and location.
6. **Constructors and Destructors:** Constructors initialise objects. Destructors release dynamically allocated user objects and save manager data.
7. **Abstraction:** `User` is an abstract class because it declares pure virtual functions. The user interface works with the abstract `User` type rather than needing to know every derived implementation.
8. **File Handling:** `ifstream` reads existing data and `ofstream` writes updated records to text files.
9. **STL:** `vector` stores collections, `string` stores text, and algorithms such as `max` and `replace` assist processing.

## 10. Testing
The following test cases can be used during demonstration:

| Test | Input / Action | Expected result |
|---|---|---|
| Register donor | Enter valid details | User ID is generated and saved |
| Login | Correct ID and password | Donor menu opens |
| Invalid login | Wrong password | Error message is displayed |
| Add donation | Valid food, quantity and future expiry | Donation is saved |
| Invalid quantity | Enter zero or negative quantity | Input is rejected |
| Search | Select a category | Matching available donations appear |
| Request donation | NGO selects available donation | Pending request is created |
| Duplicate pending request | Request same donation again | Duplicate pending request is blocked |
| Donor approval | Donor approves request | Request status becomes Approved |
| Collection | NGO marks approved request collected | Donation and request become Collected |
| Expired food | Try to request expired donation | Request is blocked |
| Admin statistics | Open statistics menu | Totals and redistribution percentage appear |
| Restart program | Exit and run again | Saved records are loaded |

## 11. Conclusion
The Food Donation and Redistribution System provides a simple digital process for recording surplus food and coordinating its redistribution through NGOs. It reduces dependence on manual records and demonstrates how OOP concepts can be combined with file handling in C++. The project supports the broader aim of SDG 2 by encouraging food sharing and helping connect surplus food with people who need it.

## 12. Future Scope
- Add a graphical or web/mobile interface.
- Use a secure database and hashed passwords.
- Add OTP-based contact verification.
- Add GPS-based distance and location search.
- Add food image upload and quality verification.
- Send SMS/email notifications for requests and expiry reminders.
- Add quantity splitting so one donation can serve multiple NGOs.
- Add role-based audit logs and downloadable reports.

## 13. SDG Contribution
This project is aligned with **SDG 2 – Zero Hunger**. It supports food access by connecting donors, NGOs and receivers. It also helps reduce edible food waste by making surplus food visible and trackable. The system is an educational prototype; its real-world impact would depend on safe handling, timely pickup, food-quality checks and participation from local organisations.
