#pragma once

#include <QObject>

class Crc
{
public:
	void init();
	void add(const QByteArray& data);
	void add(quint8 byte);
	quint8 crc() const
	{
		return _crc;
	}

private:
	static constexpr quint8 Init = 0x00;
	static constexpr quint8 Poly = 0xD5;

	quint8 _crc;
};
