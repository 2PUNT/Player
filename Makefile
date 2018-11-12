#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
#
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := hwlib_TM1637.cpp Entities.cpp EncodeDecodeMSG.cpp IrReceiver.cpp InitGameControl.cpp DisplayControl.cpp MSGDecoderControl.cpp ProcessHitControl.cpp RegisterGameParamsControl.cpp ShootControl.cpp UpdateGametimeControl.cpp Speaker.cpp SpeakerControl.cpp DigitLedDisplay.cpp

# header files in this project
HEADERS := hwlib_TM1637.hpp ADTs.hpp IRunGameTaskDummy.hpp EncodeDecodeMSG.hpp IrReceiver.hpp PauseDetectionControl.hpp IrTransmitter.hpp SendIrMessageControl.hpp Keyboard_4x4.hpp Speaker.hpp SpeakerControl.hpp DisplayControl.hpp TransferData.hpp Entities.hpp ShootControl.hpp IRunGameTask.hpp IRunGameTaskDummy.hpp MSGDecoderControl.hpp RegisterGameParamsControl.hpp ProcessHitControl.hpp UpdateGameTimeControl.hpp DigitLedDisplay.hpp note.hpp


# other places to look for files for this project
SEARCH  := ../Player ../Button/lib ../DigitLedDisplay/lib ../DisplayOLED/lib ../EncodeDecodeMSG/lib ../GameMaster ../IRReceiver/lib ../IRSend/lib ../Keyboard_4x4/lib ../Speaker/lib

# set RELATIVE to the next higher directory and defer to the Makefile.due
RELATIVE := $(RELATIVE)../
include $(RELATIVE)Makefile.due
