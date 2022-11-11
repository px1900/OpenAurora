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
   */
  void ReadBufferCommon(_Page& _return, const _Smgr_Relation& _reln, const int32_t _relpersistence, const int32_t _forknum, const int32_t _blknum, const int32_t _readBufferMode) {
    // Your implementation goes here
    printf("ReadBufferCommon\n");
  }

  void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum) {
    // Your implementation goes here
    printf("RpcMdRead\n");
  }

  int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum) {
    // Your implementation goes here
    printf("RpcMdNblocks\n");
  }

  int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum) {
    // Your implementation goes here
    printf("RpcMdExists\n");
  }

  void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo) {
    // Your implementation goes here
    printf("RpcMdCreate\n");
  }

  void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync) {
    // Your implementation goes here
    printf("RpcMdExtend\n");
  }

  void RpcTruncate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum) {
    // Your implementation goes here
    printf("RpcTruncate\n");
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

