#include <iostream>
#include <fstream>
#include <cstring> // Для strlen, strcmp
// РАЗДЕЛ 1: ТИПЫ ДАННЫХ
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
    char hotelName[50];
    int         roomNumber;
    Comfort     comfort;
    int         capacity;
    double      price;
    Amenities   amenities;
};

// РАЗДЕЛ 2: ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ВЫВОДА
void printPadded(const char* str, int width) {
    int len = std::strlen(str);
    std::cout << str;
    for (int i = len; i < width; ++i) {
        std::cout << ' ';
    }
}

// Функция для печати чисел с фиксированной шириной
void printPaddedInt(int num, int width) {
    int temp = num;
    int len = (num == 0) ? 1 : 0;
    while (temp > 0) {
        len++;
        temp /= 10;
    }
    for (int i = len; i < width; ++i) std::cout << ' ';
    std::cout << num;
}

const char* comfortToStr(Comfort c) {
    switch (c) {
    case Comfort::Luxury:     return "Luxury    ";
    case Comfort::SemiLuxury: return "Semi-Lux  ";
    case Comfort::Standard:   return "Standard  ";
    case Comfort::Economy:    return "Economy   ";
    default:                  return "Unknown   ";
    }
}

const char* flag(bool v) { return v ? "[+]" : "[-]"; }

void printAmenities(const Amenities& a) {
    std::cout
        << "    WiFi:" << flag(a.wifi)
        << "  Balcony:" << flag(a.balcony)
        << "  TV:" << flag(a.tv)
        << "  Minibar:" << flag(a.minibar)
        << "  A/C:" << flag(a.airConditioning)
        << "  Parking:" << flag(a.parking) << "\n";
}

void printMissingAmenities(const Amenities& a) {
    bool any = false;
    if (!a.wifi) { if (any) std::cout << ", "; std::cout << "Wi-Fi"; any = true; }
    if (!a.balcony) { if (any) std::cout << ", "; std::cout << "Balcony"; any = true; }
    if (!a.tv) { if (any) std::cout << ", "; std::cout << "TV"; any = true; }
    if (!a.minibar) { if (any) std::cout << ", "; std::cout << "Minibar"; any = true; }
    if (!a.airConditioning) { if (any) std::cout << ", "; std::cout << "A/C"; any = true; }
    if (!a.parking) { if (any) std::cout << ", "; std::cout << "Parking"; any = true; }
    if (!any) std::cout << "all options available";
    std::cout << "\n";
}

void printSeparator(char ch = '-', int width = 72) {
    for (int i = 0; i < width; ++i) std::cout << ch;
    std::cout << "\n";
}

void printTableHeader() {
    printSeparator();
    std::cout << "  #  ";
    printPadded("Hotel", 18);
    printPadded("Room", 8);
    printPadded("Comfort", 12);
    printPadded("Pax", 6);
    std::cout << "Price (rub./night)\n";
    printSeparator();
}

void printRoomLine(const HotelRoom& r, int idx = -1) {
    if (idx >= 0) {
        printPaddedInt(idx, 3);
        std::cout << ". ";
    }
    else {
        std::cout << "     ";
    }

    printPadded(r.hotelName, 18);
    printPaddedInt(r.roomNumber, 5);
    std::cout << "   ";
    printPadded(comfortToStr(r.comfort), 12);
    printPaddedInt(r.capacity, 2);
    std::cout << " pax  " << r.price << "\n";
    printAmenities(r.amenities);
}

// РАЗДЕЛ 3: ВЫВОД ОТФИЛЬТРОВАННОГО СПИСКА (ОБЕРТОЧНАЯ ФУНКЦИЯ)

void printFiltered(const HotelRoom arr[], int size, const char* title) {
    std::cout << "\n";
    printSeparator('=');
    std::cout << "  " << title << "  [" << size << " entries]\n";
    printSeparator('=');
    if (size == 0) { std::cout << "  (no entries)\n"; return; }
    printTableHeader();
    for (int i = 0; i < size; ++i) {
        printRoomLine(arr[i], i + 1);
    }
    printSeparator();
}

// РАЗДЕЛ 4: ФИЛЬТРАЦИЯ

bool endsWith(const char* str, const char* suffix) {
    int strLen = std::strlen(str);
    int suffLen = std::strlen(suffix);
    if (suffLen > strLen) return false;
    return std::strcmp(str + strLen - suffLen, suffix) == 0;
}

void filterByHotelSuffix(const HotelRoom src[], int srcSize, HotelRoom dest[], int& destSize) {
    destSize = 0;
    for (int i = 0; i < srcSize; ++i) {
        if (endsWith(src[i].hotelName, "hotel")) {
            dest[destSize] = src[i];
            destSize++;
        }
    }
}

void filterByBalcony(const HotelRoom src[], int srcSize, HotelRoom dest[], int& destSize) {
    destSize = 0;
    for (int i = 0; i < srcSize; ++i) {
        if (src[i].amenities.balcony) {
            dest[destSize] = src[i];
            destSize++;
        }
    }
}

// РАЗДЕЛ 5: СОРТИРОВКА ПУЗЫРЬКОМ

void bubbleSortByPrice(HotelRoom arr[], int size) {
    for (int i = 0; i < size - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < size - 1 - i; ++j) {
            if (arr[j].price > arr[j + 1].price) {
                HotelRoom temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

// РАЗДЕЛ 6: ПОИСК И ДЕТАЛЬНЫЙ ВЫВОД

void printRoomByNumber(const HotelRoom arr[], int size, int roomNumber) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].roomNumber == roomNumber) {
            printSeparator();
            std::cout
                << "  Hotel   : " << arr[i].hotelName << "\n"
                << "  Room    : " << arr[i].roomNumber << "\n"
                << "  Comfort : " << comfortToStr(arr[i].comfort) << "\n"
                << "  Guests  : " << arr[i].capacity << " pax\n"
                << "  Price   : " << arr[i].price << " rub./night\n"
                << "  Options :\n";
            printAmenities(arr[i].amenities);
            printSeparator();
            return;
        }
    }
    std::cout << "  Room #" << roomNumber << " — not found.\n";
}

void printCheapest3(const HotelRoom arr[], int size) {
    int k = (size < 3) ? size : 3;
    printSeparator();
    for (int i = 0; i < k; ++i) {
        std::cout << "  " << (i + 1) << ". ";
        printPadded(arr[i].hotelName, 18);
        std::cout << "  #" << arr[i].roomNumber << "  " << arr[i].price << " rub./night\n"
                  << "     Missing: ";
        printMissingAmenities(arr[i].amenities);
    }
    printSeparator();
}

// РАЗДЕЛ 7: ИЗМЕНЕНИЕ ДАННЫХ НОМЕРА

bool modifyRoom(HotelRoom arr[], int size, int roomNumber,
    double newPrice, int newCapacity, Comfort newComfort) {
    for (int i = 0; i < size; ++i) {
        if (arr[i].roomNumber == roomNumber) {
            printSeparator();
            std::cout << "  [Before]  Room #" << arr[i].roomNumber
                << "  " << comfortToStr(arr[i].comfort)
                << "  " << arr[i].capacity << " pax"
                << "  " << arr[i].price << " rub.\n";
            
            arr[i].price = newPrice;
            arr[i].capacity = newCapacity;
            arr[i].comfort = newComfort;
            
            std::cout << "  [After ]  Room #" << arr[i].roomNumber
                << "  " << comfortToStr(arr[i].comfort)
                << "  " << arr[i].capacity << " pax"
                << "  " << arr[i].price << " rub.\n";
            printSeparator();
            return true;
        }
    }
    std::cout << "  [modifyRoom] Room #" << roomNumber << " — not found.\n";
    return false;
}

// РАЗДЕЛ 8: РАБОТА С ФАЙЛАМИ

void generateTestTextFile(const char* filename) {
    std::ofstream fout(filename);
    if (fout.is_open()) {
        fout << "101 20000.0\n";
        fout << "202 4000.0\n";
        fout << "701 5500.0\n";
        fout << "1601 32000.0\n";
        fout.close();
    }
}

void updatePriceFromTextFile(HotelRoom rooms[], int size, const char* filename) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "Error opening text file for reading!\n";
        return;
    }

    int targetRoom;
    double newPrice;

    while (fin >> targetRoom >> newPrice) {
        for (int i = 0; i < size; ++i) {
            if (rooms[i].roomNumber == targetRoom) {
                rooms[i].price = newPrice;
                break;
            }
        }
    }
    fin.close();
    std::cout << "  [+] Data successfully updated from text file: " << filename << "\n";
}

void saveToBinaryFile(const HotelRoom rooms[], int size, const char* filename) {
    std::ofstream fout(filename, std::ios::binary);
    if (!fout.is_open()) {
        std::cout << "Error opening binary file for writing!\n";
        return;
    }

    // Записываем количество элементов
    fout.write(reinterpret_cast<const char*>(&size), sizeof(size));
    fout.write(reinterpret_cast<const char*>(rooms), size * sizeof(HotelRoom));
    
    fout.close();
    std::cout << "  [+] Data successfully saved to binary file: " << filename << "\n";
}

void loadFromBinaryFile(HotelRoom rooms[], int& size, const char* filename) {
    std::ifstream fin(filename, std::ios::binary);
    if (!fin.is_open()) {
        std::cout << "Error opening binary file for reading!\n";
        return;
    }

    // Считываем количество
    fin.read(reinterpret_cast<char*>(&size), sizeof(size));
    // Считываем весь массив целиком
    fin.read(reinterpret_cast<char*>(rooms), size * sizeof(HotelRoom));
    
    fin.close();
    std::cout << "  [+] Data successfully loaded from binary file: " << filename << "\n";
}

// РАЗДЕЛ 9: MAIN

int main() {
    setlocale(LC_ALL, "");

    const int MAX_ROOMS = 50; // Максимальный размер массивов
    int roomsSize = 20;

    HotelRoom rooms[MAX_ROOMS] = {
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
    printFiltered(rooms, roomsSize, "SOURCE ARRAY (20 rooms)");

    // ШАГ 2: Фильтрация по суффиксу "hotel"
    HotelRoom hotelArr[MAX_ROOMS];
    int hotelArrSize = 0;
    filterByHotelSuffix(rooms, roomsSize, hotelArr, hotelArrSize);
    printFiltered(hotelArr, hotelArrSize, "Hotels ending with 'hotel'");

    // ШАГ 3: Сортировка пузырьком по возрастанию стоимости
    bubbleSortByPrice(hotelArr, hotelArrSize);
    printFiltered(hotelArr, hotelArrSize, "Sorted by price (ascending)");

    // ШАГ 4: Вывод данных по конкретному номеру
    std::cout << "\n=== Room lookup: #401 ===\n";
    printRoomByNumber(hotelArr, hotelArrSize, 401);

    // ШАГ 5: Три самых дешёвых + отсутствующие опции
    std::cout << "\n=== 3 cheapest rooms (missing amenities) ===\n";
    printCheapest3(hotelArr, hotelArrSize);

    // ШАГ 6: Изменение данных номера 402
    std::cout << "\n=== Modifying room #402 ===\n";
    modifyRoom(hotelArr, hotelArrSize, 402, 3500.0, 4, Comfort::Standard);

    // ШАГ 7: Номера с балконом
    HotelRoom balconyArr[MAX_ROOMS];
    int balconyArrSize = 0;
    filterByBalcony(hotelArr, hotelArrSize, balconyArr, balconyArrSize);
    printFiltered(balconyArr, balconyArrSize, "Rooms with balcony (from 'hotel'-filtered list)");

    // Задание 1: Текстовый файл
    std::cout << "\n=== TASK 1: Updating prices from text file ===\n";
    const char* txtFile = "price_updates.txt";
    generateTestTextFile(txtFile); // Создаем файл
    updatePriceFromTextFile(rooms, roomsSize, txtFile);
    printFiltered(rooms, roomsSize, "ARRAY AFTER TEXT FILE UPDATE (Check rooms 101, 202, 701, 1601)");

    // Задание 2: Бинарный файл
    std::cout << "\n=== TASK 2: Binary file I/O ===\n";
    const char* binFile = "hotel_data.bin";

    saveToBinaryFile(rooms, roomsSize, binFile);

    // Имитируем очистку оригинального массива
    roomsSize = 0;
    std::cout << "  [i] Original array 'cleared'. Current size: " << roomsSize << "\n";

    // Считываем все обратно
    loadFromBinaryFile(rooms, roomsSize, binFile);
    printFiltered(rooms, roomsSize, "FINAL ARRAY LOADED FROM BINARY FILE");

    return 0;
}
