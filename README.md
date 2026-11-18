# Food Donation and Redistribution System
**OOPs Mini Project | C++ | SDG 2 – Zero Hunger**

## Files
- `main.cpp` – complete menu-driven C++ application.
- `users.txt` – registered users (created automatically).
- `donations.txt` – food donation records (created automatically).
- `requests.txt` – donation requests (created automatically).
- `completed.txt` – completed collection history (created automatically).
- `Mini_Project_Report.md` – project report.

## Compile and run
### Windows (MinGW g++)
Open a terminal in this folder:
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o food_donation
food_donation.exe
```
### Linux / macOS
```bash
g++ -std=c++17 -Wall -Wextra main.cpp -o food_donation
./food_donation
```
The application uses only standard C++ libraries. Keep the four text data files in the same working directory as the executable. They are created automatically when needed.

## Admin login
- User ID: `admin`
- Password: `admin123`

Change these credentials in `UserStore::adminLogin()` before submitting if your instructor requires a different demo password.

## Important input formats
- Preparation date: `YYYY-MM-DD`
- Expiry date/time: `YYYY-MM-DD HH:MM` (24-hour clock)
- Contact number: 10–15 digits
- Quantity: positive number (servings or kg)

## Typical workflow
1. Register a Donor, NGO, and Receiver from the main menu.
2. Log in as Donor and add a donation with a future expiry time.
3. Log in as NGO and request that donation.
4. Log in as Donor and approve the request.
5. Log in as the same NGO and mark the approved request as collected.
6. Log in as Admin and view users, donations, requests, completed records, and statistics.

## Notes / limitations
- This is an educational console prototype, not a production food-safety platform.
- Expiry validation uses the fixed `YYYY-MM-DD HH:MM` text format and the computer's local clock. It does not parse arbitrary date formats.
- Passwords are stored as plain text to keep the file-handling example beginner-friendly; a real system must hash passwords and protect personal data.
- The receiver selects a registered NGO ID to coordinate collection. The NGO can view the receiver's request and complete collection after donor approval.
