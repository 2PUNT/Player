#include "hwlib.hpp"
#include "rtos.hpp"

/* bool DisplayControl::compareCharArray(char* firstArray, char* secondArray){
	index = 0;
	char firstChar = firstArray[index];
	char secondChar = secondArray[index];
	while(true){
		if(firstChar != secondChar) return false;
		if(firstChar == '\0') return true;
		
		index++;
		firstChar = firstArray[index];
		secondChar = secondArray[index];
	}
} */

void DisplayControl::DisplayString(char* s, StringType ID){
	switch(ID){
		case RELOAD:
			if(currentTextEntireScreen) Clear(ENTIRE_SCREEN);
			
			Clear(ID); // clear current screen
			if(s[0] == '\0')return // nothing to display
			currentTextReload = true; // else, there is something to display
			int Index = 0; // index of the current char
			char currentChar = s[Index]; // load the current Char
			while(currentChar != '\0'){ // write all the characters until we encounter a \0
				reloadOstream << currentChar;
				currentChar = s[Index];
			}
			reloadOstream << "\n" << hwlib::flush; // flush the display
		
		case HEALTH:
			hwlib::cout << "Cannot display char on Health screen!!\n"; // writing characters to an healthBar?
			
		case ENTIRE_SCREEN:
			Clear(ID); // clear current screen
			if(s[0] == '\0')return
			currentTextEntireScreen = true;
			int Index = 0;
			char currentChar = s[Index];
			while(currentChar != '\0'){
				entireScreenOstream << currentChar;
				currentChar = s[Index];
			}
			entireScreenOstream << "\n" << hwlib::flush;
	}
}
void DisplayControl::DisplayString(int s, StringType ID){
	switch(ID){
		case RELOAD:
			if(currentTextEntireScreen) Clear(ENTIRE_SCREEN);
			
			Clear(ID); // clear current screen
			if(s[0] == '\0')return
			
			currentTextReload = true;
			
			int index = s;
			int sLength = 0;
			while(index > 1){
				sLength++
				index = index/10;
			}
			
			index = 0;
			char currentChar = '0'
			int currentNumber = s;
			while(index < sLength){ // TODO: convert int to chars
				currentChar = '0' + currentNumber;
				reloadOstream << currentChar;
			}
			reloadOstream << "\n" << hwlib::flush;
		
		case HEALTH:
			hwlib::cout << "Cannot display char on Health screen!!\n";
			
		case ENTIRE_SCREEN:
			if(currentTextReload) Clear(RELOAD);
			if(currentTextHealth) Clear(HEALTH);
			
			Clear(ID); // clear current screen
			if(s[0] == '\0')return
			currentTextEntireScreen = true;
			int Index = 0;
			char currentChar = s[Index];
			while(currentChar != '\0'){
				entireScreenOstream << currentChar;
				currentChar = s[Index];
			}
			entireScreenOstream << "\n" << hwlib::flush;
	}
}
void DisplayControl::Clear(StringType ID);

/*
enum StringType{
	RELOAD,
	HEALTH,
	ENTIRE_SCREEN
};

class DisplayControl{
private:
	hwlib::window& display; // reference to the Display
	hwlib::window_part reloadDisplay; // the part of the Display that shows whether the weapon is reloading
	hwlib::window_part healthDisplay; // the part of the Display that shows the healthBar
	hwlib::window_ostream reloadOstream; // Ostream to write characters to the reloadScreen
	hwlib::window_ostream entireScreenOstream; // Ostream to write characters to the Entire screen
	
	int health; // variable to store the current health displayed.
	bool currentTextReload; // bool to store whether there is something is currently displayed on the Reload window.
	bool currentTextHealth; // bool to store whether there is something is currently displayed on the Health window.
	bool currentTextEntireScreen; // bool to store whether there is something is currently displayed on the entire screen window.
	
	//bool compareCharArray(char* firstArray, char* secondArray);
	
public:
	DisplayControl(hwlib::window& _display, int _health = -1):
		display(_display),
		reloadDisplay(display, hwlib::location(0,0), hwlib::location(display.size.x/2, display.size.y/2)),
		healthDisplay(display, hwlib::location(display.size.x/2 + 1, display.size.y/2 + 1), hwlib::location(display.size.x - 1, display.size.y - 1)),
		reloadOstream(reloadDisplay, hwlib::font_default_8x8),
		entireScreenOstream(display, hwlib::font_default_8x8),
		health(_health), currentTextReload(false), currentTextHealth(false), currentTextEntireScreen(false){
			if(health != -1)
				// TODO initialize HealthBar
		}
	
	void DisplayString(char* s, StringType ID);
	void DisplayString(int s, StringType ID);
	void Clear(StringType ID);
};
*/