#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
#include <map>
#include <iomanip>
#include <cmath>

using namespace std;

// Helper functions
string toLower(const string& s) {
    string lower = s;
    transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return lower;
}

bool isValidNumber(const string& number) {
    if (number.length() != 10) return false;
    for (char c : number) {
        if (!isdigit(c)) return false;
    }
    return true;
}

bool isValidEmail(const string& email) {
    size_t atPos = email.find('@');
    size_t dotPos = email.find('.', atPos);
    return (atPos != string::npos && dotPos != string::npos && atPos < dotPos);
}

// Currency converter
double convertCurrency(double amount, const string& fromCurrency, const string& toCurrency) {
    map<string, double> rates = {
        {"INR", 1.0},       // Indian Rupee (base)
        {"USD", 0.012},     // US Dollar
        {"EUR", 0.011},     // Euro
        {"GBP", 0.0095},    // British Pound
        {"VND", 290.0},     // Vietnamese Dong
        {"IDR", 190.0},     // Indonesian Rupiah
        {"TRY", 0.38},      // Turkish Lira
        {"AED", 0.044},     // UAE Dirham
        {"KHR", 50.0},      // Cambodian Riel
        {"THB", 0.43},      // Thai Baht
        {"MYR", 0.057},     // Malaysian Ringgit
        {"SGD", 0.016},     // Singapore Dollar
        {"JPY", 1.77},      // Japanese Yen
        {"KRW", 16.0},      // South Korean Won
        {"RUB", 1.10}       // Russian Ruble
    };
    
    if (rates.find(fromCurrency) == rates.end() || rates.find(toCurrency) == rates.end()) {
        return amount; // Return same amount if currency not found
    }
    
    double inBase = amount / rates[fromCurrency];
    return inBase * rates[toCurrency];
}

// --------------------- CustomerDetails ---------------------
class CustomerDetails {
public:
    string name, number, email;
    int adults, children;
    string tripPurpose; // family, couple, friends, solo
    
    bool inputDetails() {
        cout << "\nEnter customer details (type 'x' anytime to restart):\n";

        cout << "Name of the customer: ";
        getline(cin, name);
        if (toLower(name) == "x") return false;

        while (true) {
            cout << "Enter your 10-digit number: ";
            getline(cin, number);
            if (toLower(number) == "x") return false;
            if (isValidNumber(number)) break;
            else cout << "Invalid number. Please enter exactly 10 digits.\n";
        }

        while (true) {
            cout << "Enter your email: ";
            getline(cin, email);
            if (toLower(email) == "x") return false;
            if (isValidEmail(email)) break;
            else cout << "Invalid email format. Please try again.\n";
        }

        while (true) {
            cout << "Number of adults traveling: ";
            string input;
            getline(cin, input);
            if (toLower(input) == "x") return false;
            try {
                adults = stoi(input);
                if (adults <= 0) throw invalid_argument("");
                break;
            } catch (...) {
                cout << "Please enter a valid positive number.\n";
            }
        }

        while (true) {
            cout << "Number of children traveling: ";
            string input;
            getline(cin, input);
            if (toLower(input) == "x") return false;
            try {
                children = stoi(input);
                if (children < 0) throw invalid_argument("");
                break;
            } catch (...) {
                cout << "Please enter a valid non-negative number.\n";
            }
        }

        while (true) {
            cout << "Trip purpose (family/couple/friends/solo): ";
            getline(cin, tripPurpose);
            tripPurpose = toLower(tripPurpose);
            if (tripPurpose == "x") return false;
            if (tripPurpose == "family" || tripPurpose == "couple" || 
                tripPurpose == "friends" || tripPurpose == "solo") break;
            else cout << "Invalid input. Please choose from family/couple/friends/solo.\n";
        }

        return true;
    }

    void displayDetails() {
        cout << "\n-------> WELCOME TO SMART WANDERPLAN <-------\n";
        cout << "--- Customer Details ---" << endl;
        cout << "Name: " << name << endl;
        cout << "Number: " << number << endl;
        cout << "Email: " << email << endl;
        cout << "Travelers: " << adults << " adults, " << children << " children" << endl;
        cout << "Trip Purpose: " << tripPurpose << endl;
    }
};

// --------------------- Destination & PreferenceTree ---------------------
class Destination {
public:
    string name, type, currency;
    double minbudget, maxbudget;
    vector<string> attractions;
    vector<string> foods;
    vector<string> itineraryOptions[3]; // 3 different itinerary options
    bool familyFriendly;
    bool coupleFriendly;
    string weather;
    string visaInfo;
    string bestSeason;
    Destination *left, *right;

    Destination(string n, string t, string curr, double minb, double maxb, 
                vector<string> attr, vector<string> fd, 
                vector<string> itin1, vector<string> itin2, vector<string> itin3,
                bool famFriendly, bool coupFriendly, string wthr, string visa, string season) {
        name = n; type = t; currency = curr; 
        minbudget = minb; maxbudget = maxb;
        attractions = attr; foods = fd;
        itineraryOptions[0] = itin1;
        itineraryOptions[1] = itin2;
        itineraryOptions[2] = itin3;
        familyFriendly = famFriendly;
        coupleFriendly = coupFriendly;
        weather = wthr;
        visaInfo = visa;
        bestSeason = season;
        left = right = NULL;
    }
};

class PreferenceTree {
private:
    Destination* root;
    vector<Destination*> available;
    vector<Destination*> allDestinations;

    Destination* insert(Destination* node, string name, string type, string currency, 
                       double minb, double maxb, vector<string> attr, vector<string> fd,
                       vector<string> itin1, vector<string> itin2, vector<string> itin3,
                       bool famFriendly, bool coupFriendly, string weather, string visa, string season) {
        if (node == NULL) {
            Destination* newDest = new Destination(name, type, currency, minb, maxb, attr, fd, 
                                                 itin1, itin2, itin3, famFriendly, coupFriendly,
                                                 weather, visa, season);
            allDestinations.push_back(newDest);
            return newDest;
        }
        if (type == "National")
            node->left = insert(node->left, name, type, currency, minb, maxb, attr, fd, 
                              itin1, itin2, itin3, famFriendly, coupFriendly, weather, visa, season);
        else
            node->right = insert(node->right, name, type, currency, minb, maxb, attr, fd, 
                               itin1, itin2, itin3, famFriendly, coupFriendly, weather, visa, season);
        return node;
    }

    void collectDestinationsByBudgetRange(Destination* node, const string& selectedType, 
                                        double minRange, double maxRange,
                                        const string& tripPurpose, int adults, int children) {
        if (node == NULL) return;
        
        // Calculate total budget for all travelers (children at 70% cost)
        double minTotal = node->minbudget * adults + node->minbudget * children * 0.7;
        double maxTotal = node->maxbudget * adults + node->maxbudget * children * 0.7;
        
        if (node->type == selectedType && 
            ((minTotal >= minRange && minTotal <= maxRange) || 
             (maxTotal >= minRange && maxTotal <= maxRange))) {
            // Check if destination matches trip purpose
            if ((tripPurpose == "family" && node->familyFriendly) ||
                (tripPurpose == "couple" && node->coupleFriendly) ||
                (tripPurpose == "friends" || tripPurpose == "solo")) {
                available.push_back(node);
            }
        }
        
        collectDestinationsByBudgetRange(node->left, selectedType, minRange, maxRange, 
                                       tripPurpose, adults, children);
        collectDestinationsByBudgetRange(node->right, selectedType, minRange, maxRange, 
                                       tripPurpose, adults, children);
    }

public:
    PreferenceTree() { root = NULL; }

    void addDestination(string name, string type, string currency, double minbudget, double maxbudget,
                       vector<string> attr, vector<string> fd,
                       vector<string> itin1, vector<string> itin2, vector<string> itin3,
                       bool famFriendly, bool coupFriendly, string weather, string visa, string season) {
        root = insert(root, name, type, currency, minbudget, maxbudget, attr, fd, 
                     itin1, itin2, itin3, famFriendly, coupFriendly, weather, visa, season);
    }

    void showBudgetRanges(const string& tripType, const string& tripPurpose, int adults, int children) {
        cout << "\n--- Available Budget Ranges for " << tripType << " Trips ---\n";
        
        // Define budget ranges with clear labels
        vector<pair<string, pair<double, double>>> ranges;
        if (tripType == "National") {
            cout << "1. Budget (15,000-30,000 INR)\n";
            cout << "2. Mid-Range (30,000-50,000 INR)\n";
            cout << "3. Premium (50,000-80,000 INR)\n";
            ranges = {
                {"Budget (15,000-30,000 INR)", {15000, 30000}},
                {"Mid-Range (30,000-50,000 INR)", {30000, 50000}},
                {"Premium (50,000-80,000 INR)", {50000, 80000}}
            };
        } else {
            cout << "1. Budget (50,000-1,00,000 INR)\n";
            cout << "2. Mid-Range (1,00,000-2,00,000 INR)\n";
            cout << "3. Premium (2,00,000+ INR)\n";
            ranges = {
                {"Budget (50,000-1,00,000 INR)", {50000, 100000}},
                {"Mid-Range (1,00,000-2,00,000 INR)", {100000, 200000}},
                {"Premium (2,00,000+ INR)", {200000, 500000}}
            };
        }
        
        // Show destinations for each range
        for (size_t i = 0; i < ranges.size(); i++) {
            available.clear();
            collectDestinationsByBudgetRange(root, tripType, ranges[i].second.first, ranges[i].second.second, 
                                           tripPurpose, adults, children);
            
            if (!available.empty()) {
                cout << "\n" << (i+1) << ". " << ranges[i].first << ":\n";
                for (size_t j = 0; j < available.size(); j++) {
                    double minTotal = available[j]->minbudget * adults + available[j]->minbudget * children * 0.7;
                    double maxTotal = available[j]->maxbudget * adults + available[j]->maxbudget * children * 0.7;
                    
                    cout << "   " << (j+1) << ") " << available[j]->name << " (" 
                         << fixed << setprecision(0) << minTotal << " - " << maxTotal << " INR)\n";
                }
            }
        }
    }

    Destination* selectDestinationFromRange(const string& tripType, const string& tripPurpose, 
                                          int adults, int children, int rangeChoice) {
        // Define budget ranges based on choice
        pair<double, double> range;
        if (tripType == "National") {
            switch(rangeChoice) {
                case 1: range = {15000, 30000}; break;
                case 2: range = {30000, 50000}; break;
                case 3: range = {50000, 80000}; break;
                default: return nullptr;
            }
        } else {
            switch(rangeChoice) {
                case 1: range = {50000, 100000}; break;
                case 2: range = {100000, 200000}; break;
                case 3: range = {200000, 500000}; break;
                default: return nullptr;
            }
        }
        
        available.clear();
        collectDestinationsByBudgetRange(root, tripType, range.first, range.second, 
                                       tripPurpose, adults, children);
        
        if (available.empty()) {
            return nullptr;
        }
        
        cout << "\nAvailable destinations in your selected range:\n";
        for (int i = 0; i < available.size(); i++) {
            cout << (i+1) << ". " << available[i]->name << "\n";
        }
        
        cout << "Select a destination (1-" << available.size() << "): ";
        string choice;
        getline(cin, choice);
        
        try {
            int selected = stoi(choice) - 1;
            if (selected >= 0 && selected < available.size()) {
                return available[selected];
            }
        } catch (...) {
            return nullptr;
        }
        
        return nullptr;
    }

    Destination* getDestinationByName(const string& name) {
        for (auto dest : allDestinations) {
            if (toLower(dest->name) == toLower(name)) {
                return dest;
            }
        }
        return NULL;
    }

    void showDestinationDetails(Destination* dest, int adults, int children) {
        cout << "\n--- " << dest->name << " Destination Details ---\n";
        
        // Calculate total cost range
        double minTotal = dest->minbudget * adults + dest->minbudget * children * 0.7;
        double maxTotal = dest->maxbudget * adults + dest->maxbudget * children * 0.7;
        double avgForeignCost = convertCurrency((minTotal + maxTotal)/2, "INR", dest->currency);
        
        cout << "\nEstimated Total Cost: " << fixed << setprecision(2) 
             << minTotal << " - " << maxTotal << " INR (" 
             << avgForeignCost << " " << dest->currency << ")\n";
        
        cout << "\nBest Season to Visit: " << dest->bestSeason << endl;
        cout << "Typical Weather: " << dest->weather << endl;
        if (dest->type == "International") {
            cout << "Visa Information: " << dest->visaInfo << endl;
        }
        
        cout << "\nTop Attractions:\n";
        for (auto attr : dest->attractions) {
            cout << "- " << attr << "\n";
        }
        
        cout << "\nMust-Try Foods:\n";
        for (auto food : dest->foods) {
            cout << "- " << food << "\n";
        }
        
        cout << "\nItinerary Options:\n";
        for (int i = 0; i < 3; i++) {
            cout << "\nOption " << (i+1) << ":\n";
            for (auto day : dest->itineraryOptions[i]) {
                cout << "- " << day << "\n";
            }
        }
    }
};

// --------------------- Transport ---------------------
class Transport {
private:
    map<string, vector<pair<string, double>>> transportOptions;
    
public:
    Transport() {
        // Initialize transport options with prices (in INR)
        // National destinations
        transportOptions["jaipur"] = {{"Train", 1200}, {"Bus", 800}, {"Flight", 4500}};
        transportOptions["delhi"] = {{"Train", 1500}, {"Flight", 5000}};
        transportOptions["goa"] = {{"Bus", 1500}, {"Flight", 6000}};
        transportOptions["kerala"] = {{"Train", 1800}, {"Flight", 5500}};
        transportOptions["shimla"] = {{"Train", 2000}, {"Bus", 1200}};
        transportOptions["manali"] = {{"Bus", 1800}, {"Flight", 7000}};
        transportOptions["varanasi"] = {{"Train", 1300}, {"Flight", 5000}};
        transportOptions["mumbai"] = {{"Train", 1700}, {"Flight", 5500}};
        transportOptions["udaipur"] = {{"Train", 1400}, {"Bus", 900}, {"Flight", 5000}};
        transportOptions["darjeeling"] = {{"Train", 2200}, {"Flight", 6500}};
        transportOptions["agra"] = {{"Train", 1000}, {"Bus", 700}, {"Flight", 4000}};
        transportOptions["hyderabad"] = {{"Train", 1600}, {"Flight", 5000}};
        transportOptions["chennai"] = {{"Train", 1800}, {"Flight", 5500}};
        transportOptions["kolkata"] = {{"Train", 1500}, {"Flight", 5000}};
        transportOptions["amritsar"] = {{"Train", 1700}, {"Flight", 5500}};
        
        // International destinations
        transportOptions["vietnam"] = {{"Flight", 25000}};
        transportOptions["bali"] = {{"Flight", 30000}};
        transportOptions["turkey"] = {{"Flight", 40000}};
        transportOptions["dubai"] = {{"Flight", 22000}};
        transportOptions["cambodia"] = {{"Flight", 28000}};
        transportOptions["thailand"] = {{"Flight", 27000}};
        transportOptions["malaysia"] = {{"Flight", 32000}};
        transportOptions["singapore"] = {{"Flight", 30000}};
        transportOptions["japan"] = {{"Flight", 50000}};
        transportOptions["south korea"] = {{"Flight", 45000}};
        transportOptions["russia"] = {{"Flight", 38000}};
        transportOptions["france"] = {{"Flight", 42000}};
        transportOptions["italy"] = {{"Flight", 45000}};
        transportOptions["switzerland"] = {{"Flight", 48000}};
        transportOptions["spain"] = {{"Flight", 40000}};
        transportOptions["greece"] = {{"Flight", 38000}};
        transportOptions["portugal"] = {{"Flight", 39000}};
        transportOptions["australia"] = {{"Flight", 60000}};
    }
    
    void showTransportOptions(const string& destination, int adults, int children) {
        string lowerDest = toLower(destination);
        if (transportOptions.find(lowerDest) == transportOptions.end()) {
            cout << "\nNo transport options available for " << destination << ".\n";
            return;
        }
        
        cout << "\n--- Transport Options for " << destination << " ---\n";
        cout << "Mode\t\tAdult Fare\tChild Fare\tTotal (" << adults << "A," << children << "C)\n";
        cout << "-------------------------------------------------\n";
        
        for (auto option : transportOptions[lowerDest]) {
            string mode = option.first;
            double adultPrice = option.second;
            double childPrice = adultPrice * 0.7; // 30% discount for children
            double total = adultPrice * adults + childPrice * children;
            
            cout << mode << "\t\t" << adultPrice << " INR\t" << childPrice << " INR\t" 
                 << total << " INR\n";
        }
        
        cout << "\nWould you like to book transport? (y/n): ";
        string choice;
        getline(cin, choice);
        if (toLower(choice) == "y") {
            cout << "Enter transport mode: ";
            string mode;
            getline(cin, mode);
            
            bool found = false;
            for (auto option : transportOptions[lowerDest]) {
                if (toLower(option.first) == toLower(mode)) {
                    double adultPrice = option.second;
                    double childPrice = adultPrice * 0.7;
                    double total = adultPrice * adults + childPrice * children;
                    
                    cout << "\nBooking confirmed for " << mode << " to " << destination << "!\n";
                    cout << "Total cost: " << total << " INR\n";
                    found = true;
                    break;
                }
            }
            
            if (!found) {
                cout << "Invalid transport mode selected.\n";
            }
        }
    }
};

// --------------------- HotelAccommodation ---------------------
class HotelAccommodation {
private:
    struct Hotel {
        string name;
        string location; // City name
        string area;     // Specific area within city
        double distance; // Distance from city center in km
        double pricePerNight;
        bool familyFriendly;
        bool hasPool;
        bool hasRestaurant;
    };
    
    struct NearbyCity {
        string name;
        double distance; // Distance from main city in km
        vector<string> transportOptions;
        map<string, double> transportPrices; // Mode -> price
    };
    
    map<string, vector<Hotel>> hotels;
    map<string, vector<NearbyCity>> nearbyCities;
    
public:
    HotelAccommodation() {
        // Initialize hotels for each destination
        hotels["jaipur"] = {
            {"Raj Palace", "Jaipur", "City Center", 0, 4000, true, true, true},
            {"Jaipur Inn", "Jaipur", "Airport Area", 12, 3000, true, false, true},
            {"Heritage Haveli", "Jaipur", "Old City", 2, 5000, true, true, true}
        };
        
        hotels["goa"] = {
            {"Beach Resort", "Goa", "North Goa", 0, 5500, true, true, true},
            {"Goa Sands", "Goa", "South Goa", 25, 4000, true, true, true},
            {"Coastal Retreat", "Goa", "Central Goa", 15, 4500, true, true, true}
        };
        
        hotels["kerala"] = {
            {"Backwater Bliss", "Kerala", "Alleppey", 0, 5000, true, true, true},
            {"Tea Garden Resort", "Kerala", "Munnar", 30, 4500, true, false, true},
            {"Beachfront Villa", "Kerala", "Kovalam", 5, 6000, true, true, true}
        };
        
        hotels["shimla"] = {
            {"Mountain View", "Shimla", "Mall Road", 0, 4500, true, false, true},
            {"Snow Valley", "Shimla", "Chotta Shimla", 3, 4000, true, true, true},
            {"Himalayan Retreat", "Shimla", "Summer Hill", 5, 5000, true, true, true}
        };
        
        hotels["manali"] = {
            {"Apple Orchard Resort", "Manali", "Old Manali", 2, 5000, true, true, true},
            {"Snow Peak", "Manali", "Mall Road", 0, 4500, true, false, true},
            {"Valley View", "Manali", "Hidimba Temple Area", 1, 5500, true, true, true}
        };
        
        hotels["vietnam"] = {
            {"Hanoi Grand", "Hanoi", "Old Quarter", 0, 8000, true, true, true},
            {"Saigon Central", "Ho Chi Minh", "District 1", 0, 8500, true, true, true},
            {"Halong Bay View", "Halong", "Bay Area", 5, 9000, true, true, true}
        };
        
        hotels["bali"] = {
            {"Ubud Paradise", "Bali", "Ubud", 0, 10000, true, true, true},
            {"Kuta Beach Resort", "Bali", "Kuta", 0, 9500, true, true, true},
            {"Seminyak Luxury", "Bali", "Seminyak", 0, 12000, true, true, true}
        };
        
        hotels["turkey"] = {
            {"Istanbul Grand", "Istanbul", "Sultanahmet", 0, 11000, true, true, true},
            {"Cappadocia Cave", "Cappadocia", "Goreme", 0, 12000, true, true, true},
            {"Antalya Beach", "Antalya", "Konyaalti", 0, 10000, true, true, true}
        };
        
        // Initialize nearby cities with transport options
        nearbyCities["jaipur"] = {
            {"Ajmer", 130, {"Bus", "Taxi"}, {{"Bus", 500}, {"Taxi", 2500}}},
            {"Alwar", 150, {"Train", "Taxi"}, {{"Train", 300}, {"Taxi", 2000}}}
        };
        
        nearbyCities["goa"] = {
            {"Mumbai", 600, {"Flight", "Train", "Bus"}, {{"Flight", 4000}, {"Train", 1200}, {"Bus", 800}}},
            {"Pune", 450, {"Flight", "Bus"}, {{"Flight", 3500}, {"Bus", 600}}}
        };
    }
    
    void showHotelOptions(const string& destination, int adults, int children, int nights) {
        string lowerDest = toLower(destination);
        
        if (hotels.find(lowerDest) == hotels.end() || hotels[lowerDest].empty()) {
            cout << "\nNo hotel options available in " << destination << ".\n";
            if (nearbyCities.find(lowerDest) != nearbyCities.end()) {
                showNearbyOptions(destination, adults, children, nights);
            }
            return;
        }
        
        cout << "\n--- Hotel Options in " << destination << " ---\n";
        cout << "Option\tHotel\t\tLocation\tPrice/Night\tTotal (" << nights << " nights)\n";
        cout << "----------------------------------------------------------------\n";
        
        int optionNum = 1;
        for (auto hotel : hotels[lowerDest]) {
            double totalPrice = hotel.pricePerNight * nights;
            // Children stay free in existing beds
            if (adults + children > 2) {
                totalPrice += (max(0, adults + children - 2) * hotel.pricePerNight * 0.5);
            }
            
            cout << optionNum++ << "\t" << hotel.name << "\t" << hotel.location << "\t" 
                 << hotel.pricePerNight << " INR\t" << totalPrice << " INR\n";
            
            cout << "   Amenities: ";
            if (hotel.familyFriendly) cout << "Family-friendly ";
            if (hotel.hasPool) cout << "Pool ";
            if (hotel.hasRestaurant) cout << "Restaurant";
            cout << "\n";
        }
        
        cout << "\nWould you like to book a hotel? (y/n): ";
        string choice;
        getline(cin, choice);
        if (toLower(choice) == "y") {
            cout << "Enter hotel option number: ";
            string optionStr;
            getline(cin, optionStr);
            
            try {
                int option = stoi(optionStr) - 1;
                if (option >= 0 && option < hotels[lowerDest].size()) {
                    Hotel selected = hotels[lowerDest][option];
                    double totalPrice = selected.pricePerNight * nights;
                    if (adults + children > 2) {
                        totalPrice += (max(0, adults + children - 2) * selected.pricePerNight * 0.5);
                    }
                    
                    cout << "\nBooking confirmed at " << selected.name << "!\n";
                    cout << "Location: " << selected.location << "\n";
                    cout << "Total cost for " << nights << " nights: " << totalPrice << " INR\n";
                } else {
                    cout << "Invalid option selected.\n";
                }
            } catch (...) {
                cout << "Please enter a valid number.\n";
            }
        }
    }
    
    void showNearbyOptions(const string& destination, int adults, int children, int nights) {
        string lowerDest = toLower(destination);
        cout << "\nNo hotels available in " << destination << ", but here are nearby options:\n";
        
        for (auto& city : nearbyCities[lowerDest]) {
            cout << "\n--- " << city.name << " (" << city.distance << "km from " << destination << ") ---\n";
            
            // Show transport options
            cout << "Transport Options:\n";
            for (auto& transport : city.transportOptions) {
                double adultPrice = city.transportPrices[transport];
                double childPrice = adultPrice * 0.7;
                double totalTransport = adultPrice * adults + childPrice * children;
                
                cout << "- " << transport << ": " << adultPrice << " INR per adult ("
                     << childPrice << " INR per child) - Total: " << totalTransport << " INR\n";
            }
            
            // Show hotel options in nearby city
            if (hotels.find(toLower(city.name)) != hotels.end()) {
                cout << "\nAvailable Hotels in " << city.name << ":\n";
                int hotelNum = 1;
                for (auto& hotel : hotels[toLower(city.name)]) {
                    double totalPrice = hotel.pricePerNight * nights;
                    if (adults + children > 2) {
                        totalPrice += (max(0, adults + children - 2) * hotel.pricePerNight * 0.5);
                    }
                    
                    cout << hotelNum++ << ". " << hotel.name << " (" << hotel.area << ")\n"
                         << "   " << hotel.pricePerNight << " INR/night, Total: " << totalPrice << " INR\n"
                         << "   Amenities: ";
                    if (hotel.familyFriendly) cout << "Family-friendly ";
                    if (hotel.hasPool) cout << "Pool ";
                    if (hotel.hasRestaurant) cout << "Restaurant";
                    cout << "\n";
                }
            }
        }
    }
};

// --------------------- Main Function ---------------------
int main() {
    cout << "====== SMART WANDERPLAN - COMPREHENSIVE TRAVEL PLANNER ======\n";
    
    while (true) {
        CustomerDetails customer;
        if (!customer.inputDetails()) {
            cout << "Restarting input...\n";
            continue;
        }
        customer.displayDetails();

        PreferenceTree preferences;
        
        // ========== NATIONAL DESTINATIONS (20) ==========
        // 1. Jaipur
        preferences.addDestination(
            "Jaipur", "National", "INR", 5000, 8000,
            {"Amber Fort", "Hawa Mahal", "City Palace", "Jantar Mantar", "Nahargarh Fort"},
            {"Dal Baati Churma", "Laal Maas", "Ghewar", "Kachori", "Mawa Kachori"},
            {"Day 1: Arrival & City Palace", "Day 2: Amber Fort and markets", "Day 3: Hawa Mahal and departure"},
            {"Day 1: Heritage walk", "Day 2: Forts tour", "Day 3: Shopping and food trail"},
            {"Day 1: Arrival & Jantar Mantar", "Day 2: Combined palace tour", "Day 3: Relax and departure"},
            true, true, "Warm (15-30°C)", "Not required", "October-March"
        );
        
        // 2. Goa
        preferences.addDestination(
            "Goa", "National", "INR", 6000, 10000,
            {"Beaches", "Dudhsagar Falls", "Old Goa Churches", "Fort Aguada", "Spice Plantations"},
            {"Fish Curry Rice", "Bebinca", "Pork Vindaloo", "Feni", "Sorpotel"},
            {"Day 1: North Goa beaches", "Day 2: Dudhsagar Falls", "Day 3: Old Goa sightseeing"},
            {"Day 1: Beach hopping", "Day 2: Water sports", "Day 3: Relax and departure"},
            {"Day 1: Arrival & local food", "Day 2: South Goa beaches", "Day 3: Sightseeing"},
            true, true, "Humid (25-35°C)", "Not required", "November-February"
        );

        // 3-20. More national destinations
        vector<string> nationalDests = {
            "Kerala", "Shimla", "Manali", "Darjeeling", "Munnar", 
            "Udaipur", "Varanasi", "Amritsar", "Rishikesh", "Agra",
            "Mysore", "Puducherry", "Andaman", "Ooty", "Kodaikanal",
            "Khajuraho", "Ajanta-Ellora", "Hampi", "Leh-Ladakh", "Kashmir"
        };

        vector<vector<string>> nationalAttractions = {
            {"Backwaters", "Tea Gardens", "Alleppey Houseboat", "Kathakali Show"},
            {"Mall Road", "Jakhu Temple", "Kufri", "Toy Train"},
            {"Rohtang Pass", "Solang Valley", "Hadimba Temple", "Old Manali"},
            {"Tiger Hill", "Batasia Loop", "Tea Gardens", "Himalayan Railway"},
            {"Eravikulam Park", "Tea Museum", "Mattupetty Dam", "Echo Point"},
            {"City Palace", "Lake Pichola", "Jag Mandir", "Sajjangarh"},
            {"Ghats", "Kashi Vishwanath", "Sarnath", "Ganga Aarti"},
            {"Golden Temple", "Jallianwala Bagh", "Wagah Border", "Partition Museum"},
            {"River Rafting", "Beatles Ashram", "Laxman Jhula", "Ganga Aarti"},
            {"Taj Mahal", "Agra Fort", "Fatehpur Sikri", "Mehtab Bagh"},
            {"Mysore Palace", "Chamundi Hills", "Brindavan Gardens", "Zoo"},
            {"Promenade Beach", "Auroville", "Paradise Beach", "French Quarter"},
            {"Radhanagar Beach", "Cellular Jail", "Scuba Diving", "Limestone Caves"},
            {"Botanical Gardens", "Ooty Lake", "Doddabetta Peak", "Tea Factory"},
            {"Kodai Lake", "Pillar Rocks", "Coaker's Walk", "Bryant Park"},
            {"Khajuraho Temples", "Light & Sound Show", "Panna National Park"},
            {"Ajanta Caves", "Ellora Caves", "Bibi Ka Maqbara", "Grishneshwar Temple"},
            {"Virupaksha Temple", "Vittala Temple", "Elephant Stables", "Lotus Mahal"},
            {"Pangong Lake", "Nubra Valley", "Magnetic Hill", "Leh Palace"},
            {"Dal Lake", "Gulmarg", "Pahalgam", "Shalimar Garden"}
        };

        for(int i=0; i<nationalDests.size(); i++){
            preferences.addDestination(
                nationalDests[i], "National", "INR", 
                4000 + (i*1000), 8000 + (i*1500),
                nationalAttractions[i],
                {"Local Cuisine 1", "Local Cuisine 2", "Special Dessert"},
                {"Day 1: Arrival", "Day 2: Sightseeing", "Day 3: Departure"},
                {"Day 1: Relax", "Day 2: Adventure", "Day 3: Culture"},
                {"Day 1: Food Tour", "Day 2: Full day tour", "Day 3: Shopping"},
                true, true, "Varies", "Not required", "Varies"
            );
        }

        // ========== INTERNATIONAL DESTINATIONS (20) ==========
        // 1. Vietnam
        preferences.addDestination(
            "Vietnam", "International", "VND", 30000, 50000,
            {"Ha Long Bay", "Hoi An", "Hanoi Old Quarter", "Cu Chi Tunnels", "Mekong Delta"},
            {"Pho", "Banh Mi", "Bun Cha", "Goi Cuon", "Cao Lau"},
            {"Day 1: Hanoi arrival", "Day 2: Ha Long cruise", "Day 3: Hoi An"},
            {"Day 1: Food tour", "Day 2: Cu Chi Tunnels", "Day 3: Mekong"},
            {"Day 1: City tour", "Day 2: Countryside", "Day 3: Relax"},
            true, true, "Tropical (20-35°C)", "Visa required", "November-April"
        );

        // 2-20. More international destinations
        vector<string> intlDests = {
            "Bali", "Thailand", "Singapore", "Malaysia", "Dubai",
            "Turkey", "Japan", "South Korea", "France", "Italy",
            "Switzerland", "Spain", "Greece", "Portugal", "Australia",
            "New Zealand", "Maldives", "Sri Lanka", "Nepal", "Bhutan"
        };

        vector<vector<string>> intlAttractions = {
            {"Ubud", "Tanah Lot", "Uluwatu", "Tegallalang"},
            {"Bangkok", "Phuket", "Chiang Mai", "Phi Phi Islands"},
            {"Marina Bay", "Sentosa", "Gardens by the Bay", "Universal Studios"},
            {"Kuala Lumpur", "Langkawi", "Penang", "Cameron Highlands"},
            {"Burj Khalifa", "Palm Jumeirah", "Desert Safari", "Dubai Mall"},
            {"Hagia Sophia", "Cappadocia", "Pamukkale", "Ephesus"},
            {"Tokyo", "Kyoto", "Osaka", "Mount Fuji"},
            {"Seoul", "Busan", "Jeju Island", "DMZ"},
            {"Paris", "Nice", "Lyon", "French Riviera"},
            {"Rome", "Venice", "Florence", "Amalfi Coast"},
            {"Zurich", "Interlaken", "Lucerne", "Jungfraujoch"},
            {"Barcelona", "Madrid", "Seville", "Ibiza"},
            {"Athens", "Santorini", "Mykonos", "Crete"},
            {"Lisbon", "Porto", "Algarve", "Madeira"},
            {"Sydney", "Melbourne", "Great Barrier Reef", "Gold Coast"},
            {"Auckland", "Queenstown", "Rotorua", "Milford Sound"},
            {"Male", "Private Islands", "Underwater Restaurant"},
            {"Colombo", "Kandy", "Galle", "Sigiriya"},
            {"Kathmandu", "Pokhara", "Everest Base Camp", "Chitwan"},
            {"Paro", "Thimphu", "Punakha", "Tiger's Nest"}
        };

        vector<string> intlCurrencies = {
            "IDR", "THB", "SGD", "MYR", "AED",
            "TRY", "JPY", "KRW", "EUR", "EUR",
            "CHF", "EUR", "EUR", "EUR", "AUD",
            "NZD", "USD", "LKR", "NPR", "BTN"
        };

        for(int i=0; i<intlDests.size(); i++){
            preferences.addDestination(
                intlDests[i], "International", intlCurrencies[i],
                30000 + (i*5000), 60000 + (i*8000),
                intlAttractions[i],
                {"Local Specialty 1", "Local Specialty 2", "Famous Dish"},
                {"Day 1: Arrival", "Day 2: Main Attraction", "Day 3: Culture"},
                {"Day 1: Relax", "Day 2: Adventure", "Day 3: Food Tour"},
                {"Day 1: City Tour", "Day 2: Full day tour", "Day 3: Shopping"},
                true, true, "Varies", "Visa info varies", "Best season varies"
            );
        }

        // ========== TRIP PLANNING FLOW ==========
        cout << "\nSelect trip type:\n1. National\n2. International\n";
        string tripTypeChoice;
        getline(cin, tripTypeChoice);
        
        string tripType = (tripTypeChoice == "1") ? "National" : "International";
        
        preferences.showBudgetRanges(tripType, customer.tripPurpose, customer.adults, customer.children);
        
        cout << "\nSelect budget range (1-3): ";
        string rangeChoice;
        getline(cin, rangeChoice);
        
        Destination* selectedDest = preferences.selectDestinationFromRange(
            tripType, customer.tripPurpose, customer.adults, customer.children, stoi(rangeChoice));
        
        if (!selectedDest) {
            cout << "Invalid selection. Restarting...\n";
            continue;
        }
        
        preferences.showDestinationDetails(selectedDest, customer.adults, customer.children);
        
        Transport transport;
        transport.showTransportOptions(selectedDest->name, customer.adults, customer.children);
        
        HotelAccommodation hotels;
        cout << "\nEnter number of nights for stay: ";
        string nightsInput;
        getline(cin, nightsInput);
        hotels.showHotelOptions(selectedDest->name, customer.adults, customer.children, stoi(nightsInput));
        
        cout << "\n====== TRIP SUMMARY ======\n";
        cout << "Destination: " << selectedDest->name << "\nTravelers: " 
             << customer.adults << " adults, " << customer.children << " children\n";
        
        cout << "\nBook another trip? (y/n): ";
        string restartChoice;
        getline(cin, restartChoice);
        if (toLower(restartChoice) != "y") break;
    }
    
    cout << "\nThank you for using Smart WanderPlan!\n";
    return 0;
}