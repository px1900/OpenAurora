// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "DataPageAccess.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>

using namespace ::apache::thrift;
using namespace ::apache::thrift::protocol;
using namespace ::apache::thrift::transport;
using namespace ::apache::thrift::server;

using namespace  ::tutorial;

class DataPageAccessHandler : virtual public DataPageAccessIf {
 public:
  DataPageAccessHandler() {
    // Your initialization goes here
  }

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param _reln
   * @param _relpersistence
   * @param _forknum
   * @param _blknum
   * @param _readBufferMode
   * @param _lsn
   */
  void ReadBufferCommon(_Page& _return, const _Smgr_Relation& _reln, const int32_t _relpersistence, const int32_t _forknum, const int32_t _blknum, const int32_t _readBufferMode, const int64_t _lsn) {
    // Your implementation goes here
    printf("ReadBufferCommon\n");
  }

  void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum, const int64_t _lsn) {
    // Your implementation goes here
    printf("RpcMdRead\n");
  }

  int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _lsn) {
    // Your implementation goes here
    printf("RpcMdNblocks\n");
  }

  int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _lsn) {
    // Your implementation goes here
    printf("RpcMdExists\n");
  }

  void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo, const int64_t _lsn) {
    // Your implementation goes here
    printf("RpcMdCreate\n");
  }

  void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync, const int64_t _lsn) {
    // Your implementation goes here
    printf("RpcMdExtend\n");
  }

  void RpcTruncate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const int64_t _lsn) {
    // Your implementation goes here
    printf("RpcTruncate\n");
  }

  void RpcFileClose(const _File _fd) {
    // Your implementation goes here
    printf("RpcFileClose\n");
  }

  void RpcTablespaceCreateDbspace(const _Oid _spcnode, const _Oid _dbnode, const bool isRedo) {
    // Your implementation goes here
    printf("RpcTablespaceCreateDbspace\n");
  }

  _File RpcPathNameOpenFile(const _Path& _path, const _Flag _flag) {
    // Your implementation goes here
    printf("RpcPathNameOpenFile\n");
  }

  int32_t RpcFileWrite(const _File _fd, const _Page& _page, const int32_t _amount, const _Off_t _seekpos, const int32_t _wait_event_info) {
    // Your implementation goes here
    printf("RpcFileWrite\n");
  }

  void RpcFilePathName(_Path& _return, const _File _fd) {
    // Your implementation goes here
    printf("RpcFilePathName\n");
  }

  void RpcFileRead(_Page& _return, const _File _fd, const int32_t _amount, const _Off_t _seekpos, const int32_t _wait_event_info) {
    // Your implementation goes here
    printf("RpcFileRead\n");
  }

  int32_t RpcFileTruncate(const _File _fd, const _Off_t _offset) {
    // Your implementation goes here
    printf("RpcFileTruncate\n");
  }

  _Off_t RpcFileSize(const _File _fd) {
    // Your implementation goes here
    printf("RpcFileSize\n");
  }

  int32_t RpcFilePrefetch(const _File _fd, const _Off_t _offset, const int32_t _amount, const int32_t wait_event_info) {
    // Your implementation goes here
    printf("RpcFilePrefetch\n");
  }

  void RpcFileWriteback(const _File _fd, const _Off_t _offset, const _Off_t nbytes, const int32_t wait_event_info) {
    // Your implementation goes here
    printf("RpcFileWriteback\n");
  }

  int32_t RpcUnlink(const _Path& _path) {
    // Your implementation goes here
    printf("RpcUnlink\n");
  }

  int32_t RpcFtruncate(const _File _fd, const _Off_t _offset) {
    // Your implementation goes here
    printf("RpcFtruncate\n");
  }

  void RpcInitFile(_Page& _return, const _Path& _path) {
    // Your implementation goes here
    printf("RpcInitFile\n");
  }

  _File RpcOpenTransientFile(const _Path& _filename, const int32_t _fileflags) {
    // Your implementation goes here
    printf("RpcOpenTransientFile\n");
  }

  int32_t RpcCloseTransientFile(const _File _fd) {
    // Your implementation goes here
    printf("RpcCloseTransientFile\n");
  }

  void Rpcread(_Page& _return, const _File _fd, const int32_t size) {
    // Your implementation goes here
    printf("Rpcread\n");
  }

  int32_t Rpcwrite(const _File _fd, const _Page& _page, const int32_t size) {
    // Your implementation goes here
    printf("Rpcwrite\n");
  }

  int32_t RpcFileSync(const _File _fd, const int32_t _wait_event_info) {
    // Your implementation goes here
    printf("RpcFileSync\n");
  }

  void RpcPgPRead(_Page& _return, const _File _fd, const int32_t _seg_bytes, const _Off_t _start_off) {
    // Your implementation goes here
    printf("RpcPgPRead\n");
  }

  int32_t RpcPgPWrite(const _File _fd, const _Page& _page, const int32_t _amount, const _Off_t _offset) {
    // Your implementation goes here
    printf("RpcPgPWrite\n");
  }

  int32_t RpcClose(const _File _fd) {
    // Your implementation goes here
    printf("RpcClose\n");
  }

  int32_t RpcBasicOpenFile(const _Path& _path, const int32_t _flags) {
    // Your implementation goes here
    printf("RpcBasicOpenFile\n");
  }

  int32_t RpcPgFdatasync(const _File _fd) {
    // Your implementation goes here
    printf("RpcPgFdatasync\n");
  }

  int32_t RpcPgFsyncNoWritethrough(const _File _fd) {
    // Your implementation goes here
    printf("RpcPgFsyncNoWritethrough\n");
  }

  int32_t RpcLseek(const int32_t _fd, const _Off_t _offset, const int32_t _flag) {
    // Your implementation goes here
    printf("RpcLseek\n");
  }

  void RpcStat(_Stat_Resp& _return, const _Path& _path) {
    // Your implementation goes here
    printf("RpcStat\n");
  }

  int32_t RpcDirectoryIsEmpty(const _Path& _path) {
    // Your implementation goes here
    printf("RpcDirectoryIsEmpty\n");
  }

  int32_t RpcCopyDir(const _Path& _src, const _Path& _dst) {
    // Your implementation goes here
    printf("RpcCopyDir\n");
  }

  int32_t RpcPgFsync(const int32_t _fd) {
    // Your implementation goes here
    printf("RpcPgFsync\n");
  }

  int32_t RpcDurableUnlink(const _Path& _fname, const int32_t _flag) {
    // Your implementation goes here
    printf("RpcDurableUnlink\n");
  }

  int32_t RpcDurableRenameExcl(const _Path& _oldFname, const _Path& _newFname, const int32_t _elevel) {
    // Your implementation goes here
    printf("RpcDurableRenameExcl\n");
  }

  int32_t RpcXLogWrite(const _File _fd, const _Page& _page, const int32_t _amount, const _Off_t _offset, const std::vector<int64_t> & _xlblocks, const int32_t _blknum, const int32_t _idx, const int64_t _lsn) {
    // Your implementation goes here
    printf("RpcXLogWrite\n");
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

int main(int argc, char **argv) {
  int port = 9090;
  ::std::shared_ptr<DataPageAccessHandler> handler(new DataPageAccessHandler());
  ::std::shared_ptr<TProcessor> processor(new DataPageAccessProcessor(handler));
  ::std::shared_ptr<TServerTransport> serverTransport(new TServerSocket(port));
  ::std::shared_ptr<TTransportFactory> transportFactory(new TBufferedTransportFactory());
  ::std::shared_ptr<TProtocolFactory> protocolFactory(new TBinaryProtocolFactory());

  TSimpleServer server(processor, serverTransport, transportFactory, protocolFactory);
  server.serve();
  return 0;
}

