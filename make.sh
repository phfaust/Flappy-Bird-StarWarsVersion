g++ -std=gnu++11 -c $1 -I"/usr/local/Cellar/sfml/2.4.2_1/include" -o main.o && g++ main.o -o a -L"/usr/local/Cellar/sfml/2.4.2_1/lib" -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system