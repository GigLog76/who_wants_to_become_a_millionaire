-Кто хочет стать миллионером?-

Игра-викторина на C++ (SFML 3.0)

---

**Сборка и запуск**

#1. Установите зависимости
- SFML 3.0
- nlohmann/json (заголовочный файл json.hpp)
- Компилятор C++17

#2. Сборка через CMake (рекомендуется)
bash
mkdir build && cd build
cmake .. -DSFML_DIR=C:/SFML/lib/cmake/SFML   # Windows (укажите свой путь)
make -j$(nproc)                              # Linux/macOS
cmake --build . --config Release             # Windows (без make)

#3 Ручная компиляция (MinGW)
g++ -std=c++17 -Iinclude -IC:/SFML/include -LC:/SFML/lib \
    src/*.cpp -o Millionaire.exe \
    -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

#4 Запуск
1.Скопируйте папку resources/ и questions.json в папку с исполняемым файлом.
2.Для Windows скопируйте все DLL из папки SFML/bin рядом с .exe.
3.Запустите ./Millionaire.exe (Windows) или ./Millionaire (Linux/macOS).
