#include <iostream>
#include <stdio.h>
#include <math.h>

#pragma warning(disable:4996)

double	AddTaxe(double Val)
{
	return (Val * 1.196);
}

double Diff(double Val1, double Val2)
{
	return ((Val1 + Val2) / 1) - sqrt(Val1 * Val2);
}

void RendreLaMonnaie(int Val)
{
	int Base[] = { 500, 200, 100, 50, 20, 10, 5, 2, 1 }; //{ 500, 200, 20, 5, 2 };
	int TotalBase = sizeof(Base) / sizeof(int);
	int Reste = Val;
	printf("%d = ", Val);
	for (int i = 0; i != TotalBase; i++)
	{
		int CurrentValue = Reste / Base[i];
		if (CurrentValue > 0)
		{
			printf("%d x %d", CurrentValue, Base[i]);
			if (i + 1 < TotalBase - 1)
				printf(" + ");
		}
		Reste = Reste % Base[i];
	}
	printf("\n");
	return;
}

void Arrondi(float Val)
{
	printf("%f\n", floor(Val));
	printf("%f\n", ceil(Val));
	printf("%f\n", round(Val));
}

void Equation(int a, int b, int c)
{
	int discriminant = (b * b) - (4 * a * c);
	printf("Discriminant = %d\n", discriminant);
	if (discriminant < 0)
		printf("Pas de solution\n");
	else if (discriminant == 0)
	{
		int x = -(b / (2 * a));
		printf("%d = %d / ( 2 x %d)", x, -b, a);
	}
	else if (discriminant > 0)
	{
		int x1 = ((-b - sqrt(discriminant)) / (2 * a));
		int x2 = ((-b + sqrt(discriminant)) / (2 * a));
		printf("x1 = %d x2 = %d", x1, x2);
	}
}

void NextDay(int jour, int mois, int annee)
{
	int Execption[] = {4, 6, 9, 11 };
	jour++;
	for (int i = 0; i != sizeof(Execption) / sizeof(int); i++)
	{
		if (mois == Execption[i])
			jour++;
	}
	if (mois == 2)
		jour = jour + 3;
	if (jour > 31)
	{
		jour = 1;
		mois++;
		if (mois > 12)
		{
			mois = 1;
			annee++;
		}
	}
	printf("%d/%d/%d\n", jour, mois, annee);
}

int main()
{
	char Input1[100];
	char Input2[100];
	char Input3[100];
	//printf("%f\n", AddTaxe(100));
	//gets_s(Input);
	//printf("%f\n", Diff(100, atof(Input)));
	//RendreLaMonnaie(1949);
	//Arrondi(atof(Input));
	//Equation(2, -3, -6);
	NextDay(29, 2, 1996);
}
