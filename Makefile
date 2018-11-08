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
SOURCES := DigitLedDisplay.cpp hwlib_TM1637.cpp ProcessHitControl.cpp  Entities.cpp  UpdateGametimeControl.cpp  RegisterGameParamsControl.cpp  IRunGameTaskDummy.hpp MSGDecoderControl.cpp  ShootControl.cpp

# header files in this project
HEADERS := IRunGameTask.hpp MSGDecoderControl.hpp  ShootControl.hpp UpdateGametimeControl.hpp IncludeThisToIncludeAllPlayerFiles.hpp  MessageDecoder.hpp RegisterGameParamsControl.hpp ProcessHitControl.hpp Entities.hpp TransferData.hpp

# other places to look for files for this project
SEARCH  := lib/

# set RELATIVE to the next higher directory and defer to the Makefile.due
RELATIVE := $(RELATIVE)../
include $(RELATIVE)Makefile.due
