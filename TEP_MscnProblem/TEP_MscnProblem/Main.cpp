// Lista9.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include "CMscnProblem.h"
#include "ProgramModule.h"
#include "CRandom.h"


int main()
{
	CMscnProblem c_problem;

	vShowMenu(c_problem);
}


//Uwagi do rozwiązania
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

example_problem.txt
example_solution.txt
ex_invalid_sol.txt

powyższe pliki są kompatybilne ze sobą
*/


