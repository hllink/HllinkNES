#ifndef MAPPER007_H
#define MAPPER007_H

#include "IMapper.h"

//NROM
class Mapper007 : public IMapper
{
private:

    uint8_t m_PRGReg = 0;
    uint8_t m_PRGRegMask = 0;

    bool m_mirroring = false;

public:

    Mapper007(ICartridgeData& cd) : IMapper(cd)
    {
        m_PRGRegMask = calculateMask(m_cartridgeData.numberOfPRGBanks<W32K>());    
    }

    GERANES_HOT void writePRG32k(int /*addr*/, uint8_t data) override
    {
        m_mirroring = data&0x10;
        m_PRGReg = data&m_PRGRegMask;
    }

    GERANES_INLINE_HOT uint8_t readPRG32k(int addr) override
    {
        return m_cartridgeData.readPRG<W32K>(m_PRGReg,addr);
    }

    GERANES_INLINE_HOT IMapper::MirroringType mirroringType(void) override
    {
        if(!m_mirroring) return IMapper::SINGLE_SCREEN_A;
        return IMapper::SINGLE_SCREEN_B;
    }

    void serialization(SerializationBase& s) override
    {
        IMapper::serialization(s);

        SERIALIZEDATA(s, m_PRGReg);
        SERIALIZEDATA(s, m_PRGRegMask);
        SERIALIZEDATA(s, m_mirroring);
    }

};

#endif // MAPPER007_H
