#include "MSGDecoderControl.hpp"


void MSGDecoderControl::PauseDetected(int n){
  if(n > 0){
    PauseQueue.write(n);
  }
}


bool MSGDecoderControl::check(uint16_t m){
  if(((m >> 11) & 0x001F) == (((m >> 6) & 0x001F) ^ ((m >> 1) & 0x001F))){
      return true;
  }else{
      return false;
  }
}

void MSGDecoderControl::knownMessage(uint16_t m){
    Record r;
    r.message = m;
    r.time = hwlib::now_us();
    Records.addRecord(r);
}

bool MSGDecoderControl::messageKnown(uint16_t m){
  return Records.checkKnown(m);
}
void MSGDecoderControl::main(){
  uint16_t n =0;
  uint16_t m =0;
  uint16_t bit = 0;
  while(true){
    auto ev = wait(DecoderTimer + PauseQueue);
    if(ev == DecoderTimer){
	  //hwlib::cout << "message incomplete: N: " << n << " m: " << hwlib::bin << m << hwlib::dec <<'\n';
      n=15;
      m=0;
    }
    else if(ev == PauseQueue){
    uint16_t p = PauseQueue.read();
      if(p > 400 && p < 2500){
          bit = (p < 1100) ? 1: 0;
          m += (bit << (n));
          n--;
          if(n == 0){
			//hwlib::cout << "MSGDecoderControl: Message received, checking...\n";
            m+=1;
            n = 15;
            if(check(m)){
				      //hwlib::cout << "MSGDecoderControl: Check returned true\n";
              //if(! messageKnown(m)){
                auto em =  Encode.DecodeMessage(m);
                Channeler.SendMessage(em);
                //knownMessage(m);
              //}
            }//else hwlib::cout << "MSGDecoderControl: Check returned false!\n";
            m=0;
          }else{
            DecoderTimer.set(2500);
          }
      }
    }
  } //endwhile
}
