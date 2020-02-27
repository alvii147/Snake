#include <stdio.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned int count = 0;
int snake[64];
int direction;
int head;
int tail;
int food;
int end = 0;

void move(int pixels[], int dir, int* h, int* t)
{
    int tempTail;
	int newFood = 0;
    if(dir == 0)
    {
        if(((*h) + 1) % 8)
        {
            pixels[*h] = (*h) + 1;
			if(pixels[(*h) + 1] == 65)
			{
				food = randomNum();
				while(pixels[food] != -1)
				{
					food = random();
				}
				newFood = 1;
			}
			else if(pixels[(*h) + 1] != -1)
			{
				end = 1;
				while(1)
				{
					
				}
			}
            pixels[(*h) + 1] = 64;

            *h = (*h) + 1;

            tempTail = *t;
			if(!newFood)
			{
				*t = pixels[*t];
				pixels[tempTail] = -1;
			}
        }
		else
		{
			end = 1;
			while(1)
			{
				
			}
		}
    }
    else if(dir == 1)
    {
        if(*h < 56)
        {
            pixels[*h] = (*h) + 8;
			if(pixels[(*h) + 8] == 65)
			{
				food = randomNum();
				while(pixels[food] != -1)
				{
					food = random();
				}
				newFood = 1;
			}
			else if(pixels[(*h) + 8] != -1)
			{
				end = 1;
				while(1)
				{
					
				}
			}
            pixels[(*h) + 8] = 64;
            
            *h = (*h) + 8;
            
            tempTail = *t;
            if(!newFood)
			{
				*t = pixels[*t];
				pixels[tempTail] = -1;
			}
        }
		else
		{
			end = 1;
			while(1)
			{
				
			}
		}
    }
    else if(dir == 2)
    {
        if((*h) % 8)
        {
            pixels[*h] = (*h) - 1;
			if(pixels[(*h) - 1] == 65)
			{
				food = randomNum();
				while(pixels[food] != -1)
				{
					food = random();
				}
				newFood = 1;
			}
			else if(pixels[(*h) - 1] != -1)
			{
				end = 1;
				while(1)
				{
					
				}
			}
            pixels[(*h) - 1] = 64;

            *h = (*h) - 1;

            tempTail = *t;
            if(!newFood)
			{
				*t = pixels[*t];
				pixels[tempTail] = -1;
			}
        }
		else
		{
			end = 1;
			while(1)
			{
				
			}
		}
    }
    else if(dir == 3)
    {
        if(*h > 7)
        {
            pixels[*h] = (*h) - 8;
			if(pixels[(*h) - 8] == 65)
			{
				food = randomNum();
				while(pixels[food] != -1)
				{
					food = random();
				}
				newFood = 1;
			}
            pixels[(*h) - 8] = 64;

            *h = (*h) - 8;

            tempTail = *t;
            if(!newFood)
			{
				*t = pixels[*t];
				pixels[tempTail] = -1;
			}
        }
		else
		{
			end = 1;
			while(1)
			{
				
			}
		}
    }
	pixels[food] = 65;
}

void changeDirection(int* dir, char newDir)
{
    if(newDir == 'u' && *dir != 1)
    {
        *dir = 3;
    }
    else if(newDir == 'd' && *dir != 3)
    {
        *dir = 1;
    }
    else if(newDir == 'l' && *dir != 0)
    {
        *dir = 2;
    }
    else if(newDir == 'r' && *dir != 2)
    {
        *dir = 0;
    }
}

void ledMatrix(int i, int j)
{
	PORTC = 0b00000000;
	PORTD = 0b00000000;
	PORTB = 0b00000000;
	if(i == 6)
	{
		if(j == 6)
		{
			PORTB = 0b10100000;
			PORTC = 0b11111111;
			PORTD = 0b00000000;
		}
		else if(j == 7)
		{
			PORTB = 0b11000000;
			PORTC = 0b11111111;
			PORTD = 0b00000000;
		}
		else
		{
			PORTB = 0b11100000;
			PORTC = (1 << (j - 1)) ^ (0b11111111);
			PORTD = (1 << (i - 1));
		}
	}
	else if(j == 6)
	{
		PORTB = 0b00100000;
		PORTC = (1 << (j - 1)) ^ (0b11111111);
		PORTD = (1 << (i - 1));
	}
	else if(j == 7)
	{
		PORTB = 0b01000000;
		PORTC = (1 << (j - 1)) ^ (0b11111111);
		PORTD = (1 << (i - 1));
	}
	else
	{
		PORTB = 0b01100000;
		PORTC = (1 << (j - 1)) ^ (0b11111111);
		PORTD = (1 << (i - 1));
	}
}

void POV(int data[])
{
	for(int i = 0; i < 64; i++)
	{
		if(data[i] != -1)
		{
			ledMatrix((i/8) + 1, (i%8) + 1);
			_delay_ms(1);
		}
	}
}

void adc_init()
{
	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

uint16_t adc_read(uint8_t ch)
{
	ch &= 0b00000111;
	ADMUX = (ADMUX & 0xF8)|ch;
	ADCSRA |= (1<<ADSC);
	while(ADCSRA & (1<<ADSC));
	
	return (ADC);
}

int randomNum()
{
	return rand() % 64;
}

void main()
{
	DDRD = 0b11111111;
	DDRC = 0b11111111;
	DDRB = 0b11111111;
	adc_init();
	srand(2);
	
    for(int i = 0; i < 64; i++)
    {
        if(i < 2)
        {
            snake[i] = i + 1;
        }
        else if(i == 2)
        {
            snake[i] = 64;
        }
        else
        {
            snake[i] = -1;
        }
	}
	
	direction = 1;
    head = 2;
    tail = 0;
	food = randomNum();
	while(snake[food] != -1)
	{
		food = randomNum();
	}
	snake[food] = 65;
	
	while(1)
	{
		TCCR0 |= (1<<CS00);
		TCCR0 &= ~(1<<CS01);
		TCCR0 &= ~(1<<CS02);
		sei();
		TIMSK |= (1<<TOIE0);
		TCNT0 = 0;
		
		POV(snake);
	}
}

ISR(TIMER0_OVF_vect)
{
	count++;
	
	if(!(count / 5) && !end)
	{
		if(adc_read(0) < 100)
		{
			changeDirection(&direction, 'u');
		}
		else if(adc_read(0) > 923)
		{
			changeDirection(&direction, 'd');
		}
		else if(adc_read(1) < 100)
		{
			changeDirection(&direction, 'r');
		}
		else if(adc_read(1) > 923)
		{
			changeDirection(&direction, 'l');
		}
	}
	
	if(count == 7812 && !end)
	{
		move(snake, direction, &head, &tail);
		count = 0;
	}
}