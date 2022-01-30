// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <iostream>
#include "DataPageAccess.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

#include "postgres.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

#include "access/xlog.h"
#include "access/xlogutils.h"
#include "commands/tablespace.h"
#include "miscadmin.h"
#include "pg_trace.h"
#include "pgstat.h"
#include "postmaster/bgwriter.h"
#include "storage/bufmgr.h"
#include "storage/fd.h"
#include "storage/md.h"
#include "storage/rpcserver.h"
#include "storage/relfilenode.h"
#include "storage/smgr.h"
#include "storage/sync.h"
#include "utils/hsearch.h"
#include "utils/memutils.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::tutorial;

class DataPageAccessHandler : virtual public DataPageAccessIf {
 public:
  DataPageAccessHandler() = default;

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param _fd
   */
  void RpcFileClose(const _File _fd) {
    FileClose(fd);
    std::cout << "RpcFileClose" << std::endl;
  }

  void RpcTablespaceCreateDbspace(const _Oid _spcnode, const _Oid _dbnode, const bool isRedo) {
    Oid spcnode = _spcnode;
    Oid _dbnode = _dbnode;
    TablespaceCreateDbspace(spcnode, dbnode, isRedo);
    std::cout << "RpcTablespaceCreateDbspace" << std::endl;
  }

  _File RpcPathNameOpenFile(const _Path& _path, const _Flag _flag) {
    char		path[MAXPGPATH];
		std::size_t length = _path.copy(path, _path.size());
		path[length] = '\0';

    std::cout << "RpcPathNameOpenFile" << std::endl;

    return PathNameOpenFile(path, _flag);
  }

  int32_t RpcFileWrite(const _File _fd, const _Page& _page, const _Off_t _seekpos) {
    char  buf[BLCKSZ];
    _page.copy(buf, BLCKSZ);

    std::cout << "RpcFileWrite" << std::endl;

    return FileWrite(_fd, buf, BLCKSZ, _seekpos, WAIT_EVENT_DATA_FILE_EXTEND);
  }

  void RpcFilePathName(_Path& _return, const _File _fd) {
    char	*path = FilePathName(_fd);
    _return.assign(path);

    std::cout << "RpcFilePathName" << std::endl;
  }

  void RpcFileRead(_Page& _return, const _File _fd, const _Off_t _seekpos) {
    char buf[BLCKSZ];
    FileRead(_fd, buf, BLCKSZ, _seekpos, WAIT_EVENT_DATA_FILE_READ);
    _return.assign(buf, BLCKSZ);

    std::cout << "RpcFileRead\n" << std::endl;
  }

  void RpcFileTruncate(const _File _fd, const _Off_t _offset) {
    FileTruncate(_fd, _offset, WAIT_EVENT_DATA_FILE_TRUNCATE);

    std::cout << "RpcFileTruncate\n" << std::endl;
  }

  _Off_t RpcFileSize(const _File _fd) {
    std::cout << "RpcFileSize\n" << std::endl;

    return FileSize(_fd);
  }

  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  void zip() {
    // Your implementation goes here
    printf("zip\n");
  }

};

void
RpcServerLoop{
  int port = 9090;
  ::std::shared_ptr<DataPageAccessHandler> handler(new DataPageAccessHandler());
  ::std::shared_ptr<TProcessor> processor(new DataPageAccessProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
}

