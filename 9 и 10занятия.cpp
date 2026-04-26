#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>

//  РАЗДЕЛ 1: ТИПЫ ДАННЫХ

enum class Comfort {
    Luxury = 0,
    SemiLuxury = 1,
    Standard = 2,
    Economy = 3
};

struct Amenities {
    bool wifi;
    bool balcony;
    bool tv;
    bool minibar;
    bool airConditioning;
    bool parking;
};

struct HotelRoom {
    std::string hotelName;
    int         roomNumber;
    Comfort     comfort;
    int         capacity;
    double      price;
    Amenities   amenities;
};

//  РАЗДЕЛ 2: ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВЫВОДА

// Фиксированная ширина — колонки не «съезжают»
std::string comfortToStr(Comfort c) {
    switch (c) {
    case Comfort::Luxury:     return "Luxury    ";
    case Comfort::SemiLuxury: return "Semi-Lux  ";
    case Comfort::Standard:   return "Standard  ";
    case Comfort::Economy:    return "Economy   ";
    default:                  return "Unknown   ";
    }
}

// [+] / [-] одинаковой ширины — выравнивание не ломается
std::string flag(bool v) { return v ? "[+]" : "[-]"; }

void printAmenities(const Amenities& a) {
    std::cout
        << "    WiFi:" << flag(a.wifi)
        << "  Balcony:" << flag(a.balcony)
        << "  TV:" << flag(a.tv)
        << "  Minibar:" << flag(a.minibar)
        << "  A/C:" << flag(a.airConditioning)
        << "  Parking:" << flag(a.parking) << "\n";
}

// Перечисляет отсутствующие опции через запятую
void printMissingAmenities(const Amenities& a) {
    bool any = false;
    auto comma = [&]() { if (any) std::cout << ", "; any = true; };
    if (!a.wifi) { comma(); std::cout << "Wi-Fi"; }
    if (!a.balcony) { comma(); std::cout << "Balcony"; }
    if (!a.tv) { comma(); std::cout << "TV"; }
    if (!a.minibar) { comma(); std::cout << "Minibar"; }
    if (!a.airConditioning) { comma(); std::cout << "A/C"; }
    if (!a.parking) { comma(); std::cout << "Parking"; }
    if (!any) std::cout << "all options available";
    std::cout << "\n";
}

// Горизонтальный разделитель
void printSeparator(char ch = '-', int width = 72) {
    std::cout << std::string(width, ch) << "\n";
}

// Шапка таблицы с именами колонок
void printTableHeader() {
    printSeparator();
    std::cout
        << std::left
        << std::setw(5) << "  #"
        << std::setw(18) << "Hotel"
        << std::setw(6) << "Room"
        << std::setw(12) << "Comfort"
        << std::setw(6) << "Pax"
        << "Price (rub./night)\n";
    printSeparator();
}

// Одна строка таблицы (idx = -1 → без номера)
void printRoomLine(const HotelRoom& r, int idx = -1) {
    if (idx >= 0)
        std::cout << std::right << std::setw(3) << idx << ". ";
    else
        std::cout << "     ";

    std::cout
        << std::left << std::setw(18) << r.hotelName
        << std::right << std::setw(5) << r.roomNumber << "  "
        << std::left << std::setw(12) << comfortToStr(r.comfort)
        << std::right << std::setw(3) << r.capacity << " pax  "
        << std::fixed << std::setprecision(2)
        << std::setw(10) << r.price << "\n";
    printAmenities(r.amenities);
}

//  РАЗДЕЛ 3: ВЫВОД ОТФИЛЬТРОВАННОГО СПИСКА

void printFiltered(const std::vector<HotelRoom>& arr, const std::string& title) {
    std::cout << "\n";
    printSeparator('=');
    std::cout << "  " << title << "  [" << arr.size() << " entries]\n";
    printSeparator('=');
    if (arr.empty()) { std::cout << "  (no entries)\n"; return; }
    printTableHeader();
    for (size_t i = 0; i < arr.size(); ++i)
        printRoomLine(arr[i], (int)(i + 1));
    printSeparator();
}

//  РАЗДЕЛ 4: ФИЛЬТРАЦИЯ

bool endsWith(const std::string& str, const std::string& suffix) {
    if (suffix.size() > str.size()) return false;
    return str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::vector<HotelRoom> filterByHotelSuffix(const std::vector<HotelRoom>& src) {
    std::vector<HotelRoom> result;
    for (const auto& r : src)
        if (endsWith(r.hotelName, "hotel"))
            result.push_back(r);
    return result;
}

std::vector<HotelRoom> filterByBalcony(const std::vector<HotelRoom>& src) {
    std::vector<HotelRoom> result;
    for (const auto& r : src)
        if (r.amenities.balcony)
            result.push_back(r);
    return result;
}

//  РАЗДЕЛ 5: СОРТИРОВКА ПУЗЫРЬКОМ (по возрастанию стоимости)

void bubbleSortByPrice(std::vector<HotelRoom>& arr) {
    bool swapped;
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        swapped = false;
        for (size_t j = 0; j < n - 1 - i; ++j) {
            if (arr[j].price > arr[j + 1].price) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

//  РАЗДЕЛ 6: ПОИСК И ДЕТАЛЬНЫЙ ВЫВОД

void printRoomByNumber(const std::vector<HotelRoom>& arr, int roomNumber) {
    for (const auto& r : arr) {
        if (r.roomNumber == roomNumber) {
            printSeparator();
            std::cout
                << "  Hotel   : " << r.hotelName << "\n"
                << "  Room    : " << r.roomNumber << "\n"
                << "  Comfort : " << comfortToStr(r.comfort) << "\n"
                << "  Guests  : " << r.capacity << " pax\n"
                << "  Price   : " << std::fixed << std::setprecision(2)
                << r.price << " rub./night\n"
                << "  Options :\n";
            printAmenities(r.amenities);
            printSeparator();
            return;
        }
    }
    std::cout << "  Room #" << roomNumber << " — not found.\n";
}

void printCheapest3(const std::vector<HotelRoom>& arr) {
    size_t k = std::min(arr.size(), size_t(3));
    printSeparator();
    for (size_t i = 0; i < k; ++i) {
        std::cout
            << "  " << (i + 1) << ". "
            << std::left << std::setw(18) << arr[i].hotelName
            << "  #" << arr[i].roomNumber
            << "  " << std::fixed << std::setprecision(2)
            << arr[i].price << " rub./night\n"
            << "     Missing: ";
        printMissingAmenities(arr[i].amenities);
    }
    printSeparator();
}

//  РАЗДЕЛ 7: ИЗМЕНЕНИЕ ДАННЫХ НОМЕРА

bool modifyRoom(std::vector<HotelRoom>& arr, int roomNumber,
    double newPrice, int newCapacity, Comfort newComfort) {
    for (auto& r : arr) {
        if (r.roomNumber == roomNumber) {
            printSeparator();
            std::cout << "  [Before]  Room #" << r.roomNumber
                << "  " << comfortToStr(r.comfort)
                << "  " << r.capacity << " pax"
                << "  " << std::fixed << std::setprecision(2)
                << r.price << " rub.\n";
            r.price = newPrice;
            r.capacity = newCapacity;
            r.comfort = newComfort;
            std::cout << "  [After ]  Room #" << r.roomNumber
                << "  " << comfortToStr(r.comfort)
                << "  " << r.capacity << " pax"
                << "  " << std::fixed << std::setprecision(2)
                << r.price << " rub.\n";
            printSeparator();
            return true;
        }
    }
    std::cout << "  [modifyRoom] Room #" << roomNumber << " — not found.\n";
    return false;
}

//  РАЗДЕЛ 8: РАБОТА С ФАЙЛАМИ

void generateTestTextFile(const std::string& filename) {
    std::ofstream fout(filename);
    if (fout.is_open()) {
        fout << "101 20000.0\n";
        fout << "202 4000.0\n";
        fout << "701 5500.0\n";
        fout << "1601 32000.0\n";
        fout.close();
    }
}

void updatePriceFromTextFile(std::vector<HotelRoom>& rooms, const std::string& filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "Error opening text file for reading!\n";
        return;
    }

    int targetRoom;
    double newPrice;

    while (fin >> targetRoom >> newPrice) {
        for (auto& r : rooms) {
            if (r.roomNumber == targetRoom) {
                r.price = newPrice;
                break;
            }
        }
    }
    fin.close();
    std::cout << "  [+] Data successfully updated from text file: " << filename << "\n";
}

void saveToBinaryFile(const std::vector<HotelRoom>& rooms, const std::string& filename) {
    std::ofstream fout(filename, std::ios::binary);
    if (!fout.is_open()) {
        std::cout << "Error opening binary file for writing!\n";
        return;
    }

    size_t count = rooms.size();
    fout.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& r : rooms) {
        size_t len = r.hotelName.size();
        fout.write(reinterpret_cast<const char*>(&len), sizeof(len));
        fout.write(r.hotelName.c_str(), len);

        fout.write(reinterpret_cast<const char*>(&r.roomNumber), sizeof(r.roomNumber));
        fout.write(reinterpret_cast<const char*>(&r.comfort), sizeof(r.comfort));
        fout.write(reinterpret_cast<const char*>(&r.capacity), sizeof(r.capacity));
        fout.write(reinterpret_cast<const char*>(&r.price), sizeof(r.price));
        fout.write(reinterpret_cast<const char*>(&r.amenities), sizeof(r.amenities));
    }
    fout.close();
    std::cout << "  [+] Data successfully saved to binary file: " << filename << "\n";
}

void loadFromBinaryFile(std::vector<HotelRoom>& rooms, const std::string& filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        std::cout << "Error opening binary file for reading!\n";
        return;
    }

    size_t count = 0;
    fin.read(reinterpret_cast<char*>(&count), sizeof(count));
    rooms.resize(count);

    for (size_t i = 0; i < count; ++i) {
        size_t len = 0;
        fin.read(reinterpret_cast<char*>(&len), sizeof(len));
        rooms[i].hotelName.resize(len);
        fin.read(&rooms[i].hotelName[0], len);

        fin.read(reinterpret_cast<char*>(&rooms[i].roomNumber), sizeof(rooms[i].roomNumber));
        fin.read(reinterpret_cast<char*>(&rooms[i].comfort), sizeof(rooms[i].comfort));
        fin.read(reinterpret_cast<char*>(&rooms[i].capacity), sizeof(rooms[i].capacity));
        fin.read(reinterpret_cast<char*>(&rooms[i].price), sizeof(rooms[i].price));
        fin.read(reinterpret_cast<char*>(&rooms[i].amenities), sizeof(rooms[i].amenities));
    }
    fin.close();
    std::cout << "  [+] Data successfully loaded from binary file: " << filename << "\n";
}

//  РАЗДЕЛ 9: MAIN

int main() {
    setlocale(LC_ALL, "");

    std::vector<HotelRoom> rooms = {
        // Оканчиваются на "hotel"
        {"Grand hotel",   101, Comfort::Luxury,     2, 15000.0, {true,  true,  true,  true,  true,  true }},
        {"Grand hotel",   102, Comfort::SemiLuxury, 2, 10500.0, {true,  false, true,  true,  true,  false}},
        {"Royal hotel",   201, Comfort::Standard,   3,  6500.0, {true,  true,  true,  false, true,  false}},
        {"Royal hotel",   202, Comfort::Economy,    4,  3200.0, {true,  false, true,  false, false, false}},
        {"City hotel",    301, Comfort::Luxury,     2, 18000.0, {true,  true,  true,  true,  true,  true }},
        {"Park hotel",    401, Comfort::Standard,   2,  5800.0, {true,  true,  true,  false, true,  true }},
        {"Park hotel",    402, Comfort::Economy,    3,  2900.0, {false, false, true,  false, false, false}},
        {"Plaza hotel",   501, Comfort::SemiLuxury, 2, 11500.0, {true,  true,  true,  true,  false, true }},
        {"Central hotel", 601, Comfort::Standard,   4,  7200.0, {true,  false, true,  false, true,  true }},
        {"Sunrise hotel", 701, Comfort::Economy,    2,  3800.0, {true,  false, false, false, true,  false}},
        // Другие названия
        {"Marriott",      801, Comfort::Luxury,     2, 22000.0, {true,  true,  true,  true,  true,  true }},
        {"Hilton",        901, Comfort::SemiLuxury, 2, 13500.0, {true,  true,  true,  true,  true,  false}},
        {"Sheraton",     1001, Comfort::Standard,   3,  8100.0, {true,  false, true,  false, true,  true }},
        {"Radisson Blu", 1101, Comfort::Economy,    4,  4200.0, {true,  false, true,  false, false, false}},
        {"Novotel",      1201, Comfort::Standard,   2,  6900.0, {true,  true,  true,  false, true,  false}},
        {"Ibis",         1301, Comfort::Economy,    2,  2500.0, {true,  false, false, false, true,  false}},
        {"InterConti",   1401, Comfort::Luxury,     2, 25000.0, {true,  true,  true,  true,  true,  true }},
        {"Hyatt",        1501, Comfort::SemiLuxury, 3, 14000.0, {true,  true,  true,  true,  false, true }},
        {"Ritz-Carlton", 1601, Comfort::Luxury,     2, 30000.0, {true,  true,  true,  true,  true,  true }},
        {"Crowne Plaza", 1701, Comfort::Standard,   4,  7500.0, {true,  true,  true,  false, true,  true }},
    };

    // ШАГ 1: Исходный массив
    printFiltered(rooms, "SOURCE ARRAY (20 rooms)");

    // ШАГ 2: Фильтрация по суффиксу "hotel"
    std::vector<HotelRoom> hotelArr = filterByHotelSuffix(rooms);
    printFiltered(hotelArr, "Hotels ending with 'hotel'");

    // ШАГ 3: Сортировка пузырьком по возрастанию стоимости
    bubbleSortByPrice(hotelArr);
    printFiltered(hotelArr, "Sorted by price (ascending)");

    // ШАГ 4: Вывод данных по конкретному номеру
    std::cout << "\n=== Room lookup: #401 ===\n";
    printRoomByNumber(hotelArr, 401);

    // ШАГ 5: Три самых дешёвых + отсутствующие опции
    std::cout << "\n=== 3 cheapest rooms (missing amenities) ===\n";
    printCheapest3(hotelArr);

    // ШАГ 6: Изменение данных номера 402
    std::cout << "\n=== Modifying room #402 ===\n";
    modifyRoom(hotelArr, 402, 3500.0, 4, Comfort::Standard);

    // ШАГ 7: Номера с балконом
    std::vector<HotelRoom> balconyArr = filterByBalcony(hotelArr);
    printFiltered(balconyArr, "Rooms with balcony (from 'hotel'-filtered list, after sorting)");

    // Задание 1: Текстовый файл
    std::cout << "\n=== TASK 1: Updating prices from text file ===\n";
    std::string txtFile = "price_updates.txt";
    generateTestTextFile(txtFile); // Создаем файл
    updatePriceFromTextFile(rooms, txtFile);
    printFiltered(rooms, "ARRAY AFTER TEXT FILE UPDATE (Check rooms 101, 202, 701, 1601)");

    // Задание 2: Бинарный файл
    std::cout << "\n=== TASK 2: Binary file I/O ===\n";
    std::string binFile = "hotel_data.bin";

    saveToBinaryFile(rooms, binFile); // Сохраняем весь измененный массив в бинарник

    // Очищаем оригинальный массив
    rooms.clear();
    std::cout << "  [i] Original array cleared. Current size: " << rooms.size() << "\n";

    // Считываем все обратно
    loadFromBinaryFile(rooms, binFile);
    printFiltered(rooms, "FINAL ARRAY LOADED FROM BINARY FILE");
}