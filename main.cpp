#include "hwlib.hpp"
#include "rtos.hpp"

#include "IncludeThisToIncludeAllPlayerFiles.hpp"

#include "IrReceiver.hpp"
#include "PauseDetectionControl.hpp"
#include "Button.hpp"
#include "DigitLedDisplay.hpp"
#include "DisplayControl.hpp"
#include "EncodeDecodeMSG.hpp"
#include "IrReceiver.hpp"
#include "PauseDetectionControl.hpp"
#include "IrTransmitter.hpp"
#include "SendIrMessageControl.hpp"
#include "Keyboard_4x4.hpp"
#include "Speaker.hpp"
#include "SpeakerControl.hpp"

int main(void){
	// kill the watchdog
	WDT->WDT_MR = WDT_MR_WDDIS;

	// <<<<<<<<<< Initialization of Data >>>>>>>>>>//
	// <<<<<<<<<< All pins used >>>>>>>>>>//
	hwlib::target::pins irReceiverPinID;
	hwlib::target::pins playerDisplaySCLPinID;
	hwlib::target::pins playerDisplaySDAPinID;
	hwlib::target::pins digitLedDisplayCLKPinID;
	hwlib::target::pins digitLedDisplayDIOPinID;
	hwlib::target::pins speakerPinID;
	hwlib::target::d2_36kHz IrTransmitterLED;
	hwlib::target::pins triggerButtonPinID;
	// Keyboard:
	hwlib::target::pins out0ID;
	hwlib::target::pins out1ID;
	hwlib::target::pins out2ID;
	hwlib::target::pins out3ID;
	hwlib::target::pins in0ID;
	hwlib::target::pins in1ID;
	hwlib::target::pins in2ID;
	hwlib::target::pins in3ID;

	// <<<<<<<<<< All priorities >>>>>>>>>>//
	// const unsigned int PriorityPauseDetectionControl <<< Not used, pauseDetectionControl has no priority?
	const unsigned int PrioritySpeakerControl;
	const unsigned int PrioritySendIrMessageControl
	const unsigned int PriorityTriggerButton;
	const unsigned int PriorityShootControl;
	const unsigned int PriorityUpdateGameTimeControl;
	const unsigned int PriorityProcessHitControl;
	const unsigned int PriorityRegisterGameParamsControl;
	const unsigned int PriorityMSGDecoderControl;

	// <<<<<<<<<< Sounds >>>>>>>>>>//
	note gameOverSound[] = {note( 621,  93750 ),note( 587,  93750 ),note( 621,  93750 ),note( 739,  375000 ),note( 830,  93750 ),note( 739,  93750 ),note( 698,  93750 ),note( 739,  93750 ),note( 466,  375000 ),
		note( 494,  125000 ),note( 466,  125000 ),note( 440,  125000 ),note( 466,  125000 ),note( 1396,  125000 ),note( 1242,  125000 ),note( 1174,  125000 ),note( 1242,  125000 ),note( 1396,  125000 ),note( -1,  -1 )};
	note shootSound[] = {note( 1108,  125000 ),note( -1,  -1 )};
	note hitSound[] = {note( 1108,  125000 ),note( -1,  -1 )};
	GameOverSound = gameOverSound;
	ShootSound = shootSound;
	HitSound = hitSound;
	
	// <<<<<<<<<< Other Data >>>>>>>>>>//
	uint16_t irTransmitterLow;
	uint16_t irTransmitterHigh;
	int triggerButtonID;


	// <<<<<<<<<< Creation and Initialization of classes >>>>>>>>>>//
	// <<<<<<<<<< Entities >>>>>>>>>>//
	PlayerData playerData = PlayerData();
	HitDatas hitDatas = HitDatas();
	RemainingTime remainingTime = RemainingTime();
	ShotDatas shotDatas = ShotDatas();

	// <<<<<<<<<< DisplayOLED >>>>>>>>>>//
	auto playerDisplayScl     = hwlib::target::pin_oc(  playerDisplaySCLPinID );
	auto playerDisplaySda     = hwlib::target::pin_oc(  playerDisplaySDAPinID );
	auto playerDisplayI2c_bus = hwlib::i2c_bus_bit_banged_scl_sda( playerDisplayScl, playerDisplaySda );
	hwlib::glcd_oled playerDisplay = hwlib::glcd_oled( playerDisplayI2c_bus, 0x3c ));
	DisplayControl displayControl = DisplayControl(playerDisplay);

	// <<<<<<<<<< DigitLedDisplay >>>>>>>>>>//
	hwlib::target::pin_in_out digitLedDisplayCLK = hwlib::target::pin_in_out(digitLedDisplayCLKPinID);
	hwlib::target::pin_in_out digitLedDisplayDIO = hwlib::target::pin_in_out(digitLedDisplayDIOPinID);
	DigitLedDisplay digitLedDisplay = DigitLedDisplay(digitLedDisplayCLK,digitLedDisplayDIO);

	// <<<<<<<<<< Speaker >>>>>>>>>>//
	hwlib::pin_out speakerPin = hwlib::pin_out(speakerPinID);
	Speaker speaker = Speaker(speakerPin);
	SpeakerControl speakerControl = SpeakerControl(PrioritySpeakerControl, "speakerControl", speaker);

	// <<<<<<<<<< IRSend >>>>>>>>>>//
	IrTransmitter irTransmitter = IrTransmitter(IrTransmitterLED);
	SendIrMessageControl sendIrMessageControl = SendIrMessageControl(irTransmitter, irTransmitterLow, irTransmitterHigh, PrioritySendIrMessageControl); // what are the high and low parameters?

	// <<<<<<<<<< EncodeDecodeMSG >>>>>>>>>>//
	EncodeDecodeMSG encodeDecodeMSG = EncodeDecodeMSG();

	// <<<<<<<<<< TransferData >>>>>>>>>>//
	PersonalComputer personalComputer = PersonalComputer();
	TransferDataControl transferDataControl = TransferDataControl(playerData, hitDatas, shotData, personalComputer);

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

	Keyboard_4x4<1> keyboard = Keyboard_4x4<1>(keypad, 0, "TheKeyBoard");

	// <<<<<<<<<< IRunGameTasks >>>>>>>>>>//
	ShootControl shootControl = ShootControl(PriorityShootControl, "shootControl", playerData, shotDatas,
		remainingTime, encodeDecodeMSG, displayControl, sendIrMessageControl, speakerControl, triggerButtonID);

	IButtonListener* shootControl_ButtonListener = &shootControl;
	triggerButton.SetButtonListener(shootControl_ButtonListener); // add shootControl to the listeners of the triggerButton.

	IRunGameTaskDummy irunGameTaskDummy // a tmp Dummy to fix a small circular reference we have....
	UpdateGameTimeControl updateGameTimeControl = UpdateGameTimeControl(PriorityUpdateGameTimeControl, "updateGameTimeControl", remainingTime, digitLedDisplay, shootControl, irunGameTaskDummy, speakerControl); // give a dummy for now
	ProcessHitControl processHitControl = ProcessHitControl(PriorityProcessHitControl, "processHitControl", updateGameTimeControl, shootControl);
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
	PauseDetectionControl pauseDetectionControl = PauseDetectionControl(irReceiverPinr); // Geen priority?? Geen reference naar irReceiver?? Geen reference naar msgDecoderControl?? Een pin meegeven aan een Control object? dat is iets voor een Boundary...
	// This is how it should be done:
	/*
	IrReceiver irReceiver = IrReceiver(irReceiverPinr);
	PauseDetectionControl pauseDetectionControl = PauseDetectionControl(irReceiver, msgDecoderControl);
	*/
	
	rtos::run();
	
}
