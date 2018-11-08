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
SOURCES := Entities.cpp

# header files in this project
HEADERS :=  DigitLedDisplay.hpp EncodeDecodeMSG.hpp IrReceiver.hpp PauseDetectionControl.hpp IrTransmitter.hpp SendIrMessageControl.hpp Keyboard_4x4.hpp Speaker.hpp SpeakerControl.hpp DisplayControl.hpp TransferData.hpp Entities.hpp ShootControl.hpp IRunGameTask.hpp IRunGameTaskDummy.hpp MSGDecoderControl.hpp RegisterGameParamsControl.hpp ProcessHitControl.hpp UpdateGameTimeControl.hpp






# other places to look for files for this project
SEARCH  :=

# set RELATIVE to the next higher directory and defer to the Makefile.due
RELATIVE := $(RELATIVE)
include $(RELATIVE)Makefile.due
