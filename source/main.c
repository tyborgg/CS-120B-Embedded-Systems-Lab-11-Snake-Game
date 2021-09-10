/*	Author: Tyler Pastor tpast001@ucr.edu
 *  Partner(s) Name: N/A
 *	Lab Section: 023
 *	Assignment: Lab #11
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *
 *	Demo Link: https://www.youtube.com/watch?v=0gpvBb3rfPo
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "PWM.h"
#include "bit.h"
#include "scheduler.h"
#include <time.h>
#include <stdlib.h>
#endif

//For the Joystick
unsigned short LR;
unsigned short UD;

void A2D_init() {
      ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: Enables analog-to-digital conversion
	// ADSC: Starts analog-to-digital conversion
	// ADATE: Enables auto-triggering, allowing for constant
	//	    analog to digital conversions.
}

// Pins on PORTA are used as input for A2D conversion
	//    The default channel is 0 (PA0)
	// The value of pinNum determines the pin on PORTA
	//    used for A2D conversion
	// Valid values range between 0 and 7, where the value
	//    represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum) {
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for ( i=0; i<15; i++ ) { asm("nop"); }
}
//////////////////////////////////////////////////////

char prev = 'a';
int val;
int cnt;
int begin = 1;
int next = 1;
int flag;
int wait = 1;
unsigned char currC;
unsigned char currD;
unsigned char appleC;
unsigned char appleD;
unsigned char A;
int snakeSize;
int sound;
int moveNoise = -1;
unsigned char cVal[40];
unsigned char dVal[40];
unsigned char tempC[40];
unsigned char tempD[40];
int apple = 0;
int arrayIndex = 38;
int rVal = 0;
int lVal = 0;
int rightNum;
int leftNum;
int side = 0;
int score;
int waitVal = 0;
int cnt = 0;
int check = 0;

////////////////////////////////////////////////////////

enum userControl {shift, waitLR, waitUD};
int dirInput(int state) {
	switch (state) {
		case shift:
			
			if(!(UD > 768 && LR > 768) && !(UD > 768 && LR < 256) && !(UD < 256 && LR > 768) && !(UD < 256 && LR < 256)){ //prevents diagonal movement of the snake
				if(LR > 750){
					if(prev != 'l'){
						prev = 'r';
						moveNoise = 1;
						state = waitLR;
					}
				}
				else if(LR < 274){
					if(prev != 'r' && wait == 0){
						prev = 'l';
						moveNoise = 0;
						state = waitLR;
					}
				}
				else if(UD > 750){
					if(prev != 'd'){
						prev = 'u';
						moveNoise = 1;
						state = waitUD;
					}
                                }
                                else if(UD < 274){
					if(prev != 'u'){
						prev = 'd';
						moveNoise = 0;
						state = waitUD;
					}
                                }
			}
			else{
				state = shift;
			}
			break;	

		case waitLR:
			if(LR > 750 || LR < 274){
				state = waitLR;
			}
			else{
				state = shift;
			}
			break;

		case waitUD:
			if(UD > 750 || UD < 274){
                                state = waitUD;
                        }
                        else{
                                state = shift;
                        }
                        break;

		default:
			state = shift;
			break;
	}

	switch(state){
                case shift:
                        break;

                case waitLR:
                        break;

		case waitUD:
			break;
        }
	
	return state;
}

////////////////////////////////////////////////////////

enum gameOver{animate};
int lightShift(int state){
	switch(state){
		case animate:
			if((A & 0xF0) == 0x10){
				flag = 1;
				val = 2;
			}

			if(flag == 1){
				set_PWM(0);

				if(val == 0 && cnt == 0){
					PORTC = 0x66;
					PORTD = 0x1C;
					cnt = 1;
				}
				else if(val == 0 && cnt == 1){
					PORTD = 0x0F;
					PORTC = 0x42;
					cnt = 2;
				}
				else if(val == 0 && cnt == 2){
        	                        PORTD = 0x17;
                	                PORTC = 0x3C;
					cnt = 0;
				}
				else if(val == 1 && side == 0){
					if(rightNum == 0){
						if(rVal == 0){
							PORTC = 0x07;
							PORTD = 0x0E;
							rVal = 1;
						}
						else{
							PORTC = 0x05;
							PORTD = 0x00;
							rVal = 0;
						}
					}
					else if(rightNum == 1){
						if(rVal == 0){
							PORTC = 0x02;
							PORTD = 0x00;	
							rVal = 1;
						}
						else{
							PORTC = 0x04;
							PORTD = 0x1D;
							rVal = 0;
						}
					}
					else if(rightNum == 2){
						if(rVal == 0){
							PORTC = 0x07;
							PORTD = 0x0A;
							rVal = 1;
						}
						else if(rVal == 1){
							PORTC = 0x01;
							PORTD = 0x1D;
							rVal = 2;
						}
						else{ 
							PORTC = 0x04;
							PORTD = 0x17;
							rVal = 0;
						}
					}
					else if(rightNum == 3){
						if(rVal == 0){
							PORTC = 0x07;
							PORTD = 0x0A;
							rVal = 1;
						}
						else{
							PORTC = 0x01;
							PORTD = 0x15;
							rVal = 0;
						}
					}
					else if(rightNum == 4){
						if(rVal == 0){
							PORTC = 0x01;
							PORTD = 0x00;
							rVal = 1;
						}
						else if(rVal == 1){
							PORTC = 0x02;
							PORTD = 0x1B;
							rVal = 2;
						}
						else{
							PORTC = 0x04;
							PORTD = 0x18;
							rVal = 0;
						}
                                        }
                                        else if(rightNum == 5){
						if(rVal == 0){
							PORTC = 0x07;
							PORTD = 0x0A;
							rVal = 1;
						}
						else if(rVal == 1){
							PORTC = 0x01;
							PORTD = 0x17;
							rVal = 2;
						}
						else{
							PORTC = 0x04;
							PORTD = 0x1D;
							rVal = 0;
						}
                                        }
					else if(rightNum == 6){
						if(rVal == 0){
							PORTC = 0x07;
							PORTD = 0x0A;
							rVal = 1;
						}
						else if(rVal == 1){
							PORTC = 0x01;
							PORTD = 0x17;
							rVal = 2;
						}
						else{
							PORTC = 0x04;
							PORTD = 0x15;
							rVal = 0;
						}
					}
                                        else if(rightNum == 7){
                                        	if(rVal == 0){
							PORTC = 0x01;
							PORTD = 0x00;
							rVal = 1;
						}
						else if(rVal == 1){
							PORTC = 0x02;
							PORTD = 0x1E;
							rVal = 2;
						}
						else{
							PORTC = 0x04;
							PORTD = 0x1C;
							rVal = 0;
						}

                                        }
                                        else if(rightNum == 8){
						if(rVal == 0){
							PORTC = 0x07;
							PORTD = 0x0A;
							rVal = 1;
						}
						else{
							PORTC = 0x05;
							PORTD = 0x15;
							rVal = 0;
						}
                                        }
                                        else if(rightNum == 9){
						if(rVal == 0){
							PORTC = 0x07;
							PORTD = 0x1A;
							rVal = 1;
						}
						else if(rVal == 1){
							PORTC = 0x01;
							PORTD = 0x00;
							rVal = 2;
						}
						else{
							PORTC = 0x04;
							PORTD = 0x1D;
							rVal = 0;
						}
                                        }

					side = 1;
				}
				else if(val == 1 && side == 1){
					if(leftNum == 0){
                                                if(lVal == 0){
                                                        PORTC = 0xE0;
                                                        PORTD = 0x0E;
                                                        lVal = 1;
                                                }
                                                else{
                                                        PORTC = 0xA0;
                                                        PORTD = 0x00;
                                                        lVal = 0;
                                                }
                                        }
                                        else if(leftNum == 1){
                                                if(lVal == 0){
                                                        PORTC = 0x20;
                                                        PORTD = 0x00;
                                                        lVal = 1;
                                                }
                                                else{
                                                        PORTC = 0x40;
                                                        PORTD = 0x1D;
                                                        lVal = 0;
                                                }
                                        }
                                        else if(leftNum == 2){
                                                if(lVal == 0){
                                                        PORTC = 0xE0;
                                                        PORTD = 0x0A;
                                                        lVal = 1;
                                                }
                                                else if(lVal == 1){
                                                        PORTC = 0x80;
                                                        PORTD = 0x1D;
                                                        lVal = 2;
                                                }
                                                else{
                                                        PORTC = 0x20;
                                                        PORTD = 0x17;
                                                        lVal = 0;
                                                }
                                        }
					else if(leftNum == 3){
                                                if(lVal == 0){
                                                        PORTC = 0xE0;
                                                        PORTD = 0x0A;
                                                        lVal = 1;
                                                }
                                                else{
                                                        PORTC = 0x80;
                                                        PORTD = 0x15;
                                                        lVal = 0;
                                                }
                                        }

					side = 0;
				}
				else if(val == 2){
  		                        for(int i = 0; i < 40; i++){
                				cVal[i] = 0x00;
                				dVal[i] = 0x00;
        				}

        				PORTC = 0x70;
        				PORTD = 0x1B;
        				currC = 0x10;
	       				currD = PORTD;
        				cVal[0] = 0x10;
        				dVal[0] = PORTD;
					cVal[1] = 0x20;
					dVal[1] = PORTD;
					cVal[2] = 0x40;
					dVal[2] = PORTD;
					snakeSize = 3;
					arrayIndex = 0;
					begin = 1;
					wait = 1;
					rVal = 0;
					lVal = 0;
					side = 0;
					score = 0;
					val = 0;
                                        flag = 0;
                                        waitVal = 0;
                                        prev = 'a';
                       		}
			}

			state = animate;
			break;

		default:
			state = animate;
			break;
	}

	switch(state){
		case animate:
			break;
	}

	return state;
}

////////////////////////////////////////////////////////

enum constantSpeed{movement};
int snakeMovement(int state) {
	switch(state){
		case movement:
			if((PORTD == 0x1F || PORTD == 0x1F || PORTC > 0x80 || PORTC == 0x00) || flag == 1){
				if(score == 0){
					snakeSize -= 3;
					leftNum = snakeSize / 10;
					rightNum = snakeSize % 10;
					score = 1;
				}

				if(flag == 0){
					flag = 1;
				}
				else if(val != 2){
					if(waitVal == 3){
						if(val == 0){
							val = 1;
						}
						else{
							val = 0;
						}
						waitVal = 0;
					}
					else{
						waitVal++;
					}
				}
			}
			else if(prev != 'a'){
				for(unsigned int i = 0; i < 40; i++){
					tempC[i] = cVal[i];
					tempD[i] = dVal[i];
				}

				if(prev == 'u'){
					if(wait == 1){
						wait = 0;
					}
					currD = (currD >> 1) | 0x10;
				}
				else if(prev == 'd'){
					if(wait == 1){
                                                wait = 0;
                                        }
					currD = ((currD << 1) | 0x01) & 0x1F;
				}
				else if(prev == 'r'){
					if(wait == 1){
                                                wait = 0;
                                        }
					currC = currC >> 1;
				}
				else if(prev == 'l'){
					currC = currC << 1;  
				}
				
				if(cVal[0] == appleC && dVal[0] == appleD && apple == 1){
					apple = 0;
					snakeSize++;			
					sound = 1;
				}

				if(snakeSize == 40){
					flag = 1;
				}
				
				for(unsigned int i = 0; i < snakeSize; i++){
					if(currC == cVal[i] && currD == dVal[i]){
						flag = 1;
						set_PWM(0);
					}
				}
				
				if(flag != 1){
					cVal[0] = currC;
					dVal[0] = currD;

					for(unsigned int i = 1; i < snakeSize; i++){
						cVal[i] = tempC[i-1];
						dVal[i] = tempD[i-1];
					}
				}
			}

			state = movement;
			break;
		
		default:
			state = movement;
			break;
	}

	switch(state){
		case movement:
			break;
	}

	return state;
}

////////////////////////////////////////////////////////

enum snakeSounds {noise};
int dirAndEating(int state){
	switch(state){
		case noise:
			if(sound == 1 && cnt == 0){
				set_PWM(300);
				cnt++;
				check = 1;
			}
			else if(sound == 1 && cnt == 1){
				set_PWM(500);
				sound = 0;
				cnt = 0;
				check = 1;
			}
			else{
				set_PWM(0.0);
			}

			if(check == 0){
				if(moveNoise == 0){
                                	set_PWM(600);
                                	moveNoise = -1;
                        	}
                      	  	else if(moveNoise == 1){
                        	        set_PWM(700);
                	                moveNoise = -1;         
        	                }
	                        else{
                                	set_PWM(0.0);
                        	}
			}

			check = 0;

			state = noise;
			break;

		default:
			state = noise;
			break;
	}

	switch(state){
		case noise:
			break;
	}

	return state;
}

////////////////////////////////////////////////////////

enum TrackingSnakehead {length};
int snakeLength(int state) {
	switch(state){
		case length:
			if(flag != 1){			
				if(cVal[arrayIndex] != 0x00){ 
					PORTC = cVal[arrayIndex];
					PORTD = dVal[arrayIndex];
					arrayIndex++;
				}
				else{
					PORTC = appleC;
					PORTD = appleD;
					arrayIndex = 0;
				}
			}

			state = length;
			break;

		default:
			state = length;
			break;
	}

	switch(state){
		case length:
			break;
        }

	return state;
}

////////////////////////////////////////////////////////

enum bodyGrowing {apples};
int snakeGrowth(int state){
	switch(state){
		case apples:
			if(begin == 1){
				apple = 1;
				appleC = 0x02;
				appleD = 0x1B;
				begin = 0;
			}

			if(apple == 0){
				int C = rand() % 8; //0 to 7
				int D = rand() % 5;   //0 to 4
				
				if(C == 0){
					appleC = 0x01;
				}
				else if(C == 1){
					appleC = 0x02;
				}
				else if(C == 2){
	                                appleC = 0x04;
                        	}
                	        else if(C == 3){
        	                        appleC = 0x08;
	                        }
				else if(C == 4){
                	        	appleC = 0x10;
               	        	}
                        	else if(C == 5){
                	                appleC = 0x20;
        	                }
      	                	else if(C == 6){
                        	        appleC = 0x40;
                  	 	}
                        	else if(C == 7){
                	                appleC = 0x80;
        	                }
	
				if(D == 0){
					appleD = 0x1E;
				}
				else if(D == 1){
                	                appleD = 0x1D;
        	                }
	                        else if(D == 2){
                                	appleD = 0x1B;
                        	}
                        	else if(D == 3){
                	                appleD = 0x17;
        	                }
	                        else if(D == 4){
                        	        appleD = 0x0F;
                	        }
				
				for(unsigned int i = 0; i < snakeSize; i++){
					if(appleC == cVal[i] && appleD == dVal[i]){
						apple = 0;
						break;
					}
					else{ 
						apple = 1;
					}
				}
			}

			state = apples;
			break;

		default:
			state = apples;
			break;
	}
	
	switch(state){
		case apples:
			break;
	}

	return state;
}

////////////////////////////////////////////////////////

int main(void) {
	srand(time(NULL));

	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTD = 0x00;
    	DDRD = 0xFF; PORTC = 0x00;

	//populate arrays with #0
	for(int i = 0; i < 40; i++){
		cVal[i] = 0x00;
		dVal[i] = 0x00;
	}

	PORTC = 0x70;
	PORTD = 0x1B;
	currC = 0x10;
	currD = PORTD;
	cVal[0] = 0x10;
	dVal[0] = PORTD;
	cVal[1] = 0x20;
	dVal[1] = PORTD;
	cVal[2] = 0x40;
	dVal[2] = PORTD;
	snakeSize = 3;

	//Call Func For The Joystick	
	A2D_init();

    	static task task1, task2, task3, task4, task5, task6;
    	task *tasks[] = {&task1, &task2, &task3, &task4, &task5, &task6};
    	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

    	const char start = -1;

    	task1.state = start;
    	task1.period = 1;
    	task1.elapsedTime = task1.period;
    	task1.TickFct = &dirInput;

	task2.state = start;
	task2.period = 300;
        task2.elapsedTime = task2.period;
        task2.TickFct = &snakeMovement;

	task3.state = start;
        task3.period = 1;
        task3.elapsedTime = task3.period;
        task3.TickFct = &lightShift;
	
	task4.state = start;
        task4.period = 1;
        task4.elapsedTime = task4.period;
        task4.TickFct = &snakeLength;

	task5.state = start;
        task5.period = 1;
        task5.elapsedTime = task5.period;
        task5.TickFct = &snakeGrowth;

	task6.state = start;
        task6.period = 100;
        task6.elapsedTime = task6.period;
        task6.TickFct = &dirAndEating;

	PWM_on();
    	TimerSet(1);
    	TimerOn();

	unsigned short i;
    	while (1) {
		A = ~PINA;
		Set_A2D_Pin(0x00);
		LR = ADC;
		Set_A2D_Pin(0x02);
                UD = ADC;

		for(i = 0; i < numTasks; i++){
 			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}	

		while(!TimerFlag);
		TimerFlag = 0;
    	}

    	return 0;
}
