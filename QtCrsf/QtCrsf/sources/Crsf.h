#pragma once

#include <QObject>

#include "Crc.h"

class Crsf : public QObject
{
	Q_OBJECT
public:
	enum class State
	{
		Start,
		Length,
		Type,
		Payload,
		Crc
	};

	enum class Address
	{
		Broadcast		 = 0,
		USB				 = 0x10,
		Bluetooth		 = 0x12,
		TbsCorePrpPro	 = 0x90,
		Reserved1		 = 0x8A,
		CurrentSensor	 = 0xC0,
		Gps				 = 0xC2,
		TbsBlackBox		 = 0xC4,
		FlightController = 0xC8,
		Reserved2		 = 0xCA,
		RaceTag			 = 0xCC,
		RadioTransmitter = 0xEA,
		CrsfReceived	 = 0xEC,
		CrsfTrasmitter	 = 0xEE,
		ElsrLua			 = 0xEf
	};

	enum class FrameType
	{
		Invalid				   = 0,
		Gps					   = 0x02,
		Vario				   = 0x07,
		BatterySensor		   = 0x08,
		baroAltitude		   = 0x09,
		Heartbeat			   = 0x0B,
		LinkStatistics		   = 0x14,
		RcChannelPacked		   = 0x16,
		SubsetRcChannelPacked  = 0x17,
		LinkRxId			   = 0x1C,
		LinkTxId			   = 0x1D,
		Attitude			   = 0x1E,
		FlightMode			   = 0x21,
		devicePing			   = 0x28,
		DeviceInfo			   = 0x29,
		ParametersSettingEntry = 0x2B,
		parameterRead		   = 0x2C,
		ParameterWrite		   = 0x2D,
		ElrsStatus			   = 0x2E,
		Command				   = 0x32,
		RadioId				   = 0x3A,
		Kissreq				   = 0x78,
		KissResp			   = 0x79,
		MspReq				   = 0x7A,
		MspResp				   = 0x7B,
		MspWrite			   = 0x7C,
		DisplayPortCmd		   = 0x7D,
		ArdupilotResp		   = 0x80
	};

	typedef struct
	{
		qint32 latitude;
		qint32 longitude;
		qint16 groundSpeed;
		qint16 groundCourse;
		quint16 altitude;
		quint8 satelliteCount;
	} Gps;

	typedef struct
	{
		qint16 voltage;
		qint16 current;
		quint32 capacity;
		quint8 percent;
	} BatterySensor;

	typedef struct
	{
		qint16 pitch;
		qint16 roll;
		qint16 yaw;
	} Attitude;

	typedef struct
	{
		quint8 Rssi1;
		quint8 Rssi2;
		quint8 UplinkSuccessRate;
		qint8 UplinkSNR;
		quint8 diversityActiveAntenna;
		quint8 RfMode;
		quint8 UplinkTxPower;
		quint8 DownlinkRssi;
		quint8 DownlinkSuccessRate;
		qint8 DownlinkSNR;
	} LinkStatistics;

	typedef struct
	{
		quint8 subtype;
		qint32 packetInterval;
		qint32 phaseShiftCorrection;
	} RadioId;

	using QObject::QObject;

	void byteReceived(quint8 bytes);
	void bytesReceived(const QByteArray &data);

	void parse();
	void printMessage();

	quint32 received() const
	{
		return _received;
	}

	quint32 error() const
	{
		return _error;
	}

protected:
	bool isValidAddress(quint8 data);
	bool isValidFrameType(quint8 data);
	void setState(State state)
	{
		_state = state;
	}

signals:
	void gpsReceived(const Gps &gps);
	void linkStatisticsReceived(const LinkStatistics &linkStatistics);
	void attitudeReceived(const Attitude &attitude);
	void batteryReceived(const BatterySensor &battery);

private:
	State _state = State::Start;
	Address _address;
	quint8 _length;
	FrameType _frameType;
	QByteArray _payload;
	quint8 _crc;

	Crc _crcCalculator;
	quint32 _error	  = 0;
	quint32 _received = 0;
};
