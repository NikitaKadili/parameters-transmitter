# Передатчик параметров
## Описание
Клиент-серверное приложение, позволяющее в реальном времени вносить данные в заданном диапозоне (в серверной части) и непрерывно передавать их пакетами клиентской части для отображения по протоколу UDP.
## Компиляция
Компиляция осуществяелтся отдельно из корневых папок клиента и сервера при помощи CMake, с предустановленной средой разработки Qt версии 5.15 и выше.
## Системные требования
* C++17 (STL)
* g++ с поддержкой 17-го стандарта (также, возможно применения иных компиляторов C++ с поддержкой необходимого стандарта)
* Qt 5.15
## Планы по доработке
* Релизовать клиентскую часть
## Стек
* C++17
* CMake
* Qt
