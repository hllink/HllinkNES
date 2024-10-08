#ifndef INCLUDE_MAPPER002
#define INCLUDE_MAPPER002

#include "IMapper.h"

//UxROM
class Mapper002 : public IMapper
{
private:

    int m_selectedBank = 0;

public:

    Mapper002(ICartridgeData& cd) : IMapper(cd)
    {
    }

    GERANES_INLINE_HOT uint8_t readPRG32k(int addr) override
    {
        if(addr < 0x4000) return m_cartridgeData.readPRG<W16K>(m_selectedBank,addr);
        return m_cartridgeData.readPRG<W16K>(m_cartridgeData.numberOfPRGBanks<W16K>()-1,addr);
    }

    GERANES_INLINE_HOT void writePRG32k(int /*addr*/, uint8_t data) override
    {
        m_selectedBank = data;
    };

    void serialization(SerializationBase& s) override
    {
        IMapper::serialization(s);
        SERIALIZEDATA(s, m_selectedBank);
    }

};

#endif
