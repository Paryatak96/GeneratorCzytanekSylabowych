#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <Windows.h>
#include <wchar.h>



struct Sylaba
{
	char text[10];
	int dlugosc;
};

struct Slowo
{
	struct Sylaba sylaby[10];
	int iloscSylab;
};


void dodajLitere(struct Slowo* slowo, char znak);
int wypiszSylaby(struct Slowo* slowo, int kolor, FILE* plikZapis, HANDLE hConsole);
void CzytajCzytanke2(HANDLE hConsole, char* plik1, char* plik2);
void czyscSlowo(struct Slowo* slowo);
void poprawSylaby(struct Slowo* slowo);


int main(int argc, char* argv[])
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	if (argc < 4)
	{
		printf("Niepoprwana ilosc parametrow");
		return 1;
	}
	else
	{
		printf("%s %s\n", argv[2], argv[3]);
		CzytajCzytanke2(hConsole, argv[2], argv[3]);
	}
}


void CzytajCzytanke2(HANDLE hConsole, char* plik1, char* plik2)
{
	FILE* plik;
	plik = fopen(plik1, "r");
	if (plik == NULL)
	{
		printf("nie mozna odczytac z pliku");
	}
	FILE* plikZapis;
	plikZapis = fopen(plik2, "w");
	if (plikZapis == NULL)
	{
		printf("nie mozna zapisac do pliku");
	}
	char znak = 'x';
	struct Slowo slowo;
	czyscSlowo(&slowo);
	int kolor = 0;
	while (znak != EOF)
	{
		znak = getc(plik);
		if (znak != EOF)
		{
			if (znak == ' ' || znak == '\n')
			{
				poprawSylaby(&slowo);
				kolor = wypiszSylaby(&slowo, kolor, plikZapis, hConsole);
				czyscSlowo(&slowo);
				fprintf(plikZapis, "%c", znak);
				printf("%c", znak);
			}
			else
			{
				dodajLitere(&slowo, znak);
			}
		}
	}
	fclose(plik);
	fclose(plikZapis);
}

void czyscSlowo(struct Slowo* slowo)
{
	for (int i = 0; i < 10; i++)
	{
		slowo->sylaby[i].dlugosc = 0;
	}
	slowo->iloscSylab = 0;
}

void dodajLitere(struct Slowo* slowo, char znak)
{
	int dlugoscTekstuSylaby = slowo->sylaby[slowo->iloscSylab].dlugosc;
	slowo->sylaby[slowo->iloscSylab].text[dlugoscTekstuSylaby] = znak;
	slowo->sylaby[slowo->iloscSylab].dlugosc++;

	if (znak == 'a' || znak == 'e' || znak == 'o' || znak == 'u' || znak == 'y')
	{
		slowo->iloscSylab++;
	}
}

int zawieraI(struct Sylaba sylaba)
{
	for (int i = 0; i < sylaba.dlugosc; i++)
	{
		if (sylaba.text[i] == 'i')
		{
			return 1;
		}
	}
	return 0;
}

void poprawSylaby(struct Slowo* slowo)
{
	if (slowo->iloscSylab == 0 && slowo->sylaby[0].dlugosc > 0)
	{
		slowo->iloscSylab++;
	}
	else if (slowo->sylaby[slowo->iloscSylab].dlugosc > 0)
	{
		if (!zawieraI(slowo->sylaby[slowo->iloscSylab]))
		{
			int dl = slowo->sylaby[slowo->iloscSylab - 1].dlugosc;
			for (int i = 0; i < slowo->sylaby[slowo->iloscSylab].dlugosc; i++)
			{
				slowo->sylaby[slowo->iloscSylab - 1].text[dl + i] = slowo->sylaby[slowo->iloscSylab].text[i];
				slowo->sylaby[slowo->iloscSylab - 1].dlugosc++;
			}
			slowo->sylaby[slowo->iloscSylab].dlugosc = 0;
		}
		else
		{
			slowo->iloscSylab++;
		}
	}
}

void wypiszSylabeDopliku(struct Sylaba s, FILE* plikZapis, int kolor)
{

	fprintf(plikZapis, "<c%i>", (kolor % 2) + 1);
	for (int i = 0; i < s.dlugosc; i++)
	{
		fprintf(plikZapis, "%c", s.text[i]);
	}
	fprintf(plikZapis, "<c%i>", (kolor % 2) + 1);

}

int wypiszSylaby(struct Slowo* slowo, int kolor, FILE* plikZapis, HANDLE hConsole)
{
	for (int i = 0; i < slowo->iloscSylab; i++)
	{

		for (int j = 0; j < slowo->sylaby[i].dlugosc; j++)
		{
			if (kolor % 2 == 0)
			{
				SetConsoleTextAttribute(hConsole, 2);
				printf("%c", slowo->sylaby[i].text[j]);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, 4);
				printf("%c", slowo->sylaby[i].text[j]);
			}

		}
		wypiszSylabeDopliku(slowo->sylaby[i], plikZapis, kolor);
		kolor++;
	}
	return kolor;
}
