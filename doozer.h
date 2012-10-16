/*-
 * Copyright (c) 2012 Tonnerre Lombard <tonnerre@ancient-solutions.com>,
 *                    Ancient Solutions. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions  of source code must retain  the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions  in   binary  form  must   reproduce  the  above
 *    copyright  notice, this  list  of conditions  and the  following
 *    disclaimer in the  documentation and/or other materials provided
 *    with the distribution.
 *
 * THIS  SOFTWARE IS  PROVIDED BY  ANCIENT SOLUTIONS  AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO,  THE IMPLIED WARRANTIES OF  MERCHANTABILITY AND FITNESS
 * FOR A  PARTICULAR PURPOSE  ARE DISCLAIMED.  IN  NO EVENT  SHALL THE
 * FOUNDATION  OR CONTRIBUTORS  BE  LIABLE FOR  ANY DIRECT,  INDIRECT,
 * INCIDENTAL,   SPECIAL,    EXEMPLARY,   OR   CONSEQUENTIAL   DAMAGES
 * (INCLUDING, BUT NOT LIMITED  TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE,  DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT  LIABILITY,  OR  TORT  (INCLUDING NEGLIGENCE  OR  OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef DOOZER_DOOZER_H
#define DOOZER_DOOZER_H 1

#define	DOOZER_URI_PREFIX	"doozer:?"

struct QTcpSocket;

namespace google {
namespace protobuf {
class Message;
}  // namespace protobuf
}  // namespace google

namespace doozer {

class Transaction;

const QString doozer_uri_prefix = QString(DOOZER_URI_PREFIX);

// Basic Doozer error type
class Error {
public:
	// Constructs a new generic Doozer error with the given "message".
	Error(std::string message);
	Error(QString message);

	// Returns a string describing the error which ocurred.
	std::string ToString();

	// Same, but returns a Qt compatible string.
	QString ToQString();

protected:
	// Contained error message.
	QString message_;
};

// Doozer connection type.
class Conn {
public:
	// Various methods of connecting.
	Conn(std::string addr);
	Conn(std::string uri, std::string boot_uri);
	Conn(QString addr);
	Conn(QString uri, QString boot_uri);

	// Disconnect and dispose of the connection.
	virtual ~Conn();

	// Whether or not the connection was established successfully.
	virtual bool IsValid();

	// The error which ocurred when establishing the connection
	virtual Error* GetError();

	// Attempts to gain access using the given "token".
	virtual Error* Access(std::string token);
	virtual Error* Access(QString token);

	// Set the contents of "file". This operation is atomic, so you have
	// to set the whole "body" at a time. "oldRev" must be set to the
	// current revision of the file, and "newRev" will be set to the
	// revision number created by the mutation.
	virtual Error* Set(std::string file, int64_t oldRev, int64_t* newRev,
			char *body, size_t len);
	virtual Error* Set(QString file, int64_t oldRev, int64_t* newRev,
			QByteArray body);

	// Delete the given "file" at the given revision "rev".
	virtual Error* Del(std::string file, int64_t rev);
	virtual Error* Del(QString file, int64_t rev);

	// Does absolutely nothing, but this can be used to check the server
	// connectivity.
	virtual Error* Nop();

	// Gets the content of the given "file" at revision "storerev".
	// Stores the contents into "buf" and the revision into "filerev".
	// If "rev" is NULL, the latest revision will be returned.
	virtual Error* Get(std::string file, int64_t* storerev,
			std::string* buf, int64_t* filerev);
	virtual Error* Get(QString file, int64_t* storerev, QByteArray* buf,
			int64_t* filerev);

	// Returns stats about the given "path" at or before version
	// "storerev". If "storerev" is NULL, returns the latest version.
	// Stores the result into "len" and "filerev".
	virtual Error* Stat(std::string path, int64_t* storerev,
			int* len, int64_t* filerev);
	virtual Error* Stat(QString path, int64_t* storerev,
			int* len, int64_t* filerev);

	// Returns the current revision of the store in "rev".
	virtual Error* Rev(int64_t* rev);

	// TODO(tonnerre): Port the more complex functions.

private:
	void init(QString uri, QString buri);
	Error* send(const ::google::protobuf::Message& msg);
	Error* recv(::google::protobuf::Message* msg);

	// Error which may have occured during initialization
	Error* error_;
	bool valid_;

	// Connection to the Doozer service.
	QTcpSocket* conn_;
};

}  // namespace doozer

#endif /* DOOZER_DOOZER_H */