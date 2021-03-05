#include<iostream>
#include<time.h>

enum ComboType { POKER, KARE, FULL_HOUSE, BIG_STRAIGHT, LITTLE_STRAIGHT, SET, TWO_PAIRS, PAIR, NOTHING };

typedef int (*checkCombo) (int*);

const char ComboNames[9][50] = {
	"ПОКЕР",
	"Каре",
	"Фулл хауз",
	"Большой стрейт",
	"Малый стрейт",
	"Сет",
	"Две пары",
	"Пара",
	"Ничего"
};

struct Combo
{
	ComboType type;
	int points;
};

int getD(int dice)
{
	return rand() % dice + 1;
}

void swap(int& a, int& b)
{
	int c = a;
	a = b;
	b = c;
}

void sortHand(int* hand, int size)
{
	for (int j = 0; j < size - 1; j++)
		for (int i = 0; i < size - 1; i++)
			if (hand[i] < hand[i + 1])
				swap(hand[i], hand[i + 1]);
}

void generateHand(int* hand)
{
	for (int i = 0; i < 5; i++)
		hand[i] = getD(6);
}

void printHand(int* hand, bool isEndLine = false)
{
	for (int i = 0; i < 5; i++)
		printf("%i ", hand[i]);
	if (isEndLine)
		printf("\n");
}

void copyHand(int* dstHand, int* srcHand)
{
	for (int i = 0; i < 5; i++)
		dstHand[i] = srcHand[i];
}

int getHandPoints(int* hand)
{
	int points = 0;
	for (int i = 0; i < 5; i++)
		points += hand[i];
	return points;
}

int checkPoker(int* hand)
{
	int counter = 0;
	for (int i = 1; i < 5; i++)
		if (hand[i] == hand[0])
			counter++;

	if (counter == 4) return counter * hand[0];
	
	return 0;
}

int checkKare(int* hand)
{
	int counter = 0;
	for (int i = 1; i < 4; i++)
		if (hand[i] == hand[0])
			counter++;
	if (counter == 3) return 4 * hand[0];

	counter = 0;
	for (int i = 2; i < 5; i++)
		if (hand[i] == hand[1])
			counter++;
	if (counter == 3) return 4 * hand[1];

	return 0;
}

int checkFullHouse(int* hand)
{
	int points = getHandPoints(hand);

	// сначала тройка, потом пара
	if (hand[0] == hand[1] && hand[0] == hand[2]     &&     hand[3] == hand[4]) return points;

	// сначала пара, потом тройка
	if (hand[0] == hand[1]     &&     hand[2] == hand[3] && hand[2] == hand[4]) return points;

	return 0;
}

int checkStraight(int* hand, int firstDice)
{
	int points = getHandPoints(hand);

	int rightDice = firstDice;
	for (int i = 0; i < 5; i++)
	{
		if (hand[i] != rightDice) return 0;
		rightDice--;
	}

	return points;
}

int checkBigStraight(int* hand)
{
	return checkStraight(hand, 6);
}

int checkLittleStraight(int* hand)
{
	return checkStraight(hand, 5);
}

int checkSet(int* hand)
{
	int pos = -1;
	for (int i = 0; i < 3; i++)
		if (hand[i] == hand[i + 1] && hand[i] == hand[i + 2])
			pos = i;
	if (pos < 0) return 0;
	return 3 * hand[pos];
}

int checkTwoPairs(int* hand)
{
	int points = getHandPoints(hand);

	if (hand[0] == hand[1] && hand[2] == hand[3]) return points - hand[4];
	if (hand[0] == hand[1] && hand[3] == hand[4]) return points - hand[2];
	if (hand[1] == hand[2] && hand[3] == hand[4]) return points - hand[0];

	return 0;
}

int checkPair(int* hand)
{
	int pos = -1;
	for (int i = 0; i < 4; i++)
		if (hand[i] == hand[i + 1])
			pos = i;
	if (pos < 0) return 0;
	return 2 * hand[pos];
}

checkCombo checks[9] = {
	checkPoker, checkKare, checkFullHouse, checkBigStraight,
	checkLittleStraight, checkSet, checkTwoPairs, checkPair,
	getHandPoints
};

Combo findComboInHand(int* hand)
{
	int thand[10];
	copyHand(thand, hand);
	sortHand(thand, 5);

	Combo c;

	for (int i = 0; i < 9; i++)
	{
		c.points = checks[i](thand);
		if (c.points > 0)
		{
			c.type = ComboType(i);
			break;
		}
	}

	return c;
}

void printCombo(Combo c)
{
	printf("Комбинация: %s, очков в ней: %i\n", ComboNames[c.type], c.points);
}

bool operator<(Combo& c1, Combo& c2)
{
	if (c1.type < c2.type) return false;
	if (c1.type > c2.type) return true;
	if (c1.points > c2.points) return false;
	if (c1.points < c2.points) return true;
	return false;
}

bool operator<=(Combo& c1, Combo& c2)
{
	if (c1.type < c2.type) return false;
	if (c1.type > c2.type) return true;
	if (c1.points > c2.points) return false;
	if (c1.points < c2.points) return true;
	return true;
}

bool operator==(Combo& c1, Combo& c2)
{
	if (c1.type == c2.type && c1.points == c2.points) return true;
	return false;
}

bool operator>(Combo& c1, Combo& c2)
{
	return c2 < c1;
}

bool operator>=(Combo& c1, Combo& c2)
{
	return c2 <= c1;
}

bool operator!=(Combo& c1, Combo& c2)
{
	return !(c1 == c2);
}

void changeHand(int* hand)
{
	printf("Сколько костей вы хотите перебросить?\n");
	
	int reroll = 0;
	do
	{
		scanf_s("%i", &reroll);
		if (reroll < 0 || reroll > 5)
			printf("У нас всего 5 костей, такого количества не может быть!\n");
	} while (reroll < 0 || reroll > 5);

	if (reroll == 0) return;
	
	int dices[5];
	if (reroll < 5)
	{
		for (int i = 0; i < reroll; i++)
		{
			bool again;
			do
			{
				again = false;
				printf("Введите номер кости: ");
				scanf_s("%i", &dices[i]);
				for (int j = 0; j < i; j++)
					if (dices[j] == dices[i])
						again = true;
				if (again)
					printf("Такую кость вы уже называли!\n");
				if (dices[i] < 1 || dices[i] > 5)
					printf("У нас всего 5 костей, таких номеров не бывает\n");
			} while (again || dices[i] < 1 || dices[i] > 5);
		}
	}
	else
	{
		for (int i = 0; i < reroll; i++)
			dices[i] = i + 1;
	}

	for (int i = 0; i < reroll; i++)
		hand[dices[i]-1] = getD(6);
}

int main()
{
	system("chcp 1251 > nul");
	int seed = time(0);
	srand(seed);

	int phand[5], ehand[5];
	generateHand(phand); Combo pCombo = findComboInHand(phand);
	generateHand(ehand); Combo eCombo = findComboInHand(ehand);

	printf("Ваша рука:\t "); printHand(phand, true); printCombo(pCombo);

	printf("\n\n");

	printf("Рука противника: "); printHand(ehand, true); printCombo(eCombo);

	printf("\n\n");

	changeHand(phand);
	pCombo = findComboInHand(phand);

	system("cls");

	printf("Ваша рука:\t "); printHand(phand, true); printCombo(pCombo);

	printf("\n\n");

	printf("Рука противника: "); printHand(ehand, true); printCombo(eCombo);

	printf("\n\n");

	if (pCombo > eCombo)
		printf("Вы победили!\n\n");
	else if (pCombo == eCombo)
		printf("Ничья.\n\n");
	else
		printf("Вы проиграли...\n\n");

	system("pause");
	return 0;
}