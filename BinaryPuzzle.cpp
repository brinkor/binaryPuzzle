#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "conio2.h"
#include "myHeader.h"

int main() {
	ustawienia_t podstawowe;
	ascii_t klawisze;
	plansza_t plansza;
	srand(time(NULL));
	// jeżeli program jest kompilowany w czystym języku C
	// należy wywołać poniższą linię
#ifndef __cplusplus
	Conio2_Init();
#endif
	// settitle ustawia tytuł okienka
	settitle("Michal Hajdasz 172156");
	inicjalizacjaPlanszy(&plansza);
	poczatkoweWypelnienie(&plansza);
	do 
	{
		wyswietlEkranPoczatkowy(&podstawowe,&plansza);
		wypiszPlansze(&plansza);
		stworzKursor(&podstawowe);
		czytajKlawisz(&podstawowe, &klawisze, &plansza);
	} 
	while (podstawowe.zn != klawisze.ESC);
	zwolnijPamiec(&plansza);
	return 0;
}

void inicjalizacjaPlanszy(plansza_t *plansza)
{
	//dynamiczna alokacja tablicy o podanym rozmiarze
	plansza->rozmiar;
	plansza->pole = (pole_t**)malloc(plansza->rozmiar * sizeof(pole_t*));
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		(plansza->pole)[i] = (pole_t*)malloc(plansza->rozmiar * sizeof(pole_t));
	}
}

void poczatkoweWypelnienie(plansza_t *plansza)
{
	//wczytuje plansze z gotowego pliku, jesli dostepny
	char nazwaPlikuPlanszy[20];
	char znak;
	FILE *plikPlanszy = NULL;
	sprintf(nazwaPlikuPlanszy, "plansza%dx%d.txt", plansza->rozmiar, plansza->rozmiar);
	plikPlanszy = fopen(nazwaPlikuPlanszy, "r");
	if (plikPlanszy != NULL)
	{
		for (int i = 0; i < plansza->rozmiar; i++)
		{
			for (int j = 0; j < plansza->rozmiar; j++)
			{
				znak = fgetc(plikPlanszy);
				if (znak == '\n' || znak == '\r')
				{
					j--;
					continue;
				}
				if (znak == PUSTE_POLE)
					plansza->pole[i][j].wartosc = PUSTE_POLE;
				else if (znak == JEDYNKA)
					plansza->pole[i][j].wartosc = JEDYNKA;
				else if (znak == ZERO)
					plansza->pole[i][j].wartosc = ZERO;
			}
		}
		fclose(plikPlanszy);
	}
	else
		wypelnijLosowo(plansza);
}

void wypiszPlansze(plansza_t *plansza)
{
	//wypisuje ramke
	int n;
	gotoxy(POZ_PLANSZY_X, POZ_PLANSZY_Y);
	textcolor(KOLOR_PLANSZY);
	for (int i = 0; i < plansza->rozmiar + 2; i++)
	{
		cputs("#");
	}
	int i;
	bool przerwij;
	//wypisanie tablicy na ekran w odpowiednich miejscach
	//zmiana koloru, jesli dane pole zostalo uzupelnione przez uzytkownika sa niemozliwe lub jednoznaczne
	for (i = 0; i < plansza->rozmiar; i++)
	{
		gotoxy(POZ_PLANSZY_X, POZ_PLANSZY_Y + i + 1);
		cputs("#");
		przerwij = false;
		for (n = 0; n < plansza->rozmiar; n++)
		{
			if (plansza->pole[i][n].wprowadzona == true)
			{
				przerwij = true;
				textcolor(KOLOR_WPROWADZONYCH);
			}
			else if (plansza->pole[i][n].niemozliwe == true)
			{
				przerwij = true;
				textbackground(KOLOR_NIEMOZLIWYCH);

			}
			else if (plansza->pole[i][n].jednoznaczne == JEDYNKA || plansza->pole[i][n].jednoznaczne == ZERO)
			{
				przerwij = true;
				textbackground(KOLOR_JEDNOZNACZNYCH);
			}
			putch(plansza->pole[i][n].wartosc);
			if (przerwij == true)
			{
				textcolor(KOLOR_PLANSZY);
				textbackground(KOLOR_TLA);
			}
		}
		cputs("#");
	}
	cputs("\n");
	gotoxy(POZ_PLANSZY_X, POZ_PLANSZY_Y + i + 1);
	for (i = 0; i < plansza->rozmiar + 2; i++)
	{
		cputs("#");
	}
}

void wyswietlEkranPoczatkowy(ustawienia_t *podst, plansza_t *plansza)
{
	// ustawiamy czarny kolor tła, z którego będziemy
	// korzystali rysując po ekranie
	// proszę zerknąć do conio2.h na listę dostępnych kolorów
	textbackground(BLACK);
	// czyścimy ekran: wypełniamy spacjami z ustawionym kolrem tła
	clrscr();
	// ustawiamy kolor tekstu na jasnoszary (7 == LIGHTGRAY)
	textcolor(LIGHTGRAY);
	// przesuwamy kursor do kolumny 55 i wiersza 1
	// wiersze i kolumny liczymy od 1
	int i = 1;
	wypiszLinieTekstu("Autor: Michal Hajdasz",&i);
	wypiszLinieTekstu("nr indexu: 172156", &i);
	wypiszLinieTekstu("Zaimplementowano:", &i);
	wypiszLinieTekstu("a, b, c, d, e, f, g, h, i, j, k, l, m", &i);
	//wypisanie pozycji kursora
	wypiszLinieTekstu("Pozycja kursora:", &i);
	char buffor[20];
	sprintf(buffor, " x = %2d y = %2d", podst->x, podst->y);
	cputs(buffor);
	wypiszLinieTekstu("Skroty klawiszowe:", &(++i));
	wypiszLinieTekstu("Esc = wyjscie", &i);
	wypiszLinieTekstu("strzalki = poruszanie", &i);
	wypiszLinieTekstu("0 / 1 = wpisanie wartosci na plansze", &i);
	wypiszLinieTekstu("spacja = zmiana koloru", &i);
	wypiszLinieTekstu("enter = zmiana koloru tla", &i);
	wypiszLinieTekstu("n = nowa gra z domyslna plansza", &i);
	wypiszLinieTekstu("o = losowe wypelnienie planszy", &i);
	wypiszLinieTekstu("p = prosta podpowiedz", &i);
	wypiszLinieTekstu("r -> liczba = zmiana rozmiaru planszy", &i);
	wypiszLinieTekstu("k = sprawdzenie mozliwosci ukonczenia gry", &i);
	wypiszLinieTekstu("d = sprawdzenie reguly drugiej", &i);
	wypiszLinieTekstu("a = wlacz tryb automatycznego konca gry", &i);
	wypiszLinieTekstu("j = zaznacza pola jednoznaczne", &i);
	wypiszLinieTekstu("ponowne wcisniecie j = wypelnia pola jednoznaczne", &i);
	// wypisujemy na ekranie kod ostatnio naciśniętego klawisza
	if (podst->special) sprintf(podst->txt, "kod klawisza: 0x00 0x%02x", podst->zn);
	else sprintf(podst->txt, "kod klawisza: 0x%02x", podst->zn);
	gotoxy(POZ_LEGENDY_X, i++);
	cputs(podst->txt);
	if (podst->sprKoniec == true)
	{
		textcolor(KOLOR_INFO);
		wypiszLinieTekstu("Automatyczny koniec gry jest aktywny.", &i);
		textcolor(KOLOR_OSTRZ);
		if (autoKoniec(plansza))
		{
			wypiszLinieTekstu("Koniec gry. Gry jest niemozliwa do ukonczenia", &(++i));
			i++;
		}
		textcolor(KOLOR_PLANSZY);
	}
	if (podst->podpowiedz.wyswietlic == true)
	{
		wyswietlPodpowiedz(podst, &i);
	}
	if (podst->regDruga == true)
		wypiszRegDrug(plansza, podst);
}

void wypiszLinieTekstu(char* tekst, int* licznik)
{
	// wyświetlamy na ekranie, w miejscu kursora napis
	// kursor przesuwa się w prawo o długość napisu
	gotoxy(POZ_LEGENDY_X, (*licznik)++);
	cputs(tekst);
}

void stworzKursor(ustawienia_t *podst)
{
	// rysujemy na ekranie kolorową gwiazdkę
	gotoxy(podst->x, podst->y);
	textcolor(podst->attr);
	textbackground(podst->back);
	// putch rysuje jeden znak i przesuwa kursor w prawo
	putch('*');
}

void czytajKlawisz(ustawienia_t *podst, ascii_t *klawisze, plansza_t *plansza)
{
	// czekamy na naciśnięcie klawisza i odczytujemy jego kod,
	// większość kodów klawiszy odpowiada ich znakom, np.
	// a to 'a', 2 to '2', + to '+', ale niektóre klawisze
	// specjalne, np. strzałki są zgłaszane jako dwa kolejne znaki,
	// z których pierwszy jest zerem, np. strzałka w górę
	// to zero i 'H'
	podst->special = false;
	podst->podpowiedz.wyswietlic = false;
	podst->zn = getch();
	// nie chcemy, aby klawisz 'H' działał jako strzałka w górę
	// więc sprawdzamy, czy pierwszym znakiem było zero
	if (podst->zn == 0)
	{
		podst->special = true;		// jeżeli tak, to czytamy
		podst->zn = getch();		// kolejny znak wiedząc już,
		if (podst->zn == klawisze->DOL) podst->y--;	// że będzie to znak specjalny
		else if (podst->zn == klawisze->GORA) podst->y++;
		else if (podst->zn == klawisze->LEWO) podst->x--;
		else if (podst->zn == klawisze->PRAWO) podst->x++;
	}
	else if (podst->zn == klawisze->SPACE) podst->attr = (podst->attr + 1) % L_KOLOROW;
	else if (podst->zn == klawisze->ENTER) podst->back = (podst->back + 1) % L_KOLOROW;
	else if (podst->zn == klawisze->JEDEN) zmienZnak(podst, plansza, JEDYNKA); //edycja wartości pola
	else if (podst->zn == klawisze->ZERO) zmienZnak(podst, plansza, ZERO);
	else if (podst->zn == klawisze->NOWA_GRA) nowaGra(plansza);
	else if (podst->zn == klawisze->LOSOWE_WYPELNIENIE) wypelnijLosowo(plansza);
	else if (podst->zn == klawisze->PROSTA_PODPOWIEDZ) prostaPodpowiedz(podst, plansza);
	else if (podst->zn == klawisze->ZMIEN_ROZMIAR) zmianaRozmiaru(plansza); 
	else if (podst->zn == klawisze->SPR_MOZLIWOSC_UKONCZENIA) sprMozliwoscUkonczenia(plansza);
	else if (podst->sprKoniec == false && podst->zn == klawisze->SPR_KONIEC) podst->sprKoniec = true;
	else if (podst->sprKoniec == true && podst->zn == klawisze->SPR_KONIEC) podst->sprKoniec = false;
	else if (podst->regDruga == false && podst->zn == klawisze->SPR_REG_DRUGIEJ) podst->regDruga = true;
	else if (podst->regDruga == true && podst->zn == klawisze->SPR_REG_DRUGIEJ) podst->regDruga = false;
	else if (podst->zn == klawisze->JEDNOZNACZNE) sprJednoznaczne(plansza, klawisze->JEDNOZNACZNE);
}

bool czyNaPlanszy(int x, int y, plansza_t *plansza)
{
	//porownuje x i y do po pocztku i konca tablicy
	if ((x < plansza->rozmiar && x >= 0) && (y < plansza->rozmiar && y >= 0))
		return true;
	else
		return false;
}

bool czyEdytowalny(int x, int y, plansza_t *plansza)
{
	//sprawdza czy edytowany znak nie jest znakiem poczatkowym, nietykalnym
	if ((plansza->pole[y][x].wartosc == PUSTE_POLE) || (plansza->pole[y][x].wprowadzona == true))
		return true;
	else
		return false;
}

bool regulaPierwsza(int x, int y, plansza_t *plansza, char wart)
{
	int licznik[LICZBA_KIERUNKOW] = {};
	//petla iteruje po czterech kierunkach planszy
	//sprawdza czy w polach obok znajduja sie takie same wartosci
	for (int kierunek = 0; kierunek < LICZBA_KIERUNKOW; kierunek++)
	{
		for (int i = 0; i < 2; i++)
		{
			if (kierunek == LEWO)
			{
				if (x - 1 - i >= 0)
				{
					if (plansza->pole[y][x - 1 - i].wartosc == wart)
						licznik[LEWO]++;
					else
						break;
				}
				else
					break;
			}
			if (kierunek == PRAWO)
			{
				if (x + 1 + i < plansza->rozmiar)
				{
					if (plansza->pole[y][x + 1 + i].wartosc == wart)
						licznik[PRAWO]++;
					else
						break;
				}
				else
					break;
			}
			if (kierunek == DOL)
			{
				if (y + 1 + i < plansza->rozmiar)
				{
					if (plansza->pole[y + 1 + i][x].wartosc == wart)
						licznik[DOL]++;
					else
						break;
				}
				else
					break;
			}
			if (kierunek == GORA)
			{
				if (y - 1 - i >= 0)
				{
					if (plansza->pole[y - 1 - i][x].wartosc == wart)
						licznik[GORA]++;
					else
						break;
				}
				else
					break;
			}
		}
	}
	// jesli w jednej lini juz sa dwie te same wartosci lub wiecej to zwraca falsz, w przeciwnym razie prawde
	if ((licznik[LEWO] + licznik[PRAWO] < 2) && (licznik[GORA] + licznik[DOL] < 2))
		return true;
	else
		return false;
}

bool regulaDruga(int x, int y, plansza_t *plansza, char wart)
{
	// sprawdz czy w kazdej kolumnie i kazdym wierszu jest taka sama koncowa ilosc wartosci JEDEN i ZERO
	char temp = plansza->pole[y][x].wartosc;
	plansza->pole[y][x].wartosc = wart;
	if ((liczWiersz(plansza, y, wart) > ((plansza->rozmiar) / 2)) || (liczKolumne(plansza, x, wart) > ((plansza->rozmiar) / 2)))
	{
		plansza->pole[y][x].wartosc = temp;
		return false;
	}
	else
	{
		plansza->pole[y][x].wartosc = temp;
		return true;
	}
}

int liczWiersz(plansza_t *plansza, int numer, char wart)
{
	// liczy ilosc wartosci wystepujacych w danym wierszu
	int ilosc = 0;
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		if (plansza->pole[numer][i].wartosc == wart)
			ilosc++;
	}
	return ilosc;
}

int liczKolumne(plansza_t *plansza, int numer, char wart)
{
	// liczy wystapienia danej wartosci w podenej kolumnie
	int ilosc = 0;
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		if (plansza->pole[i][numer].wartosc == wart)
			ilosc++;
	}
	return ilosc;
}

bool regulaTrzecia(int x, int y, plansza_t *plansza, char wart)
{
	// sprawdza czy po wpisanie wartosci w pole x,y nie powstana na planszy dwie takie same kolumny lub wiersze
	char temp = plansza->pole[y][x].wartosc;
	plansza->pole[y][x].wartosc = wart;
	if (porownajWiersze(plansza, y) || porownajKolumny(plansza, x))
	{
		plansza->pole[y][x].wartosc = temp;
		return false;
	}
	else
	{
		plansza->pole[y][x].wartosc = temp;
		return true;
	}
}

bool porownajWiersze(plansza_t *plansza, int numer)
{
	// porownuje do siebie odpowiednie wartosci z kazdego kolejnego wiersza roznego indeksem od podanego
	// zwraca true jesli sa takie wiersze
	int licznik;
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		licznik = 0;
		for (int k = 0; k < plansza->rozmiar; k++)
		{
			if (i == numer)
				break;
			else if ((plansza->pole[numer][k].wartosc != PUSTE_POLE) && (plansza->pole[numer][k].wartosc == plansza->pole[i][k].wartosc))
				licznik++;
		}
		if (licznik == plansza->rozmiar)
			return true;
	}
	return false;
}

bool porownajKolumny(plansza_t *plansza, int numer)
{
	//porownuje odpowadajace wartosci z kazdej kolumny rozniacej sie indeksem od podanej
	//zwraca true jesli sa takie kolumny
	int licznik;
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		licznik = 0;
		for (int k = 0; k < plansza->rozmiar; k++)
		{
			if (i == numer)
				break;
			else if ((plansza->pole[k][numer].wartosc != PUSTE_POLE) && (plansza->pole[k][numer].wartosc == plansza->pole[k][i].wartosc))
				licznik++;
		}
		if (licznik == plansza->rozmiar)
			return true;
	}
	return false;
}

void zmienZnak(ustawienia_t *podst, plansza_t *plansza, char wart)
{
	//edytuje wartosc przypisana do pola w tablicy planszy na podana
	//oznacza pole w tablicy jako wprowadzone
	int xPlanszy = podst->x - POZ_PLANSZY_X - 1;
	int yPlanszy = podst->y - POZ_PLANSZY_Y - 1;
	if (warunekWprowadzeniaWartosci(plansza,xPlanszy,yPlanszy,wart))
	{
		plansza->pole[yPlanszy][xPlanszy].wartosc = wart;
		plansza->pole[yPlanszy][xPlanszy].wprowadzona = true;
	}
}

bool warunekWprowadzeniaWartosci(plansza_t *plansza, int x, int y, char wart)
{
	//sprawdza czy w dane pole mozna wprowadzic dana wartosc
	if (czyNaPlanszy(x, y, plansza) && czyEdytowalny(x, y, plansza) &&
		regulaPierwsza(x, y, plansza, wart) && regulaDruga(x, y, plansza, wart) && regulaTrzecia(x, y, plansza, wart))
		return true;
	else
		return false;
}

void nowaGra(plansza_t *plansza)
{
	//rozpoczyna nowa gre od planszy domyslnej
	zwolnijPamiec(plansza);
	plansza->rozmiar = 12;
	inicjalizacjaPlanszy(plansza);
	poczatkoweWypelnienie(plansza);

	for (int i = 0; i < plansza->rozmiar; i++)
	{
		for (int j = 0; j < plansza->rozmiar; j++)
		{
			plansza->pole[i][j].wprowadzona = false;
		}
	}
}

int losujIloscPol(plansza_t *plansza)
{
	//losuje ilosc pol do wypelnienia z zakresu MIN-MAX procentu wypelnienia podanego w pliku naglowkowym
	int procent = (rand() % (MAX_PROCENT_WYP - MIN_PROCENT_WYP + 1) + MIN_PROCENT_WYP);
	int liczba = plansza->rozmiar*plansza->rozmiar*((double)procent/100);
	return liczba;
}

void czyscPlansze(plansza_t *plansza)
{
	// wypelnia plansze znakami kropki, usuwa zaznaczenie wprowadzonej wartosci przez uzytkownika
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		for (int j = 0; j < plansza->rozmiar; j++)
		{
			plansza->pole[i][j].wartosc = PUSTE_POLE;
			plansza->pole[i][j].wprowadzona = false;
		}
	}
}

void wypelnijLosowo(plansza_t *plansza)
{
	// czysci plansze do zera
	czyscPlansze(plansza);
	// pobiera max ilosc prob
	int MaxIloscProb = plansza->rozmiar*plansza->rozmiar*MNOZNIK_MAX_ILOSCI_PROB;
	int licznikWypelnionychPol = 0;
	// losuje liczbe pol do wypelnienia
	int liczbaPolDoWypelnienia = losujIloscPol(plansza);
	// petla wypelnia plasze zgodnymi, losowymi wartosciam az osiagnie max ilosc prob lub okreslona ilosc pol zostanie wypelniona
	for (int i = 0; i < MaxIloscProb; i++)
	{
		int x = losujIndex(plansza);
		int y = losujIndex(plansza);
		int wart = losujWartosc();
		if (warunekWprowadzeniaWartosci(plansza, x, y, wart))
		{
			plansza->pole[y][x].wartosc = wart;
			if (autoKoniec(plansza) == true)
				plansza->pole[y][x].wartosc = PUSTE_POLE;
			else
				licznikWypelnionychPol++;

		}
		if (licznikWypelnionychPol == liczbaPolDoWypelnienia)
			break;
	}
}

char losujWartosc()
{
	// losuje wartos: JEDYNKE lub ZERO
	bool zeroJeden = rand() % 2;
	if (zeroJeden == 1)
		return JEDYNKA;
	else
		return ZERO;
}

int losujIndex(plansza_t *plansza)
{
	//losuje index pola znajdujacego sie na planszy
	return rand() % plansza->rozmiar;
}

void prostaPodpowiedz(ustawienia_t *podst, plansza_t *plansza)
{
	//zaznacza, aby na ekranie pojawil sie napis z podpowiedzia
	//wywoluje funkcje sprawdzajace czy w pole mozemy wpisac 1 lub 0
	podst->podpowiedz.wyswietlic = true;
	podst->podpowiedz.liczbaMozliwosci = 0;
	int xPlanszy = podst->x - POZ_PLANSZY_X - 1;
	int yPlanszy = podst->y - POZ_PLANSZY_Y - 1;
	sprawdz(podst, plansza, xPlanszy, yPlanszy, JEDYNKA);
	sprawdz(podst, plansza, xPlanszy, yPlanszy, ZERO);

}

void sprawdz(ustawienia_t *podst, plansza_t *plansza, int xPlanszy, int yPlanszy, char wart)
{
	// sprawdza czy w pole mozemy wpisac zadana wartosc
	//liczy mozlwosci
	//zapisuje z jakiego powdu nie mozemy wpsiac danej wartosci
	if (warunekWprowadzeniaWartosci(plansza, xPlanszy, yPlanszy, wart))
	{
		podst->podpowiedz.mozliwosc = wart; 
		podst->podpowiedz.liczbaMozliwosci++;
	}
	else
	{
		if (czyNaPlanszy(xPlanszy, yPlanszy, plansza) == false)
		{
			podst->podpowiedz.powod = POZA_PLANSZA;
			return;
		}
		if (czyEdytowalny(xPlanszy, yPlanszy, plansza) == false)
		{
			podst->podpowiedz.powod = NIETYKALNE;
			return;
		}
		if (regulaPierwsza(xPlanszy, yPlanszy, plansza, wart) == false)
		{
			podst->podpowiedz.powod = REGULA_1;
		}
		if (regulaDruga(xPlanszy, yPlanszy, plansza, wart) == false)
		{
			podst->podpowiedz.powod = REGULA_2;
		}
		if (regulaTrzecia(xPlanszy, yPlanszy, plansza, wart) == false)
		{
			podst->podpowiedz.powod = REGULA_3;
		}
	}
}

void zwolnijPamiec(plansza_t *plansza)
{
	//zwalnia pamiec zarezerwowana na plansze
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		free((plansza->pole)[i]);
	}
	free(plansza->pole);
}

void wyswietlPodpowiedz(ustawienia_t *podst, int *i) //mozna dopracowac
{
	// wypisuej w odowiednim miejscu na ekranie prosta podpowiedz
	// korzysta z wczesniej zapisanych informacji w podpowiedzi
	gotoxy(POZ_LEGENDY_X, (*i)++);
	wypiszLinieTekstu("W to pole mozna wpisac ", i);
	switch (podst->podpowiedz.liczbaMozliwosci)
	{
	case 0:
		cputs("nic, poniewaz ");
		switch (podst->podpowiedz.powod)
		{
		case POZA_PLANSZA:
			wypiszLinieTekstu("Kursor jest poza plansza", i);
			break;
		case REGULA_1:
			wypiszLinieTekstu("Regula pierwsza nie zostanie spelniona", i);
			break;
		case REGULA_2:
			wypiszLinieTekstu("Regula druga nie zostanie spelniona", i);
			break;
		case REGULA_3:
			wypiszLinieTekstu("Regula trzecia nie zostanie spelniona", i);
			break;
		case NIETYKALNE:
			wypiszLinieTekstu("Pole nie jest edytowalne", i);
			break;
		}
		break;
	case 1:
		if (podst->podpowiedz.mozliwosc == JEDYNKA)
		{
			wypiszLinieTekstu("tylko jedynke, ", i);
			cputs("poniewaz jesli wpiszemy zero ");
		}
		else
		{
			wypiszLinieTekstu("tylko zero, ", i);
			cputs("poniewaz jesli wpiszemy jedynke ");
		}
		switch (podst->podpowiedz.powod)
		{
		case REGULA_1:
			wypiszLinieTekstu("otzymamy wiecej niz 2 wartosci rowne ", i);
			if (podst->podpowiedz.mozliwosc == JEDYNKA)
			{
				wypiszLinieTekstu("zero obok siebie", i);
			}
			else
				wypiszLinieTekstu("jeden obok siebie", i);
			break;
		case REGULA_2:
			wypiszLinieTekstu("otrzymamy niezgodna liczbe elementow w lini", i);
			break;
		case REGULA_3:
			wypiszLinieTekstu("dwa takie same ciag wiartosci", i);
			break;
		}
		break;
	case 2:
		wypiszLinieTekstu("jeden lub zero", i);
		break;
	}
}

void zmianaRozmiaru(plansza_t *plansza)
{
	// tworzy nowa plansze o nowym rozmiarze wprowadzonym przez uzytkownika
	char rozmiar[3];
	rozmiar[0] = getch();
	rozmiar[1] = getch();
	rozmiar[2] = '\0';
	int rozm = atoi(rozmiar);
	if (rozm % 2 == 0)
	{
		zwolnijPamiec(plansza);
		plansza->rozmiar = atoi(rozmiar);
		inicjalizacjaPlanszy(plansza);
		poczatkoweWypelnienie(plansza);
	}
}

void sprMozliwoscUkonczenia(plansza_t *plansza)
{
	//sprawdza czy na planszy sa pola niemozliwe do uzupelnienia
	//wypisje plansze z zaznaczeniami takich pol jesli sa
	//czeka na uzytkownika
	//czysci zaznaczenia pol
	nadajNiemozliwe(plansza);
	wypiszPlansze(plansza);
	getch();
	wyczyscNiemozliwe(plansza);
}

void nadajNiemozliwe(plansza_t *plansza)
{
	//sprawdza czy na planszy sa pola niemozliwe do uzupelnienia
	//nadaje tym polom wartosc true w zmiennej niemozliwe
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		for (int j = 0; j < plansza->rozmiar; j++)
		{
			if (plansza->pole[i][j].wartosc == PUSTE_POLE)
			{
				if ((warunekWprowadzeniaWartosci(plansza, j, i, JEDYNKA) == false) && (warunekWprowadzeniaWartosci(plansza, j, i, ZERO) == false))
					plansza->pole[i][j].niemozliwe = true;
			}
		}
	}
}

void wyczyscNiemozliwe(plansza_t *plansza)
{
	//czysci zaznaczenie pol niemozliwych
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		for (int j = 0; j < plansza->rozmiar; j++)
		{
			if (plansza->pole[i][j].niemozliwe == true)
				plansza->pole[i][j].niemozliwe = false;
		}
	}
}

void wypiszRegDrug(plansza_t *plansza, ustawienia_t *podst)
{
	// obok planszy wypisuje liczbe zer i jedynek w kolumnach i wierszach
	char liczba[4];
	char tekst[] = "Ilosc jedynek w kolumnach:";
	gotoxy(POZ_PLANSZY_X - (sizeof(tekst) / sizeof(tekst[0])), POZ_PLANSZY_Y - 3);
	cputs(tekst);
	gotoxy(POZ_PLANSZY_X - (sizeof(tekst) / sizeof(tekst[0])), POZ_PLANSZY_Y - 1);
	cputs("Ilosc zer w kolumnach:");
	gotoxy(POZ_PLANSZY_X + plansza->rozmiar + 3, POZ_PLANSZY_Y + plansza->rozmiar + 2);
	cputs("1/0");
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		sprintf(liczba, "%i", liczKolumne(plansza, i, JEDYNKA));
		gotoxy(POZ_PLANSZY_X + i + 1, POZ_PLANSZY_Y - 3);
		cputs(liczba);
		gotoxy(POZ_PLANSZY_X + i + 1, POZ_PLANSZY_Y - 2);
		cputs("-");
		sprintf(liczba, "%i", liczKolumne(plansza, i, ZERO));
		gotoxy(POZ_PLANSZY_X + i + 1, POZ_PLANSZY_Y - 1);
		cputs(liczba);
		sprintf(liczba, "%i-%i", liczWiersz(plansza, i, JEDYNKA), liczWiersz(plansza, i, ZERO));
		gotoxy(POZ_PLANSZY_X + plansza->rozmiar + 3, POZ_PLANSZY_Y + i + 1);
		cputs(liczba);
	}
	gotoxy(podst->x, podst->y);
}

bool autoKoniec(plansza_t *plansza)
{
	//sprawdza czy sa na polaszy pola, ktorych nie mozna uzupelnic zadna wartoscia
	//jesli tak zwraca true
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		for (int j = 0; j < plansza->rozmiar; j++)
		{
			if (plansza->pole[i][j].wartosc == PUSTE_POLE)
			{
				if ((warunekWprowadzeniaWartosci(plansza, j, i, JEDYNKA) == false) && (warunekWprowadzeniaWartosci(plansza, j, i, ZERO) == false))
					return true;
			}
		}
	}
	return false;
}

void sprJednoznaczne(plansza_t *plansza, char klawisz)
{
	// sprawdza czy na planszy sa pola mozliwe do wypelnienia tylko konkretna wartoscia
	//jesli tak ponowne wcisniecie klawisza powoduje uzupelnienie tych pol odpowiednia wartoscia
	for (int i = 0; i < plansza->rozmiar; i++)
	{
		for (int j = 0; j < plansza->rozmiar; j++)
		{
			if (plansza->pole[i][j].wartosc == PUSTE_POLE)
			{
				if ((warunekWprowadzeniaWartosci(plansza, j, i, JEDYNKA) == true) && (warunekWprowadzeniaWartosci(plansza, j, i, ZERO) == false))
					plansza->pole[i][j].jednoznaczne = JEDYNKA;
				if ((warunekWprowadzeniaWartosci(plansza, j, i, JEDYNKA) == false) && (warunekWprowadzeniaWartosci(plansza, j, i, ZERO) == true))
					plansza->pole[i][j].jednoznaczne = ZERO;
			}
		}
	}
	wypiszPlansze(plansza);
	if (getch() == klawisz)
	{
		for (int i = 0; i < plansza->rozmiar; i++)
		{
			for (int j = 0; j < plansza->rozmiar; j++)
			{
				if (plansza->pole[i][j].jednoznaczne == JEDYNKA)
				{
					plansza->pole[i][j].wartosc = JEDYNKA; 
					plansza->pole[i][j].jednoznaczne = NIC;
				}
				else if (plansza->pole[i][j].jednoznaczne == ZERO)
				{
					plansza->pole[i][j].wartosc = ZERO;
					plansza->pole[i][j].jednoznaczne = NIC;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < plansza->rozmiar; i++)
		{
			for (int j = 0; j < plansza->rozmiar; j++)
			{
				if (plansza->pole[i][j].jednoznaczne != NIC)
					plansza->pole[i][j].jednoznaczne = NIC;
			}
		}
	}
}