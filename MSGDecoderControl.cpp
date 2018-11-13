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
//    Records[i] = Record(m,hwlib::now_us());
//    mKnown = m;
//    if(i==16) i=0;
//    else i++;
//    return false;
}

bool MSGDecoderControl::messageKnown(uint16_t m){
  //msg
//    return mKnown == m ? 1 : 0;
  return true;
}
void MSGDecoderControl::main(){
  uint16_t n =0;
  uint16_t m =0;
  uint16_t bit = 0;
  while(true){
    auto ev = wait(DecoderTimer + PauseQueue);
    if(ev == DecoderTimer){
      n=15;
      m=0;
    }
    else if(ev == PauseQueue){
    uint16_t p = PauseQueue.read();
      if(p > 500 && p < 2000){
          bit = (p < 1100) ? 1: 0;
          m += (bit << (n));
          n--;
          if(n == 0){
            m+=1;
            n = 15;
            if(check(m)){
              auto em =  Encode.DecodeMessage(m);
              msg.SendMessage(em);
            }
            m=0;
          }else{
            DecoderTimer.set(2500);
          }
      }
    }
  } //endwhile
}
