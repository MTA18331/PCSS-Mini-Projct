// DiceRoll.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <string>
#include <ctime>
#include <iomanip>
#include "vars.h" 
#include "player.h"

using namespace std;

int choice();
int roll(); //Declare Function
int again();

//char choicen; // Used for the first choice - if you want to play the game
//char choicen2; // Used for the second choice - if you want to play again


int main()
{
	player player1;
	player1.name = "Jarlo";
	player1.printname();


	choice();

	if (choicen == 'y') {
		cout << "Cool beans, bro" << endl;
		cout << endl;
		roll();  // Calling Function
	}
	else {
		cout << "Fuck you " << player1.name << endl;
		cout << endl;
	}

	system("PAUSE");
	return 0;
}



int choice() {
	cout << "Do you wanna play dice?" << endl;
	cout << "Press 'y' for yes or 'n' for no " << endl;
	cout << endl;
	cin >> choicen;
	cout << endl;

	return 0;
}


int roll() {
	srand(time(NULL));

	int number = 6;
	int result = 0;

	result = rand() % number + 1;

	cout << "You rolled: " << result << endl;

	switch (result) {
	case 1:
		cout << "Bummer, you lost" << endl;
		break;
	case 6:
		cout << "Yay, you won" << endl;
		break;
	default:
		cout << "Try again" << endl;
		break;
	}


	again();
	return 0;
}


int again() {
	cout << endl;
	cout << "You wanna play again?" << endl;
	cout << "Press 'y' for yes or 'n' for no " << endl;
	cout << endl;
	cin >> choicen2;

	if (choicen2 == 'y') {
		cout << endl;
		roll();
	}

	else {
		cout << endl;
		cout << "K, bye" << endl;
	}

	return 0;
}

