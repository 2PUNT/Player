#include "MSGDecoderControl.hpp"

void MSGDecoderControl::PauseDetected(int n){
    PauseQueue.write(n);
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
    
}

bool MSGDecoderControl::messageKnown(uint16_t m){
    //msg 
//    return mKnown == m ? 1 : 0;
    return true;
}

void MSGDecoderControl::main(){
    switch(STATE){
        case state_t::WAIT_FOR_PAUSE:
        auto event = PauseQueue + DecoderTimer;
            if(event == PauseQueue){
                p = PauseQueue.read();
                if(p>600 && p<1800){
                    n++;
                    m <<= n-1;
                    m |= (p<1000) ? 1 : 0;
                    if(n==15){
                        m <<= n;
                        m |= 1;
                        if(check(m)){
                            if(messageKnown(m)){
                                n = 0;
                                m = 0;
                                STATE = state_t::WAIT_FOR_PAUSE;
                            }else{
                                Message em = Encode.DecodeMessage(m);
                                Channeler.SendMessage(em);
                                knownMessage(m);
                            }
                        }else{
                            n = 0;
                            m = 0;
                            STATE = state_t::WAIT_FOR_PAUSE;
                        }
                    }else{
                        DecoderTimer.set(2500);
                        STATE = state_t::WAIT_FOR_PAUSE;
                    }
                }else{
                    STATE = state_t::WAIT_FOR_PAUSE;
                }
            }else if(event == DecoderTimer){
                n = 0;
                m = 0;
                STATE = state_t::WAIT_FOR_PAUSE;
            }
            break;
    }
}