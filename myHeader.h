#ifndef MYHEADER
#define MYHEADER

static const int POZ_LEGENDY_X = 1;
static const int POZ_PLANSZY_X = 80;
static const int POZ_PLANSZY_Y = 4; // nie mniej niz 4
static const int L_KOLOROW = 16;
static const int KOLOR_PLANSZY = WHITE;
static const int KOLOR_TLA = BLACK;
static const int KOLOR_WPROWADZONYCH = CYAN;
static const int KOLOR_NIEMOZLIWYCH = RED;
static const int KOLOR_JEDNOZNACZNYCH = GREEN;
static const int KOLOR_INFO = YELLOW;
static const int KOLOR_OSTRZ = RED;
static const int JEDYNKA = '1';
static const int ZERO = '0';
static const int PUSTE_POLE = '.';
static const int NIC = '\0';
static const int LICZBA_KIERUNKOW = 4;
static const int MIN_PROCENT_WYP = 15;
static const int MAX_PROCENT_WYP = 35;
static const double MNOZNIK_MAX_ILOSCI_PROB = 1;

typedef enum
{
	PRAWO, LEWO, GORA, DOL
}kierunki_t;

typedef enum
{
	POZA_PLANSZA,
	REGULA_1,
	REGULA_2,
	REGULA_3,
	NIETYKALNE
}powody_t;

typedef struct
{
	const int GORA = 0x50;
	const int DOL = 0x48;
	const int LEWO = 0x4b;
	const int PRAWO = 0x4d;
	const int ENTER = 0x0d;
	const int SPACE = ' ';
	const int ESC = 27;
	const int JEDEN = '1';
	const int ZERO = '0';
	const int NOWA_GRA = 'n';
	const int LOSOWE_WYPELNIENIE = 'o';
	const int PROSTA_PODPOWIEDZ = 'p';
	const int ZMIEN_ROZMIAR = 'r';
	const int SPR_MOZLIWOSC_UKONCZENIA = 'k';
	const int SPR_REG_DRUGIEJ = 'd';
	const int SPR_KONIEC = 'a';
	const int JEDNOZNACZNE = 'j';
} ascii_t;

typedef struct
{
	int zn = 0;
	int x = POZ_PLANSZY_X + 1;
	int y = POZ_PLANSZY_Y + 1;
	int attr = LIGHTGRAY;
	int back = BLACK;
	bool special = false; //sygnalizacja klawisza specjalnego
	bool regDruga = false;
	bool sprKoniec = false;
	char txt[32];
	struct
	{
		bool wyswietlic = false;
		int liczbaMozliwosci = 0;
		char mozliwosc;
		powody_t powod;
	}podpowiedz;
} ustawienia_t;

typedef struct
{
	char jednoznaczne = NIC;
	bool niemozliwe = false; //czy podswietlona kolorem niemozliwych
	bool wprowadzona = false; // czy zostala wprwadzona przez uzytkownika (mozna edytowac)
	char wartosc;
} pole_t;

typedef struct
{
	int rozmiar = 12;
	pole_t **pole;
} plansza_t;

void wyswietlEkranPoczatkowy(ustawienia_t *podst, plansza_t *plansza);
void poczatkoweWypelnienie(plansza_t *plansza);
void wypiszPlansze(plansza_t *plansza);
void stworzKursor(ustawienia_t *podst);
void czytajKlawisz(ustawienia_t *podst, ascii_t *klawisze, plansza_t *plansza);
void inicjalizacjaPlanszy(plansza_t *plansza);
void zwolnijPamiec(plansza_t *plansza);
void wypiszLinieTekstu(char* tekst, int* licznik);
void zmienZnak(ustawienia_t *podst, plansza_t *plansza, char wartosc);
bool czyNaPlanszy(int x, int y, plansza_t *plansza); // zwraca true jesli punkt znajduje sie na planszy, inaczej false
bool czyEdytowalny(int x, int y, plansza_t *plansza); // jesli punkt mozna edytowac zwraca true, inaczej false
bool regulaPierwsza(int x, int y, plansza_t *plansza, char wart); //jesli spelniowa to true, inaczej false
void nowaGra(plansza_t *plansza);
bool regulaDruga(int x, int y, plansza_t *plansza, char wart); //jesli spelniona zwraca true w przeciwnym wypadku false
int liczWiersz(plansza_t *plansza, int numer, char wart);
int liczKolumne(plansza_t *plansza, int numer, char wart);
bool regulaTrzecia(int x, int y, plansza_t *plansza, char wart); //jesli spelniona zwraca true w przeciwnym wypadku false
bool porownajWiersze(plansza_t *plansza, int numer); // zwraca true jesli sa takie same, false jesli sa rozne
bool porownajKolumny(plansza_t *plansza, int numer); // zwraca true jesli sa takie same, false jesli sa rozne
bool warunekWprowadzeniaWartosci(plansza_t *plansza, int x, int y, char wart); // true jesli spelniony, false jesli nie
int losujIloscPol(plansza_t *plansza); // zwraca wylosowana ilosc pol
void czyscPlansze(plansza_t *plansza);
void wypelnijLosowo(plansza_t *plansza);
char losujWartosc();
int losujIndex(plansza_t *plansza); //zwraza numer indexu
void prostaPodpowiedz(ustawienia_t *podst, plansza_t *plansza);
void sprawdz(ustawienia_t *podst, plansza_t *plansza, int xPlanszy, int yPlanszy, char wart);
void wyswietlPodpowiedz(ustawienia_t *podst, int *i);
void zmianaRozmiaru(plansza_t *plansza);
void sprMozliwoscUkonczenia(plansza_t *plansza);
void nadajNiemozliwe(plansza_t *plansza);
void wyczyscNiemozliwe(plansza_t *plansza);
void wypiszRegDrug(plansza_t *plansza, ustawienia_t *podst);
bool autoKoniec(plansza_t *plansza); // zwraca true jesli gry nie da sie skonczyc, false jesli sie da
void sprJednoznaczne(plansza_t *plansza, char klawisz);

#endif
