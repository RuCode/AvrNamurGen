/*
 * Test.c
 *
 * Created: 17.06.2015 22:43:16
 *  Author: Anton
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

#define bit_test(var, bitno)((var) | 1 << (bitno))


// ��� ����������
#define PIN_RELE 0b01000000
#define RELE_OPEN 0b01000000
#define RELE_CLOSE 0b00000000

#define PIN_A 0b00000010
#define PIN_F 0b00000001

#define PIN_B 0b00000010
#define PIN_C 0b00001000
#define PIN_D 0b00010000
#define PIN_E 0b00100000
#define PIN_G 0b00000100

short int impulseCount = 1; 
char rele_state = 0b00000000; // ��������� ����
char segm_state = 0b00000000; // ��������� 7-�����������

void init_io(void);

void SetReleState(char NewReleState)
{
	PORTD = 0;
	rele_state = NewReleState;
	PORTD = segm_state | rele_state;	
}

void PrintNum(short int Value)
{
	PORTA = 0;
	PORTD = 0;
	switch (Value) {
	case 0:
		PORTA = PIN_A | PIN_F;
		segm_state = PIN_B | PIN_C | PIN_D | PIN_E; 
		break;
	case 1:
		segm_state = PIN_B | PIN_C;
		break;
	case 2:
		PORTA = PIN_A;
		segm_state = PIN_B | PIN_G | PIN_D | PIN_E;
		break;
	case 3:
		PORTA = PIN_A;
		segm_state = PIN_B | PIN_C | PIN_D | PIN_G;
		break;
	case 4:
		PORTA = PIN_F;
		segm_state = PIN_B | PIN_C | PIN_G;
		break;
	case 5:
		PORTA = PIN_A | PIN_F;
		segm_state = PIN_G | PIN_C | PIN_D;
		break;
	case 6:
		PORTA = PIN_A | PIN_F;
		segm_state = PIN_G | PIN_C | PIN_D | PIN_E; 
		break;
	case 7:
		PORTA = PIN_A ;
		segm_state = PIN_B | PIN_C;
		break;
	case 8:
		PORTA = PIN_A | PIN_F;
		segm_state = PIN_B | PIN_C | PIN_D | PIN_E | PIN_G;
		break;
	case 9:
		PORTD = 0;
		PORTA = 0;
		PORTA = PIN_A + PIN_F;
		segm_state = PIN_B + PIN_G + PIN_D + PIN_C;
		break;
	default:
		segm_state = PIN_G;
		break;
	}
	// ��������� ������ ��� ����� D
	PORTD = segm_state | rele_state;
}

void init_io(void)
{
	// �� ������ ��������������� A � F ����������� � ����� A
	PORTA = 0b00000011;
	DDRA = 0b00000011;
	// � ��������� � ����� D �� �������� ������������ XD
	PORTD = 0b11111111;
	DDRD =  0b11111111;
	// ��� ������
	DDRB = 0;
	PORTB = 0xFF;
}

int ReadButton(int PinBtn) {
	// ���� ������ ������ �� ���������� 1
	int result = 0;
	if ((PINB&(1 << PinBtn)) == 0) { // ���� ������ �����
		_delay_ms(20); // ��� 0.020 ���
		if ((PINB&(1 << PinBtn)) == 0) // ������ ������ �����
		{
			// ���� ��� ���������, �� ������ ������
			result = 1;
			// ������ �� ����� ����� ���� ���� �� �������� ������
			while ((PINB&(1 << PinBtn)) == 0)
			{
				// � ���� ��� ��� �� �������� �� ����������� ������� ������� ������
			}
		}
	}
	return result;
}

int main(void)
{
	//short int Test = 0;
	init_io();
	PrintNum(impulseCount);
    while(1)
    {
		// ������ ������ ������ - ������� ������
		if (ReadButton(PB0) == 1) {
			for (int i = impulseCount; i > 0; --i) {
				PrintNum(i);
				SetReleState(RELE_OPEN);
				_delay_ms(500);
				SetReleState(RELE_CLOSE);
				_delay_ms(1000);
				impulseCount = i;
			}
		// ������� ������ ������ - + 1
		} else if (ReadButton(PB1) == 1) {//if ((PINB&(1 << PB1)) == 0) {
			// ���� �������, �� ����� ����������
			if (impulseCount == 1)
			{
				PrintNum(impulseCount);
				continue;
				} else {
				// ����� ������ -1
				impulseCount = impulseCount - 1;
				PrintNum(impulseCount);
			}
		// ����� ������ ������ - - 1
		} else if (ReadButton(PB2) == 1) {
			// ���� ��������, �� ����� ����������
			if (impulseCount == 9)
			{
				PrintNum(impulseCount);
				continue;
				} else {
				// ����� ������ +1
				impulseCount = impulseCount + 1;
				PrintNum(impulseCount);
			}
		// �� ���� �� ������ :)
		} else {
			//PrintNum(impulseCount);
			//_delay_ms(100);
		} 
    }
}