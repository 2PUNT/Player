#include "hwlib.hpp"
#include "rtos.hpp"

#include "Entities.hpp"

#include "Button.hpp"
#include "UpdateGametimeControl.hpp"
#include "ProcessHitControl.hpp"
#include "ShootControl.hpp"
#include "DigitLedDisplay.hpp"
#include "DisplayControl.hpp"
#include "EncodeDecodeMSG.hpp"
#include "IrReceiver.hpp"
#include "IrTransmitter.hpp"
#include "SendIrMessageControl.hpp"
#include "Keyboard_4x4.hpp"
#include "Speaker.hpp"
#include "SpeakerControl.hpp"
#include "TransferData.hpp"
#include "IRunGameTask.hpp"
#include "IRunGameTaskDummy.hpp"
#include "MSGDecoderControl.hpp"
#include "PauseDetectionControl.hpp"

#include "RegisterGameParamsControl.hpp"

int main(void){
	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;
	 namespace target = hwlib::target;
	// <<<<<<<<<< Initialization of Data >>>>>>>>>>//
	// <<<<<<<<<< All pins used >>>>>>>>>>//
	hwlib::target::pins irReceiverPinID =  hwlib::target::pins::a0;
	hwlib::target::pins playerDisplaySCLPinID =  hwlib::target::pins::a0;
	hwlib::target::pins playerDisplaySDAPinID =  hwlib::target::pins::a0;
	hwlib::target::pins digitLedDisplayCLKPinID =  hwlib::target::pins::a0;
	hwlib::target::pins digitLedDisplayDIOPinID =  hwlib::target::pins::a0;
	hwlib::target::d2_36kHz IrTransmitterLED = hwlib::target::d2_36kHz();
	hwlib::target::pins triggerButtonPinID=  hwlib::target::pins::a0;
	hwlib::target::pins speakerPinID = hwlib::target::pins::a0;


	// Keyboard:
	hwlib::target::pins out0ID =   hwlib::target::pins::a0 ;
	hwlib::target::pins out1ID =   hwlib::target::pins::a1 ;
	hwlib::target::pins out2ID =   hwlib::target::pins::a2 ;
	hwlib::target::pins out3ID =   hwlib::target::pins::a3 ;
	hwlib::target::pins in0ID  =   hwlib::target::pins::a4 ;
	hwlib::target::pins in1ID  =   hwlib::target::pins::a5 ;
	hwlib::target::pins in2ID  =   hwlib::target::pins::a6 ;
	hwlib::target::pins in3ID  =   hwlib::target::pins::a7 ;


	// <<<<<<<<<< All priorities >>>>>>>>>>//
	const unsigned int PriorityPauseDetectionControl	= 1;
	const unsigned int Prioritykeyboard					= 2;
	const unsigned int PrioritySpeakerControl 			= 9;
	const unsigned int PrioritySendIrMessageControl 	= 4;
	const unsigned int PriorityTriggerButton			= 3;
	const unsigned int PriorityShootControl 			= 5;
	const unsigned int PriorityUpdateGameTimeControl	= 11;
	const unsigned int PriorityProcessHitControl		= 10;
	const unsigned int PriorityRegisterGameParamsControl= 7;
	const unsigned int PriorityMSGDecoderControl    	= 6;

	// <<<<<<<<<< Sounds >>>>>>>>>>//
	note gameOverSound[] = {note( 621,  93750 ),note( 587,  93750 ),note( 621,  93750 ),note( 739,  375000 ),note( 830,  93750 ),note( 739,  93750 ),note( 698,  93750 ),note( 739,  93750 ),note( 466,  375000 ),
		note( 494,  125000 ),note( 466,  125000 ),note( 440,  125000 ),note( 466,  125000 ),note( 1396,  125000 ),note( 1242,  125000 ),note( 1174,  125000 ),note( 1242,  125000 ),note( 1396,  125000 ),note( -1,  -1 )};
	note shootSound[] = {note( 1108,  125000 ),note( -1,  -1 )};
	note hitSound[] = {note( 1108,  125000 ),note( -1,  -1 )};
	GameOverSound = gameOverSound;
	ShootSound = shootSound;
	HitSound = hitSound;

	// <<<<<<<<<< Other Data >>>>>>>>>>//
	int defaultPlayerHealth = 100;
	uint16_t irTransmitterLow = 1600;
	uint16_t irTransmitterHigh = 1800;
	int triggerButtonID = 1;


	// <<<<<<<<<< Creation and Initialization of classes >>>>>>>>>>//
	// <<<<<<<<<< Entities >>>>>>>>>>//
	PlayerData playerData = PlayerData();
	playerData.SetHealth(defaultPlayerHealth);
	HitDatas hitDatas = HitDatas();
	RemainingTime remainingTime = RemainingTime();
	ShotDatas shotDatas = ShotDatas();

	// <<<<<<<<<< DisplayOLED >>>>>>>>>>//
	auto playerDisplayScl     = hwlib::target::pin_oc(  playerDisplaySCLPinID );
	auto playerDisplaySda     = hwlib::target::pin_oc(  playerDisplaySDAPinID );
	auto playerDisplayI2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( playerDisplayScl, playerDisplaySda );
	hwlib::glcd_oled playerDisplay = hwlib::glcd_oled( playerDisplayI2c_bus, 0x3c );

	auto w1 = hwlib::window_part(
		playerDisplay,
		hwlib::location( 0, 0 ),
		hwlib::location( 128, 32));

	auto w2 = hwlib::window_part(
		playerDisplay,
		hwlib::location( 0, 32 ),
		hwlib::location( 128, 32));

	auto f1 = hwlib::font_default_8x8();
	auto d1 = hwlib::window_ostream( w1, f1 );

	auto f2 = hwlib::font_default_8x8();
	auto d2 = hwlib::window_ostream( w2, f2 );

	auto ose = hwlib::window_part(
		playerDisplay, hwlib::location( 0, 0 ),
	hwlib::location( 128, 32));

	DisplayControl displayControl = DisplayControl(playerDisplay,w1, w2, d1, d2, 100);

	// <<<<<<<<<< DigitLedDisplay >>>>>>>>>>//
	hwlib::target::pin_in_out digitLedDisplayCLK = hwlib::target::pin_in_out(digitLedDisplayCLKPinID);
	hwlib::target::pin_in_out digitLedDisplayDIO = hwlib::target::pin_in_out(digitLedDisplayDIOPinID);
	DigitLedDisplay digitLedDisplay = DigitLedDisplay(digitLedDisplayCLK,digitLedDisplayDIO);

	// <<<<<<<<<< Speaker >>>>>>>>>>//
	hwlib::target::pin_out speakerPin = hwlib::target::pin_out( speakerPinID );
	Speaker speaker = Speaker(speakerPin);
	SpeakerControl speakerControl = SpeakerControl(PrioritySpeakerControl, "speakerControl", speaker);


	// <<<<<<<<<< IRSend >>>>>>>>>>//
	IrTransmitter irTransmitter = IrTransmitter(IrTransmitterLED);
	SendIrMessageControl sendIrMessageControl = SendIrMessageControl(irTransmitter, irTransmitterLow, irTransmitterHigh, PrioritySendIrMessageControl); // what are the high and low parameters?

	// <<<<<<<<<< EncodeDecodeMSG >>>>>>>>>>//
	EncodeDecodeMSG encodeDecodeMSG = EncodeDecodeMSG();

	// <<<<<<<<<< TransferData >>>>>>>>>>//
	PersonalComputer personalComputer = PersonalComputer();
	TransferDataControl tdc(playerData, hitDatas, shotDatas, personalComputer);

	// <<<<<<<<<< Buttons >>>>>>>>>>//
	hwlib::target::pin_in triggerButtonPin = hwlib::target::pin_in(triggerButtonPinID);
	Button<1> triggerButton = Button<1>(triggerButtonPin, triggerButtonID, PriorityTriggerButton, "triggerButton");

	// <<<<<<<<<< Keyboard_4x4 >>>>>>>>>>//
	hwlib::target::pin_oc out0 = hwlib::target::pin_oc( out0ID );
	hwlib::target::pin_oc out1 = hwlib::target::pin_oc( out1ID );
	hwlib::target::pin_oc out2 = hwlib::target::pin_oc( out2ID );
	hwlib::target::pin_oc out3 = hwlib::target::pin_oc( out3ID );

	hwlib::target::pin_in in0  = hwlib::target::pin_in( in0ID );
	hwlib::target::pin_in in1  = hwlib::target::pin_in( in1ID );
	hwlib::target::pin_in in2  = hwlib::target::pin_in( in2ID );
	hwlib::target::pin_in in3  = hwlib::target::pin_in( in3ID );

	hwlib::port_oc_from_pins out_port = hwlib::port_oc_from_pins( out0, out1, out2, out3 );
	hwlib::port_in_from_pins in_port  = hwlib::port_in_from_pins( in0,  in1,  in2,  in3  );
	hwlib::matrix_of_switches matrix   = hwlib::matrix_of_switches( out_port, in_port );
	hwlib::keypad< 16 > keypad   = hwlib::keypad< 16 >( matrix, "123A456B789C*0#D" );

	Keyboard_4x4<1> keyboard = Keyboard_4x4<1>(keypad, Prioritykeyboard, "TheKeyBoard");

	// <<<<<<<<<< IRunGameTasks >>>>>>>>>>//
	ShootControl shootControl = ShootControl(PriorityShootControl, "shootControl", playerData, shotDatas,
		remainingTime, encodeDecodeMSG, displayControl, sendIrMessageControl, speakerControl, triggerButtonID);

	IButtonListener* shootControl_ButtonListener = &shootControl;
	triggerButton.SetButtonListener(shootControl_ButtonListener); // add shootControl to the listeners of the triggerButton.


	IRunGameTaskDummy irunGameTaskDummy; // a tmp Dummy to fix a small circular reference we have....
	UpdateGameTimeControl updateGameTimeControl = UpdateGameTimeControl(PriorityUpdateGameTimeControl, "updateGameTimeControl", remainingTime, digitLedDisplay, shootControl, irunGameTaskDummy, speakerControl); // give a dummy for now
	ProcessHitControl processHitControl = ProcessHitControl(PriorityProcessHitControl, "processHitControl", remainingTime, hitDatas, playerData, updateGameTimeControl, shootControl);

	updateGameTimeControl.SetProcessHitControl(processHitControl); // replace the dummy with the actual processHitControl and the circular reference is fixed.



	// <<<<<<<<<< RegisterGameParamsControl >>>>>>>>>>//
	RegisterGameParamsControl registerGameParamsControl = RegisterGameParamsControl(PriorityRegisterGameParamsControl, "registerGameParamsControl", remainingTime, processHitControl,
		updateGameTimeControl, shootControl, playerData);


	IKeyboardListener* registerGameParamsControl_KeyboardListener = &registerGameParamsControl;
	keyboard.setKeyboardListener(registerGameParamsControl_KeyboardListener); // add registerGameParamsControl to the listeners of the keyboard.

	// <<<<<<<<<< MSGDecoderControl >>>>>>>>>>//
	MessageChanneler messageChanneler = MessageChanneler(registerGameParamsControl, processHitControl);

	MSGDecoderControl msgDecoderControl = MSGDecoderControl(PriorityMSGDecoderControl, "msgDecoderControl", messageChanneler);

	// <<<<<<<<<< IRReceiver >>>>>>>>>>//
	hwlib::target::pin_in irReceiverPin = hwlib::target::pin_in(irReceiverPinID);
	IrReceiver irReceiver = IrReceiver(irReceiverPin);
	PauseDetectionControl pauseDetectionControl = PauseDetectionControl(PriorityPauseDetectionControl, "pauseDetectionControl", irReceiver, msgDecoderControl);

	rtos::run();

}
