#include "Crc.h"

void Crc::init()
{
	_crc = Init;
}

void Crc::add(const QByteArray& data)
{
	for (quint8 byte : data)
	{
		add(byte);
	}
}

void Crc::add(quint8 byte)
{
	_crc ^= byte;
	for (int i = 0; i < 8; ++i)
	{
		if (_crc & 0x80)
		{
			_crc = (_crc << 1) ^ Poly;
		}
		else
		{
			_crc <<= 1;
		}
		_crc &= 0xFF;  // Ensure CRC remains an 8-bit value
	}
}
