// Main.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include "CMscnProblem.h"
#include "ProgramModule.h"


int main()
{
	CMscnProblem c_problem;
	vShowMenu(c_problem);
}

//Odpowiedzi do listy 11

//Zastanów się, który z obiektów powinien zliczać liczbę wywołań oceny przystosowania
//i umieść kod w odpowiednim miejscu.

/*Moim zdaniem najsensowniejszym rozwiązaniem jest pozostawienie zliczania liczby oceny przystosowania
obiektowi optymalizatora a dokładnie jego głownej metodzie optymalizującej. Licznik wystarczy zrobić w postaci
zwykłego integera i inkrementować go po każdym wywołaniu odpowiedniej funkcji

Trzymanie licznika w innym miejscu jest niepotrzebne (szczególnie jako pole obiektu), ponieważ jest to wartość tymczasowa
i kompletnie niepotrzebna po wykonaniu głównej metody
*/


//Odpowiedzi do listy 10

//Ustal relację, którą
//zachodzi pomiędzy klasami CRandomSearch i CMscnProblem.Czy jest to agregacja silna,
//słaba, dziedziczenie, a może jakaś inna relacja ?

/*W moim rozwiązaniu mamy do czynienia z agregacją słabą - klasa CMscnProblem jest
składową CRandomSearch ale nie jest z nią ściśle związana, czyli czas życia części i
całości są niezależne.

CRandomSearch przechowuje wskaznik do danego obiektu klasy CMscnProblem i używa go tylko jako
wymienne narzędzie do wykonywania obliczeń. Agregacja silna byłaby nie na miejscu, ponieważ obiekt klasy
CMscnProblem, może być składową wielu obiektów i usunięcie go przez jeden z tych obiektów powodowałoby błędy działania programu*/



//Uwagi do rozwiązania - Lista 9

/*Do reprezentacji tablic i macierzy wykorzystałem strukturę danych vector z 
biblioteki standardowej (tylko funkcjonalości z C++98) zamiast tablic dynamicznych 
ze względów praktycznych (metod: np. size(), brak potrzeby definiowania specjalnych destruktorów itp.)

! Program wciąż spełnia wszystkie wymagane funkcjonalości !*/

//Krótka instrukcja do programu
/*Do rozwiązania dodałem prosty interfejs pozwalajający wykonać poniższe akcje:
1 - tworzy pusty plik problemu dla podanej ilości fabryk, dostawców, magazynów i sklepów
2 - tworzy pusty plik rozwiazania dla problemu o danej ilości fabryk, dowstawców, itd.
3 - wczytuje ustawienia dla instancji problemu w programie, potrzebne do obliczenia dGetQuality w opcji 4
4 - wczytuje plik rozwiązania (powinien być kompatybilny z problemem wczytanym opcją nr 3) i
oblicza jego jakość

Zeby stworzyć własne rozwiązania należy stworzyć nowe pliki opcjami 1 i 2 oraz wypełnić ręcznie
powstałe pliki (klasa ma wymagane metody pozwalające to stworzyć z poziomu kodu ale jest
to mało praktycznie przy większych instancjach)


PRZYKŁADOWE PLIKI W ROZWIĄZANIU

ex_problem.txt
ex_solution.txt

powyższe pliki są kompatybilne ze sobą
*/


