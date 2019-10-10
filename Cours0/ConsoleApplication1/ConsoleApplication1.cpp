// Cour1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
//#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <chrono>

//Vec3 IncrX(Vec3 _in)
//{
//	_in.x++;
//	return _in;
//}
//
//Vec3 StackOverflow(Vec3 _in)
//{
//	Vec3 temp = _in;
//	temp.y++;
//	return StackOverflow(temp);
//}



int Strlenght(const char * src)
{
	int count = 0;
	for (int i = 0; src[i] != 0; i++)
		count++;
	return count;
}

int Strcpy(char * dest, const char * src)
{
	int i = 0;
	for (i = 0; i != Strlenght(src); i++)
		dest[i] = src[i];
	dest[i + 1] = 0;
	return (0);
}

int main()
{
	//std::cout << "Hello World!\n";

	/*int foo = 0;
	float f = 0.5f;
	double d = 0.5f;
	const char * label = "Sapin";
	printf("Hello world this is a test : int %d label : %s\n float : %f double : %D\n", foo, label, f, d);
	Vec3 toto = { 1, 2, 3 };
	IncrX(toto);
	printf("%f\n", toto.x);
	StackOverflow(toto);*/

	/*const char label[6] = { 's', 'a', 'p', 'i', 'n', 0 };
	const char * ptr = &label[0];
	*ptr++;
	printf("%c\n", *ptr);

	Vec3 vectab[3];
	vectab[0] = { 1, 2, 3 };
	vectab[1] = { 1, 2, 3 };
	vectab[2] = { 1, 2, 3 };

	printf("V0.x %f\n", vectab[0].x);
	Vec3 * t0 = 0;
	Vec3 * t1 = nullptr;
	Vec3 * t2 = &vectab[1];

	Vec3 * iter = &vectab[0];

	for (int i = 0; i < 3; i++)
	{
		printf("Vectab[%d] X : %d Y : %d Z : %d \n", i, iter->x, iter->y, iter->z);
		iter++;
	}


	(*t2).y = 777;
	t2->y = 888;
	Vec3 * t3 = t2 + 1;*/



	/*int * BigBlock = (int *) malloc(1024 * 1024 * 1024);
	auto start = std::chrono::system_clock::now();
	for (int i = 0; i < 64 * 1024 * 1024; i++)
		BigBlock[i] = 0xdeadbeef;
	printf("beef : ? %x\n", BigBlock[1024 * 1024]);
	auto end = std::chrono::system_clock::now();
	auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	printf("%d Ms\n", millis);*/

	/*Vec3 tableau[16];
	Vec3 *tableau0 = (Vec3*)malloc(sizeof(Vec3) * 16); // C
	Vec3 *tableau1 = (Vec3*)calloc(sizeof(Vec3), 16); // C

	Vec3 *tableau2 = new Vec3[16]; //c++
	Vec3 *unvec = new Vec3(); //c++ alloc dynamique
	Vec3 unvecstatic = Vec3();// c++ alloc statique*/


	const char * source = "mon lapin est dodu";
	char * dest = (char*)calloc(1024, sizeof(char));
	Strcpy(dest, source);
	printf("Lenght : %d\n", Strlenght(source));
	printf("Dest : %s", dest);
}