#Оголошення змінної для компілятора C++
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++11

#Головна ціль для побудови проекту
all: app

#Створення статичної бібліотеки libcalculator.a
libcalculator.a: calculator.o
	ar rcs libcalculator.a calculator.o

#Компіляція об'єктного файлу бібліотеки
calculator.o: calculator.cpp calculator.h
	$(CXX) $(CXXFLAGS) -c calculator.cpp -o calculator.o

#Компіляція об'єктного файлу головної програми
main.o: main.cpp calculator.h
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

#Лінкування виконуваного файлу зі статичною бібліотекою
app: main.o libcalculator.a
	$(CXX) $(CXXFLAGS) main.o -L. -lcalculator -o app

#Секція очищення скомпільованих файлів
clean: rm -f *.o *.a app
