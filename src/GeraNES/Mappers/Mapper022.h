#ifndef MAPPER022_H
#define MAPPER022_H

#include "Mapper021.h"

//TODO: data eeprom

//VRC2a
class Mapper022 : public Mapper021
{

private:

public:

    Mapper022(ICartridgeData& cd) : Mapper021(cd)
    {

    }

    void writePRG32k(int addr, uint8_t data) override
    {
        //A1 A0 - $x000, $x002, $x001, $x003

        addr = (addr&0xF000) | ((addr&0x0001)<<1) | ((addr&0x0002)>>1);
        writeVRCxx(addr,data,false);

        m_PRGMode = false; // VRC2 has 1 mode only
    }

    uint8_t readCHR8k(int addr) override
    {
        if(has8kVRAM()) return IMapper::readCHR8k(addr);

        size_t index = addr >> 10;
        uint8_t bank = m_CHRReg[index];

        bank >>= 1; //VRC2a only

        return m_cartridgeData.readCHR<W1K>(bank&m_CHRREGMask,addr); // addr/1024
    }

    bool getInterruptFlag(void) override
    {
        return false;
    }

};

#endif
