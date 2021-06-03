# Complex Systems Final Project
kompilacja:
g++ -O3 main.cpp -o main

uruchamianie:\
umieścić w tym samym folderze plik main.cpp oraz params_file.txt\
najpierw uzupełnić plik "params_file.txt", przykładowo:

1 10000 500000 3 0.4 0.3 0.0 1 0.39 0.01 1.0 0.1 0.0
M n       g    s IA  pA  IB selfInf  pA_End pA_Step IA_End IA_Step conv

parametr "selfInf" oznacza, czy bierzemy pod uwagę opinię własną do obliczania wypadkowej\
należy dać wartość 1 lub 0, które oznaczają tak lub nie.\
parametry "pA_End pA_Step IA_End IA_Step" używane są do generowania heatmapy, gdzie *_End to koniec zakresu, a *_Step to krok -\
dzięki temu wygeneruje się heatmapa o poprawnym rozmiarze, dokładnie tak jak w artykule.\
parametr "conv" - odsetek agentów o zdwojonej sile przekonywania, interakcja z takim agentem powoduje osłabienie opinii\
lub jej umocnienie o dwie jednostki, zamiast o jedną, tak jak byłoby z "normalnym" agentem.\
Ustawienie "conv" na 0.0 przywraca starą symulację ( tj. wszystkie agenty mają wartość 1, czyli po staremu ;-) )

potem wpisujemy do konsoli:\
./main.exe kwadraty\
lub\
./main.exe histogram

Wyniki działania programu będą wyświetlone w plikach końcowych generowanych automatycznie.

wyniki działania symulacji "histogram":
- plik o nazwie z parametrami
- wartości końcowe dla wszystkich agentów

wyniki działania symulacji "kwadraty":
- plik o nazwie z parametrami
- tablica z danymi do heatmapy
