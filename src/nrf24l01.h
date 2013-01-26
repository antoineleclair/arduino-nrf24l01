#ifndef _WIRELESS_H_
#define _WIRELESS_H_

class Wireless {
    
    public:
        Wireless(int slaveSelectPin, int chipEnabledPin);
        void begin();
    
    private:
        int slaveSelectPin;
        int chipEnabledPin;
        
}

#endif
