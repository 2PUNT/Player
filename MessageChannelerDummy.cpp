#include "MessageChannelerDummy.hpp"


void MessageChannelerDummy::main(){
	hwlib::wait_ms(2'000);
	//hwlib::cout << '1';
	//time set
	RegisterControl.CommandReceived(Message(0,5));
	hwlib::wait_ms(2'000);
	//hwlib::cout << '2';
	//send start thing
	while(true){
		hwlib::wait_ms(4'000);
		RegisterControl.CommandReceived(Message(0,0));
		//hwlib::cout << '3';
		//send hit
		hitcontrol.HitReceived(Message(1,5));
	}
}