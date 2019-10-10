#include "pch.h"
#include <iostream>
#include <stdio.h>

static void TestRect();

void Memcpy(char* dest, char* src, int size)
{
	int i;
	for (i = 0; i != size; i++)
		dest[i] = src[i];
}

void MemcpyPtr(char* dest, char* src, int size)
{
	int i = 0;
	while (size--)
		dest[size] = src[size];
}

int StrChr(char* grange, char chat)
{
	int i = 0;
	while (grange[i] != 0)
	{
		if (grange[i] == chat)
			return i;
		i++;
	}
	return (-1);
}

int StrChr2(char* grange, char chat)
{
	int i;
	for (i = 0; grange[i] != 0; i++)
	{
		if (grange[i] == chat)
			return i;
	}
	return (-1);
}

int StrChr3(char* grange, char chat)
{
	char* PosGrange0 = grange;
	while (*grange)
	{
		if (*grange == chat)
			return (int(grange - PosGrange0));
		grange++;
	}
	return -1;
}

int Max(int a, int b)
{
	return (a < b ? a : b);
}

int Min(int a, int b)
{
	return (a < b ? b : a);
}

int StrCmp(char* Str1, char* Str2)
{
	int Str1_Lenght = strlen(Str1);
	int Str2_Lenght = strlen(Str2);

	int MaxLen = Max(Str1_Lenght, Str2_Lenght);
	int MinLen = Min(Str1_Lenght, Str2_Lenght);

	for (int i = 0; i < MinLen; i++)
	{
		if (Str1[i] < Str2[i])
			return -1;
		else if (Str1[i] > Str2[i])
			return (1);
	}
	if (MinLen == MaxLen) return 0;
	if (MaxLen == Str1_Lenght) return -1;
	return 1;
}

char* StrStr(char* Src, char* Token)
{
	for (int i_1 = 0; Src[i_1] != 0; i_1++)
	{
		bool found = true;
		for (int i_2 = 0; Token[i_2] != 0; i_2++)
		{
			if (Src[i_2] != Token[i_2])
			{
				found = false;
				break;
			}
		}
		if (found)
			return (Src);
		Src++;
	}
	return NULL;
}

int addRec(int a, int b)
{
	if (a == 0)
		return b;
	else
		addRec(a - 1, b + 1);
}

int addRec2(int a, int b)
{
	if (a == 0) return b;
	else if (b == 0) return a;
	else
	{
		int NB = 1 + addRec2(a - 1, b);
		printf("A : %d + B : %d = %d\n", a, b, NB);
		return NB;
	}
}

int Sub(int a, int b)
{
	if (b == 0)
		return a;
	else
		Sub(a - 1, b - 1);
}

int Sub_2(int a, int b)
{
	if (b == 0)
		return (a);
	else
	{
		if (b > 0)
			return Sub_2(a, b - 1) - 1;
		else if (b < 0)
			return  Sub_2(a, b + 1) + 1;
	}
}

int Multi(int a, int b)
{
	if (a == 0 || b == 0)return 0;
	else if (b == 1)return a;
	else
	{
		if (b > 0)
			return Multi(a, b - 1) + a;
		else
			return -Multi(a, -b);
	}
}

int Div(int a, int b)
{
	if (a == 0 || b == 0)return 0;
	else
	{
		if (a < b) return 0;
		else if (b > 0)
			return 1 + Div(a - b, b);
		else if (a < 0)
			return -Div(-a, b);
		else
			return -Div(a, -b);
	}
}

int Mod(int a, int b)
{
	if (b == 0)return a;
	else
		return a - Multi(Div(a, b), b);
}

int StrlenRec(const char* Str)
{
	if (*Str == 0) return 0;
	return 1 + StrlenRec(Str + 1);
}

void StrCpyRec(char* Dest, const char* Src)
{
	if (*Src == 0)
	{
		*Dest = 0;
		return;
	}
	*Dest = *Src;
	return StrCpyRec(Dest + 1, Src + 1);
}

void ZeroMemory(char* const Dest, int Size)
{
	if (Size == 0) return;
	*Dest = 0;
	ZeroMemory(Dest, Size - 1);

}

void MemCpyRec(char* Dest, const char* Src, int Size)
{

	if (Size - 1 < 0)
	{
		*Dest = 0;
		return;
	}
	*Dest = *Src;
	return MemCpyRec(Dest + 1, Src + 1, Size - 1);
}

int StrCmpRec(char* Str1, char* Str2)
{
	if (*Str1 == 0 && *Str2 == 0) return 0;
	if (*Str1 == 0) return 1;
	if (*Str2 == 0) return -1;
	if (*Str1 > * Str2) return 1;
	if (*Str1 < *Str2) return -1;
	return StrCmpRec(Str1 + 1, Str2 + 1);
}

void StrCatRec(char* Dest, char* Src)
{
	if (*Src == 0)
		return;
	if (*Dest != 0)
		return StrCatRec(Dest + 1, Src);
	*Dest = *Src;
	return StrCatRec(Dest + 1, Src + 1);
}

char* StrChrRec(char* str, char tok)
{
	if (tok == 0 || *str == 0)
		return nullptr;
	else if (*str == tok)
		return str;
	else
		return (StrChrRec(str + 1, tok));
}

char* StrStrRec(char* str0, char* str1)
{
	if (*str0 == 0 || str1 == 0)
		return nullptr;
	else if (*str0 == *str1)
	{
		char* temp = str0;
		if (str0 != str1)
			return temp;
		return StrStrRec(str0 + 1, str1 + 1);
	}
	return StrStrRec(str0 + 1, str1);
}

int main()
{
	/*std::cout << "Hello World!\n";
	char licorne[32] = "licorne";
	char vomi[32] = "vomit";
	printf("Avant : %s\n", licorne);
	MemcpyPtr(licorne, vomi, strlen(licorne) + 1);
	printf("licorne %s\n", licorne);

	char Chateau[32] = "Chateaux";
	char S = 'e';
	printf("Position %d\n", StrChr3(Chateau, S));*/

	/*char text[1024] = "Lorem ipsum dolor sit amer";
	char Token[1024] = "dolor";

	char * TokenInText = StrStr(text, Token);
	printf("%s\n", TokenInText);
	int pos = (int)(TokenInText - text);
	printf("Le token est en position %d\n", pos);*/

	char Dest[1024] = "Le test marche ?";
	char Text[1024] = " Oui il marche !";
	
	char Token = 'm';
	char Find[1024] = "Oui";

	//int SizeBuff = 32;
	//char *Buffer = (char *)malloc(sizeof(char));
	//ZeroMemory(Dest, SizeBuff);

	////StrCpyRec(Dest, Text);
	//MemCpyRec(Dest, Text, strlen(Text));
	//printf("%d\n", StrlenRec(Text));
	//printf("%s\n", Dest);
	

	printf("[StrChrRec] Source = %s | Token = %c | Resultat =  %s\n", Dest, Token, StrChrRec(Dest, Token));

	printf("[StrCatRec] Str1 = %s | Str2 = %s |", Dest, Text);
	StrCatRec(Dest, Text);
	printf(" Resultat = %s\n", Dest);

	printf("[StrStrRec] Source = %s | String to find = %s | Resultat = %s \n", Dest, Find,  StrStrRec(Dest, Find));
}

