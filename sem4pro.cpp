#include <iostream>

#include <unordered_map>

#include <vector>

#include <algorithm>

#include <fstream>

#include <ctime>

#include <regex>

#include <iomanip>

#include <cmath>

using namespace std;

const int tscar = 100, tsbike = 200, tsrick = 50, tsbus = 20;

int carcap = 0, bikecap = 0, rickcap = 0, buscap = 0;

int carcost = 0, bikecost = 0, rickcost = 0, buscost = 0;


struct Vehicle {

    string vehicle_number;

    time_t entry_time;

    string type;
};



// storage function
void logToFile(const string& action, const string& vehicle_number, const string& type, time_t time) {

    ofstream file("parking.txt", ios::app);

    file << action << "," << vehicle_number << "," << type << ","

         << put_time(localtime(&time), "%Y-%m-%d %H:%M:%S") << "\n";

}


// KMP algorithm

//LPS array

void buildLPSArray(const string& pattern, vector<int>& lps) {

    int M = pattern.size();

    int len = 0;

    lps[0] = 0;

    int i = 1;
    while (i < M) {

        if (pattern[i] == pattern[len]) {

            len++;

            lps[i] = len;

            i++;
        }
        else {

            if (len != 0) {

                len = lps[len - 1];

            }
             else {

                lps[i] = 0;

                i++;


            }
        }
    }
}


void KMPSearch(const string& text, const string& pattern) {

    int N = text.size();

    int M = pattern.size();

    vector<int> lps(M);

    buildLPSArray(pattern, lps);

    int i = 0, j = 0;

    while (i < N) {

        if (pattern[j] == text[i]) {

            i++;

            j++;

        }

        if (j == M) {

            j = lps[j - 1];

        }

        else if (i < N && pattern[j] != text[i]) {

            if (j != 0) {

                j = lps[j - 1];

            }
            else {
                i++;
            }
        }
    }
}



// Regular expression to validate vehicle number
bool isValidVehicleNumber(const string& vehicle_number) {

    regex vehicle_regex("^[A-Z]{2}[0-9]{6}$");

    return regex_match(vehicle_number, vehicle_regex);
}

vector<Vehicle> cars, bikes, rickshaws, buses;

unordered_map<string, string> users;

bool isLoggedIn = false;

unordered_map<string, string> reviews;

void Signup();

bool Login();

void AddVehicle(std::vector<Vehicle>& vehicles, int& count, int slot, int& earnings, int rate, const std::string& number);

void RemoveVehicle(vector<Vehicle>& vehicles, int& count, int& earnings, const string& type);

void DisplayEarnings(int cost, const string& type);

void TotalEarnings();

void MaxEarnings();

void DisplaySpaces();

void UserMenu();

void AdminMenu();

void SubmitReview();

void ViewReviews();

void SearchVehicle(const vector<Vehicle>& vehicles, const string& vehicle_number, const string& type);

void ReserveParking(const string& type);

int main() {

    int choice;


    cout << "\t--------------------------------------------\n";


    cout << "\t PARKING MANAGEMENT SYSTEM\n";


    cout << "\t--------------------------------------------\n";


    do {

        cout << "\n1. Signup\n2. Login\n3. Exit\nSelect an option: ";



        cin >> choice;

        switch (choice) {
            case 1: Signup(); break;


            case 2: if (Login()) UserMenu(); break;


            case 3: cout << "Thank you for using the system!\n"; return 0;



            default: cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 3);

    return 0;
}

void Signup() {

    string username, password;

    cout << "Enter a username: ";

    cin >> username;

    if (users.find(username) != users.end()) {

        cout << "Username already exists! Try another.\n";

        return;
    }

    cout << "Enter a password: ";

    cin >> password;

    users[username] = password;

    cout << "Signup successful! Please login.\n";
}

bool Login() {

    string username, password, role;

    cout << "Enter username: ";

    cin >> username;

    cout << "Enter password: ";

    cin >> password;

    if (users.find(username) != users.end() && users[username] == password) {

        isLoggedIn = true;

        cout << "Login successful! Welcome, " << username << "\n";

        cout << "Enter role (Admin/User): ";
        cin >> role;

        if (role == "Admin" || role == "admin")
            AdminMenu();
        else
            UserMenu();

        return true;

    }
     else {

        cout << "Invalid credentials. Try again.\n";

        return false;
    }
}


void AdminMenu() {

    int choice;

    do {
        cout << "\n\t--- Admin Dashboard ---\n";

        cout << "1. Car Earnings\n2. Bike Earnings\n3. Rickshaw Earnings\n4. Bus Earnings\n";

        cout << "5. Total Earnings\n6. Maximum Earnings\n7. View Reviews\n8. Logout\n";

        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice) {

            case 1: DisplayEarnings(carcost, "Car");
            break;

            case 2: DisplayEarnings(bikecost, "Bike");
            break;
            case 3: DisplayEarnings(rickcost, "Rickshaw");
            break;
            case 4: DisplayEarnings(buscost, "Bus");
            break;
            case 5: TotalEarnings();
            break;
            case 6: MaxEarnings();
            break;
            case 7: ViewReviews();
            break;

            case 8: isLoggedIn = false; cout << "Logged out successfully.\n"; return;

            default: cout << "Invalid choice. Try again.\n";

        }
    } while (isLoggedIn);
}
void UserMenu() {

    int choice;

    do {
        cout << "\n\t--- User Dashboard ---\n";

        cout << "1. Car Entry\n2. Car Exit\n";

        cout << "3. Bike Entry\n4. Bike Exit\n";

        cout << "5. Rickshaw Entry\n6. Rickshaw Exit\n";

        cout << "7. Bus Entry\n8. Bus Exit\n";

        cout << "9. Available Spaces\n10. Submit Review\n11. View Reviews\n";

        cout << "12. Search Vehicle\n13. Reserve Parking\n14. Logout\n";

        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice) {
            case 1: AddVehicle(cars, carcap, tscar, carcost, 50, "Car");
            break;
            case 2: RemoveVehicle(cars, carcap, carcost, "Car");
            break;
            case 3: AddVehicle(bikes, bikecap, tsbike, bikecost, 20, "Bike");
            break;
            case 4: RemoveVehicle(bikes, bikecap, bikecost, "Bike");
            break;
            case 5: AddVehicle(rickshaws, rickcap, tsrick, rickcost, 30, "Rickshaw");
            break;
            case 6: RemoveVehicle(rickshaws, rickcap, rickcost, "Rickshaw");
            break;
            case 7: AddVehicle(buses, buscap, tsbus, buscost, 100, "Bus");
            break;
            case 8: RemoveVehicle(buses, buscap, buscost, "Bus");
            break;
            case 9: DisplaySpaces();
            break;
            case 10: SubmitReview();
            break;
            case 11: ViewReviews();
            break;

            case 12: {

                string vehicle_number;

                cout << "Enter vehicle number to search: ";

                cin >> vehicle_number;

                SearchVehicle(cars, vehicle_number, "Car");

                SearchVehicle(bikes, vehicle_number, "Bike");

                SearchVehicle(rickshaws, vehicle_number, "Rickshaw");

                SearchVehicle(buses, vehicle_number, "Bus");

                break;
            }


            case 13: {

                string type;

                cout << "Enter vehicle type (Car/Bike/Rickshaw/Bus): ";

                cin >> type;

                ReserveParking(type);

                break;
            }

            case 14: isLoggedIn = false; cout << "Logged out successfully.\n";

            return;

            default: cout << "Invalid choice. Try again.\n";

        }
    } while (isLoggedIn);
}

void SubmitReview() {

    string username, review;

    cout << "Enter your username: ";

    cin >> username;

    cin.ignore();

    cout << "Enter your review: ";

    getline(cin, review);

    reviews[username] = review;

    cout << "Thank you for your review!\n";

}

void ViewReviews() {

    if (reviews.empty()) {

        cout << "No reviews yet.\n";

        return;
    }
    cout << "Customer Reviews:\n";

    for (const auto& review : reviews) {

        cout << review.first << ": " << review.second << "\n";

    }

}

double getVehicleFee(const string& type, double duration_hours) {

    double fee_per_hour = 0;

    if (type == "Car") {

        fee_per_hour = 10.0;

    }
    else if (type == "Bike") {

        fee_per_hour = 5.0;

    }
    else if (type == "Rickshaw") {

        fee_per_hour = 8.0;

    }
    else if (type == "Bus") {

        fee_per_hour = 15.0;

    }

    return ceil(duration_hours) * fee_per_hour;
}
void AddVehicle(vector<Vehicle>& vehicles, int& count, int slot, int& earnings, int rate, const string& type) {

    if (count < slot) {

        string vehicle_number;

        cout << "Enter " << type << " Number: ";

        cin >> vehicle_number;

        if (!isValidVehicleNumber(vehicle_number)) {

            cout << "Invalid vehicle number format! Must be two capital letters followed by six digits.\n";

            return;
        }

        Vehicle vehicle;

        vehicle.vehicle_number = vehicle_number;

        vehicle.entry_time = time(nullptr); // Record entry time

        vehicle.type = type;

        vehicles.push_back(vehicle);

        count++;

        logToFile("ENTRY", vehicle_number, type, vehicle.entry_time);  // ✅ Log entry

        cout << type << " parked successfully.\n";

    }

    else {

        cout << "No space available for " << type << "s.\n";

    }
}

void RemoveVehicle(vector<Vehicle>& vehicles, int& count, int& earnings, const string& type) {

    string vehicle_number;

    cout << "Enter " << type << " Number to remove: ";

    cin >> vehicle_number;

    auto it = find_if(vehicles.begin(), vehicles.end(), [&vehicle_number](const Vehicle& v) {

        return v.vehicle_number == vehicle_number;

    });

    if (it != vehicles.end()) {

        time_t exit_time = time(nullptr);

        double duration_minutes = difftime(exit_time, it->entry_time) / 60.0;

        double duration_hours = duration_minutes / 60.0;

        double fee = getVehicleFee(type, duration_hours);

        logToFile("EXIT", it->vehicle_number, type, exit_time);

        vehicles.erase(it);

        count--;

        earnings += fee;

        cout << type << " with number " << vehicle_number << " removed successfully.\n";

        cout << "Parked duration: " << duration_minutes << " minutes\n";

    }
    else {

        cout << type << " with number " << vehicle_number << " not found.\n";

    }

}


void DisplayEarnings(int cost, const string& type) {

    cout << "Total earnings from " << type << "s: $" << cost << "\n";

}

void TotalEarnings() {

    int total = carcost + bikecost + rickcost + buscost;

    cout << "Total earnings from all vehicles: $" << total << "\n";

}

void MaxEarnings() {

    int n = 4;

    int baseValues[] = {50, 20, 30, 100};

    int weights[] = {1, 1, 1, 1};

    string vehicleTypes[] = {"Car", "Bike", "Rickshaw", "Bus"};


    int capacity;

    cout << "\nEnter the available parking slots to estimate maximum potential earnings: ";

    cin >> capacity;

    vector<int> dp(capacity + 1, 0);

    vector<string> picked(capacity + 1, "");

    for (int i = 0; i < n; i++) {
        // Try every possible count of this vehicle type

        for (int count = 1; count <= capacity; count++) {

            int value = baseValues[i] - (count - 1) * 10;

            // reduce price dynamically

            if (value <= 0) break;
            // skip if dynamic price becomes non-profitable

            for (int w = capacity; w >= weights[i]; w--) {

                if (w >= count * weights[i]) {

                    int totalValue = dp[w - count * weights[i]] + value;

                    if (totalValue > dp[w]) {

                        dp[w] = totalValue;

                        picked[w] = picked[w - count * weights[i]] + vehicleTypes[i] + " ";

                    }
                }
            }
        }
    }

    cout << "Max possible earnings with " << capacity << " available slots: $" << dp[capacity] << "\n";

    cout << "Selected vehicle types for max earnings: " << picked[capacity] << "\n";

}

void DisplaySpaces() {

    cout << "Available Spaces:\n";

    cout << "Cars: " << (tscar - carcap) << "/" << tscar << "\n";

    cout << "Bikes: " << (tsbike - bikecap) << "/" << tsbike << "\n";

    cout << "Rickshaws: " << (tsrick - rickcap) << "/" << tsrick << "\n";

    cout << "Buses: " << (tsbus - buscap) << "/" << tsbus << "\n";
}

void SearchVehicle(const vector<Vehicle>& vehicles, const string& vehicle_number, const string& type) {

    bool found = false;

    for (const auto& v : vehicles) {

        KMPSearch(v.vehicle_number, vehicle_number);

        if (v.vehicle_number == vehicle_number) {

            found = true;

            cout << type << " found:\n";

            cout << "Number: " << v.vehicle_number << "\n";

            cout << "Entry Time: " << put_time(localtime(&v.entry_time), "%Y-%m-%d %H:%M:%S") << "\n";

            break;

        }

    }
    if (!found) {

        cout << type << " with number " << vehicle_number << " not found.\n";

    }

}

void ReserveParking(const string& type) {

    if (type == "Car" && carcap < tscar) {

        carcap++;

        cout << "Parking spot reserved for Car.\n";

    }
    else if (type == "Bike" && bikecap < tsbike) {

        bikecap++;

        cout << "Parking spot reserved for Bike.\n";

    }
    else if (type == "Rickshaw" && rickcap < tsrick) {

        rickcap++;

        cout << "Parking spot reserved for Rickshaw.\n";

    }
    else if (type == "Bus" && buscap < tsbus) {

        buscap++;

        cout << "Parking spot reserved for Bus.\n";

    }
    else {

        cout << "No space available for " << type << "s.\n";

    }

}
