# Передатчик параметров
## Описание
Клиент-серверное графическое приложение, позволяющее в реальном времени вносить данные в заданном диапозоне (в серверной части) и непрерывно передавать их пакетами по протоколу UDP клиентской части для отображения. В интерфейсах обеих частей доступны настройки соединения.
## Компиляция
Компиляция осуществяелтся отдельно для клиента и сервера из среды разработки или из их корневых папок при помощи системы сборки CMake, с предустановленной средой разработки Qt версии 5.15 и выше.
Команда для компиляции системой сборки релизной версии (для каждой части отдельно):
```
cmake -B./build-release -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```
## Системные требования
* C++17 (STL)
* g++ с поддержкой 17-го стандарта (также, возможно применения иных компиляторов C++ с поддержкой необходимого стандарта)
* Qt 5.15
## Планы по доработке
* ~~Релизовать клиентскую часть~~
## Стек
* C++17
* CMake
* Qt
