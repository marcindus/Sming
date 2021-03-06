/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * FtpServerConnection.h
 *
 ****/

#pragma once

#include "Network/TcpConnection.h"
#include "IpAddress.h"
#include "WString.h"

/**
 * @defgroup ftp FTP
 * @ingroup  tcp
 * @{
 */

class FtpServer;

class FtpServerConnection : public TcpConnection
{
	friend class FtpDataStream;
	friend class FtpServer;

public:
	static constexpr size_t MAX_FTP_CMD{255};

	FtpServerConnection(FtpServer& parentServer, tcp_pcb* clientTcp)
		: TcpConnection(clientTcp, true), server(parentServer)
	{
	}

	err_t onReceive(pbuf* buf) override;
	err_t onSent(uint16_t len) override;
	void onReadyToSendData(TcpConnectionEvent sourceEvent) override;

	void dataTransferFinished(TcpConnection* connection);

protected:
	virtual void onCommand(String cmd, String data);
	virtual void response(int code, String text = "");

	void cmdPort(const String& data);
	void createDataConnection(TcpConnection* connection);

	bool isCanTransfer()
	{
		return canTransfer;
	}

private:
	enum class State {
		Ready,
		Authorization,
		Active,
	};

	FtpServer& server;
	State state{State::Ready};
	String userName;
	String renameFrom;

	IpAddress ip;
	int port{0};
	TcpConnection* dataConnection{nullptr};
	bool canTransfer{true};
};

typedef FtpServerConnection FTPServerConnection SMING_DEPRECATED; // @deprecated Use `FtpServerConnection` instead

/** @} */
