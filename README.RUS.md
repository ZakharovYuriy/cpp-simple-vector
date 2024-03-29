# cpp-simple-vector
Разработка собственного контейнера — вектор

Читать на других языках: [English](README.md), [Русский](README.RUS.md)<br>

# Описание программы
Это сильно упрощённый аналог стандартного контейнера **vector**, со сходной структурой и функционалом. Для лучшего понимания устройства и особенностей стандартного **vector**.

# Сборка при помощи Cmake
To build this project on linux you need:<br>
1) If you don't have Cmake installed, install Cmake<br>
2) If the "Debug" or "Release" folders are not created:<br>

```
mkdir Debug
mkdir Release
```
3) Run the command for Debug and/or Release conf:<br>

```
cmake -E chdir Debug/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Debug
cmake -E chdir Release/ cmake -G "Unix Makefiles" ../ -DCMAKE_BUILD_TYPE:STRING=Release 
```
4) Go to "Debug" or "Release" folder and build:<br>

```
cmake --build .
```

5) To **Run** program - in the debug or release folder run:<br>

```
./simple_vector
```
# Системые требования:
  1. C++17(STL)
  2. GCC (MinG w64) 11.2.0  

# Стек технологий:
  1. Copy elision и NRVO ( Named Return Value Optimization )
  2. move - семантика
  3. rvalue-ссылки
  4. Три уровня безопасности исключений<br>

## Перед тем как начать:
  0. Установка и настройкка всех требуемых компонентов в среде разработки длля запуска приложения<br>
  1. Вариант использования и тесты показаны в main.cpp.<br>

# Подробное описание проекта:
В основе `SimpleVector` — массив в динамической памяти. Сам контейнер хранит лишь адрес начала массива, а также информацию о его текущем размере и вместимости:<br>
**Размер** — количество элементов, содержащееся в массиве;<br>
**Вместимость** — максимальное количество элементов, которые могут поместиться в массив.<br>
`SimpleVector` - поддерживает семантику перемещения.<br>
<br>
Можно провести аналогию с ведром воды — `capacity_` определяет максимальный объём воды, которая помещается в ведро, а `size_` — текущее количество воды в ведре.<br>
Сконструированный по умолчанию вектор имеет нулевой размер и вместимость, а указатель на массив равен **nullptr**.<br>
<br>
Как и `std::vector`, класс `SimpleVector` может изменять свой размер в сторону увеличения и уменьшения. Для этого служит метод `Resize`<br>
<br>
Очистку `SimpleVector` выполняет метод `Clear`. Он не изменяет его вместимость и семантически эквивалентен вызову `Resize` с аргументом, равным нулю. Метод `Clear` не выбрасывает исключений.<br>

# Шаблонный класс SimpleVector<Type> - 
**упрощённй аналог контейнера vector, со следующим функционалом:**<br>
 - Конструктор копирования. Копия вектора должна иметь вместимость, достаточную для хранения копии элементов исходного вектора.<br>
 - По умолчанию. Создаёт пустой вектор с нулевой вместимостью. Не выделяет динамическую память и не выбрасывает исключений.<br>
 - Параметризованный конструктор, создающий вектор заданного размера. Элементы вектора инициализированы значением по умолчанию для типа Type. Вектор должен иметь одинаковые размер и вместимость. Если размер нулевой, динамическая память для его элементов выделяться не должна.<br>
 - Конструктор из `std::initializer_list`. Элементы вектора должны содержать копию элементов `initializer_list`. Имеет размер и вместимость, совпадающую с размерами и вместимостью переданного `initializer_list`.<br>
 - Метод `GetSize` для получения количества элементов в векторе. Не выбрасывает исключений.<br>
 - Метод `GetCapacity` для получения вместимости вектора. Не выбрасывает исключений.<br>
 - Метод `IsEmpty`, сообщающий, пуст ли вектор. Не выбрасывает исключений.<br>
 - Оператор `[]` для доступа к элементу вектора по его индексу. Имеет две версии — константную и неконстантную. Не выбрасывает исключений. Для корректной работы оператора индекс элемента массива не должен выходить за пределы массива.<br>
 - Метод `At` для доступа к элементу вектора по его индексу, аналог метода at класса `vector`. В случае выхода индекса за пределы массива должен выбросить исключение `std::out_of_range`.<br>
 - Метод `Clear` для очистки массива без изменения его вместимости. Не выбрасывает исключений.<br>
 - Метод `Resize` для изменения количества элементов в массиве. Метод должен предоставлять строгую гарантию безопасности исключений.<br>
 - Методы `begin, end, cbegin и cend`, возвращающие итераторы на начало и конец массива. В качестве итераторов использованы указатели. Эти методы объявлены со спецификатором **noexcept**.<br>
 - При разрушении вектора освобождается память, занимаемая его элементами.<br>
 - Оператор присваивания. Должен обеспечивать строгую гарантию безопасности исключений.<br>
 - Метод `PushBack`, добавляющий элемент в конец вектора. Должен обеспечивать строгую гарантию безопасности исключений.<br>
 - Метод `PopBack`, удаляющий последний элемент вектора. Не выбрасывает исключений.<br>
 - Метод `Insert`, вставляющий элемент в произвольное место контейнера. Обеспечивает базовую гарантию безопасности исключений.<br>
 - Метод `Erase`, удаляющий элемент в произвольной позиции вектора. Обеспечивает базовую гарантию безопасности исключений.<br>
 - Метод `swap`, обменивающий содержимое вектора с другим вектором. Не выбрасывает исключений, имеет время выполнения **O(1)**.<br>
 - Операторы `== и !=`. Два вектора равны, если их размеры равны и содержат равные элементы. Равенство вместимости не требуется.<br>
 - Операторы `<, >, <=, >=`, выполняющие лексикографическое сравнение содержимого двух векторов.
 - `Reserve` задает ёмкость вектора. Этот метод повышает эффективность кода в случае, когда пользователь заранее знает хотя бы приблизительное количество элементов в векторе. `Reserve` сразу выделяет нужное количество памяти. При добавлении новых элементов в вектор копирование будет происходить или значительно реже или совсем не будет.<br>
Если `new_capacity` больше текущей `capacity`, память должна быть перевыделена, а элементы вектора скопированы в новый отрезок памяти.<br>
Кроме этого реализована возможность вызвать конструктор `SimpleVector`, который сразу резервирует нужное количество памяти.<br>