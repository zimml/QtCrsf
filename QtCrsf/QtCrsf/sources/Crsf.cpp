#include "Crsf.h"

#include <QDebug>
#include <QtMath>

#include "Crc.h"

void Crsf::parse()
{
	switch (_frameType)
	{
	case FrameType::Gps:
	{
		QDataStream stream(_payload);
		Gps gps;
		stream >> gps.latitude;
		stream >> gps.longitude;
		stream >> gps.groundSpeed;
		stream >> gps.groundCourse;
		stream >> gps.altitude;
		stream >> gps.satelliteCount;

		emit gpsReceived(gps);
	}
	break;
	case FrameType::Attitude:
	{
		QDataStream stream(_payload);
		Attitude attitude;
		stream >> attitude.pitch;
		stream >> attitude.roll;
		stream >> attitude.yaw;

		emit attitudeReceived(attitude);
	}
	break;
	case FrameType::LinkStatistics:
	{
		QDataStream stream(_payload);
		LinkStatistics link;
		stream >> link.Rssi1;
		stream >> link.Rssi2;
		stream >> link.UplinkSuccessRate;
		stream >> link.UplinkSNR;
		stream >> link.diversityActiveAntenna;
		stream >> link.RfMode;
		stream >> link.UplinkTxPower;
		stream >> link.DownlinkRssi;
		stream >> link.DownlinkSuccessRate;
		stream >> link.DownlinkSNR;

		emit linkStatisticsReceived(link);
	}
	break;
	case FrameType::RadioId:
	{
		QDataStream stream(_payload);
		RadioId radio;
		stream >> radio.subtype;
		stream >> radio.data;

		qDebug() << "Receied radio ";
		qDebug() << "Subtype: " << radio.subtype;
		qDebug() << "Data: " << radio.data.toHex();
	}
	break;
	case FrameType::FlightMode:
	{
		qDebug() << "Receied flight mode ";
		qDebug() << "Mode: " << _payload;
	}
	break;
	case FrameType::BatterySensor:
	{
		QDataStream stream(_payload);
		BatterySensor battery;
		stream >> battery.voltage;
		stream >> battery.current;
		quint8 capacity;
		stream >> capacity;
		battery.capacity = capacity;
		stream >> capacity;
		battery.capacity = (battery.capacity << 8) | capacity;
		stream >> capacity;
		battery.capacity = (battery.capacity << 8) | capacity;
		stream >> battery.percent;

		qDebug() << "Receied battery ";
		qDebug() << "Voltage: " << static_cast<double>(battery.voltage) / 10.
				 << " V";
		qDebug() << "Current: " << static_cast<double>(battery.current) / 10.
				 << " A";
		qDebug() << "Capacity: " << battery.capacity << " mAh";
		qDebug() << "Percent: " << battery.percent << " %";
	}
	break;
	default:
		break;
	}
}

void Crsf::byteReceived(quint8 byte)
{
	switch (_state)
	{
	case State::Start:
		if (isValidAddress(byte))
		{
			_address   = static_cast<Address>(byte);
			_frameType = FrameType::Invalid;
			_payload.clear();
			setState(State::Length);
		}
		else
		{
			qDebug() << "START ERROR " << QString::number(byte, 16).toUpper();

			_error++;
		}
		break;
	case State::Length:
		_length = byte;
		if (_length > 1)
			setState(State::Type);
		else
			setState(State::Crc);

		_crcCalculator.init();
		break;
	case State::Type:
		_crcCalculator.add(byte);
		if (isValidFrameType(byte))
		{
			_frameType = static_cast<FrameType>(byte);
			if (_length > 2)
				setState(State::Payload);
			else
				setState(State::Crc);
			_payload.clear();
		}
		else
		{
			_error++;
			qDebug()
				<< "TYPE ERROR " << QString::number(byte, 16).toUpper()
				<< " len " << _length << " from "
				<< QString::number(static_cast<int>(_address), 16).toUpper();

			setState(State::Start);
		}
		break;
	case State::Payload:
		_payload.append(byte);
		_crcCalculator.add(byte);
		if (_payload.size() >= _length - 2)
		{
			setState(State::Crc);
		}
		break;
	case State::Crc:
		_crc = byte;
		if (_crc == _crcCalculator.crc())
		{
			_received++;
			parse();
		}
		else
		{
			qDebug() << "CRC ERROR" << QString::number(_crc, 16).toUpper()
					 << " "
					 << QString::number(_crcCalculator.crc(), 16).toUpper();

			_error++;
			parse();
		}
		setState(State::Start);
		break;
	}
}

void Crsf::bytesReceived(const QByteArray &data)
{
	for (quint8 byte : data)
	{
		byteReceived(byte);
	}
}
bool Crsf::isValidAddress(quint8 data)
{
	switch (static_cast<Address>(data))
	{
	case Address::Broadcast:
	case Address::USB:
	case Address::Bluetooth:
	case Address::TbsCorePrpPro:
	case Address::Reserved1:
	case Address::CurrentSensor:
	case Address::Gps:
	case Address::TbsBlackBox:
	case Address::FlightController:
	case Address::Reserved2:
	case Address::RaceTag:
	case Address::RadioTransmitter:
	case Address::CrsfReceived:
	case Address::CrsfTrasmitter:
	case Address::ElsrLua:
		return true;
	}
	return false;
}

bool Crsf::isValidFrameType(quint8 data)
{
	switch (static_cast<FrameType>(data))
	{
	case FrameType::Invalid:
		return false;
	case FrameType::Gps:
	case FrameType::Vario:
	case FrameType::BatterySensor:
	case FrameType::baroAltitude:
	case FrameType::Heartbeat:
	case FrameType::LinkStatistics:
	case FrameType::RcChannelPacked:
	case FrameType::SubsetRcChannelPacked:
	case FrameType::LinkRxId:
	case FrameType::LinkTxId:
	case FrameType::Attitude:
	case FrameType::FlightMode:
	case FrameType::devicePing:
	case FrameType::DeviceInfo:
	case FrameType::ParametersSettingEntry:
	case FrameType::parameterRead:
	case FrameType::ParameterWrite:
	case FrameType::ElrsStatus:
	case FrameType::Command:
	case FrameType::RadioId:
	case FrameType::Kissreq:
	case FrameType::KissResp:
	case FrameType::MspReq:
	case FrameType::MspResp:
	case FrameType::MspWrite:
	case FrameType::DisplayPortCmd:
	case FrameType::ArdupilotResp:
		return true;
	}
	return false;
}
