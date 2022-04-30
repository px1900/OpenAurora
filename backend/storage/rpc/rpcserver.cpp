// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include <iostream>
#include "DataPageAccess.h"
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include <thrift/TToString.h>

#include "postgres.h"

#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

#include <fstream>
#include <sstream>

#include "access/xlog.h"
#include "access/xlogutils.h"
#include "catalog/pg_tablespace.h"
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
#include "storage/kv.h"
#include "storage/kvstore.h"
#include "storage/smgr.h"
#include "storage/sync.h"
#include "storage/lmgr.h"
#include "utils/hsearch.h"
#include "utils/memutils.h"

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;
using namespace ::apache::thrift::concurrency;

using namespace  ::tutorial;

_Page readFileIntoString(char * filename);

class DataPageAccessHandler : virtual public DataPageAccessIf {
 public:
  DataPageAccessHandler() = default;

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param _spcNode
   * @param _dbNode
   * @param _relNode
   * @param fork
   * @param upperLSN
   * @param lowerLSN
   */
  int64_t RpcKvNblocks(const _Oid _spcNode, const _Oid _dbNode, 
  const _Oid _relNode, const int32_t fork, const int64_t upperLSN, 
  const int64_t lowerLSN) {
    RelFileNode rnode;
    rnode.spcNode = (Oid)_spcNode;
    rnode.dbNode = (Oid)_dbNode;
    rnode.relNode = (Oid)_relNode;

    SMgrRelation rel = smgropen(rnode, InvalidBackendId);

    std::cout << "RpcKvNblocks" << std::endl;
    return smgrnblocks(rel, (ForkNumber)fork);
  }

  void RpcKvRead(_Page& _return, const _Oid _spcNode, const _Oid _dbNode, 
  const _Oid _relNode, const int32_t fork, const int64_t block, 
  const int64_t upperLSN, const int64_t lowerLSN) {
    /*
    RelFileNode rnode;
    rnode.spcNode = (Oid)_spcNode;
    rnode.dbNode = (Oid)_dbNode;
    rnode.relNode = (Oid)_relNode;
    XLogRecPtr LSN = (((uint64_t)upperLSN) << 32) | ((uint64_t)lowerLSN); 
    char buf[BLCKSZ];

    SMgrRelation rel = smgropen(rnode, InvalidBackendId);

    smgrread(rel, (ForkNumber)fork, (BlockNumber)block, buf);

    _return.assign(buf, BLCKSZ);
    */
   RelFileNode rnode;
    rnode.spcNode = (Oid)_spcNode;
    rnode.dbNode = (Oid)_dbNode;
    rnode.relNode = (Oid)_relNode;
   Buffer buf;
   Page page;

    buf = ReadBufferWithoutRelcache(rnode, (ForkNumber)fork, (BlockNumber)block, RBM_NORMAL, NULL);

    page = BufferGetPage(buf);

    _return.assign(page, BLCKSZ);
 
    std::cout << "RpcKvRead" << std::endl;
  }

  void RpcInitFile(_Page& _return, const _Path& _path) {
    _Path datadir(DataDir);
    _Path mpath = datadir + "/" + _path;

    char		path[MAXPGPATH];
		std::size_t length = mpath.copy(path, mpath.size());
		path[length] = '\0';

    _return = readFileIntoString(path);
    
    std::cout << "RpcInitFile" << std::endl;
  }

  _File RpcOpenTransientFile(const _Path& _filename, const int32_t _fileflags) {
    char		path[MAXPGPATH];
		std::size_t length = _filename.copy(path, _filename.size());
		path[length] = '\0';

    std::cout << "RpcOpenTransientFile" << std::endl;

    return OpenTransientFile(path, _fileflags);
  }

  void RpcCloseTransientFile(const _File _fd) {
    CloseTransientFile(_fd);
    std::cout << "RpcCloseTransientFile" << std::endl;
  }

  void Rpcread(_Page& _return, const _File _fd, const int32_t size) {
    char buf[size];
    read(_fd, buf, size);
    _return.assign(buf, size);

    std::cout << "Rpcread" << std::endl;
  }

  int32_t Rpcwrite(const _File _fd, const _Page& _page, const int32_t size) {
    char  buf[size];
    _page.copy(buf, size);

    std::cout << "Rpcwrite" << std::endl;

    return write(_fd, buf, size);
  }

  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  void zip() {
    // Your implementation goes here
    std::cout << "zip do something?\n" << std::endl;
  }

};

_Page readFileIntoString(char * filename)
{
  std::ifstream ifile(filename);

  std::ostringstream buf;
  char ch;
  while(buf&&ifile.get(ch))
    buf.put(ch);
  ifile.close();

  return buf.str();
}

void
RpcServerLoop(void){
  int port = RPCPORT;

  TThreadedServer server(
    std::make_shared<DataPageAccessProcessor>(std::make_shared<DataPageAccessHandler>()),
    std::make_shared<TServerSocket>(port), //port
    std::make_shared<TBufferedTransportFactory>(),
    std::make_shared<TBinaryProtocolFactory>());

  server.serve();
}

