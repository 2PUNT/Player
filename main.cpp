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

	// <<<<<<<<<< Initialization of Data >>>>>>>>>>//
	// <<<<<<<<<< All pins used >>>>>>>>>>//
	auto hwlib::target::pins irReceiverPinID;
	auto hwlib::target::pins playerDisplaySCLPinID;
	auto hwlib::target::pins playerDisplaySDAPinID;
	auto hwlib::target::pins digitLedDisplayCLKPinID;
	auto hwlib::target::pins digitLedDisplayDIOPinID;
	auto hwlib::target::pins speakerPinID;
	auto hwlib::target::d2_36kHz IrTransmitterLED;
	auto hwlib::target::pins triggerButtonPinID;
	// Keyboard:
	auto hwlib::target::pins out0ID;
	auto hwlib::target::pins out1ID;
	auto hwlib::target::pins out2ID;
	auto hwlib::target::pins out3ID;
	auto hwlib::target::pins in0ID;
	auto hwlib::target::pins in1ID;
	auto hwlib::target::pins in2ID;
	auto hwlib::target::pins in3ID;

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

	// <<<<<<<<<< IRReceiver >>>>>>>>>>//
	hwlib::target::pin_in irReceiverPin = hwlib::target::pin_in(irReceiverPinID);
	IrReceiver irReceiver = IrReceiver(irReceiverPinr);
	PauseDetectionControl pauseDetectionControl = PauseDetectionControl(irReceiver); // Geen priority??

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
}
