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
SOURCES :=

# header files in this project
HEADERS := Keyboard_4x4.hpp IKeyboardListener.hpp IRunGameTask.hpp MSGDecoderControl.hpp  ShootControl.hpp UpdateGametimeControl.hpp IncludeThisToIncludeAllPlayerFiles.hpp  MessageDecoder.hpp RegisterGameParamsControl.hpp ProcessHitControl.hpp Entities.hpp TransferData.hpp Keyboard_4x4.hpp ShootControl.hpp Speaker.hpp SpeakerControl.hpp SoundDefinitions.hpp IrReceiver.hpp EncodeDecodeMSG.hpp DisplayControl.hpp note.hpp IrTransmitter.hpp SendIrMessageControl.hpp PauseDetectionControl.hpp Button.hpp IButtonListener.hpp


# other places to look for files for this project
SEARCH  := lib/

# set RELATIVE to the next higher directory and defer to the Makefile.due
RELATIVE := $(RELATIVE)../
include $(RELATIVE)Makefile.due
