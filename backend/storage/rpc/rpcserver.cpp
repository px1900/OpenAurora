// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "c.h"
#include "postgres.h"
#include <cstring>


#include "DataPageAccess.h"
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TSimpleServer.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TBufferTransports.h>
#include <thrift/server/TThreadedServer.h>
#include "storage/fd.h"
#include "commands/tablespace.h"
#include "pgstat.h"
#include "storage/rpcserver.h"
#include "storage/bufmgr.h"
#include <sys/stat.h>
#include "storage/copydir.h"
#include "storage/md.h"
#include "catalog/catalog.h"
#include "utils/relcache.h"
#include "storage/smgr.h"
#include "utils/rel.h"

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
     * @param _fd
     */
    void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum) {
//        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, blkNum = %ld\n", __func__ , __FILE__, __LINE__,
//               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _blknum);
//        fflush(stdout);
        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);

        RelationData relationData;
        memset(&relationData, 0, sizeof(RelationData));
        Relation relation = &relationData;
        relation->rd_smgr = smgrReln;

        // Not guarantee right
        FormData_pg_class formDataPgClass;
        formDataPgClass.relpersistence = RELPERSISTENCE_PERMANENT;
        relation->rd_rel = &formDataPgClass;

//        printf("%s %s %d \n", __func__ , __FILE__, __LINE__);
        Buffer buff = ReadBufferExtended(relation, (ForkNumber)_forknum, _blknum, RBM_NORMAL, NULL);

        LockBuffer(buff, BUFFER_LOCK_SHARE);
        char* page;
        page = BufferGetPage(buff);

        char tempPage[BLCKSZ];
        memcpy(tempPage, page, BLCKSZ);

        _return.resize(BLCKSZ);
        _return.assign(tempPage, BLCKSZ);

        UnlockReleaseBuffer(buff);

//        char page[BLCKSZ+16];
//        mdread(smgrReln, (ForkNumber)_forknum, _blknum, page);
//        _return.assign(page, BLCKSZ);

        printf("%s end\n", __func__ );
        fflush(stdout);
    }

    int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum) {
//        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d\n", __func__ , __FILE__, __LINE__,
//               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum);
//        fflush(stdout);
        // Your implementation goes here
        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);

        int32_t result = mdnblocks(smgrReln, (ForkNumber)_forknum);
//        printf("%s result = %d end\n", __func__, result);
//        fflush(stdout);
        return result;
    }

    int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum) {
//        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d\n", __func__ , __FILE__, __LINE__,
//               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum);
//        fflush(stdout);

        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);
        int32_t result = mdexists(smgrReln, (ForkNumber)_forknum);
//        printf("%s result = %d end\n", __func__, result);
//        fflush(stdout);
        return result;
    }

    void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo) {
//        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d\n", __func__ , __FILE__, __LINE__,
//               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum);
//        fflush(stdout);

        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);
        mdcreate(smgrReln, (ForkNumber)_forknum, _isRedo);
//        printf("%s end\n", __func__);
//        fflush(stdout);
    }

    void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync) {
//        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d\n", __func__ , __FILE__, __LINE__,
//               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum);
//        fflush(stdout);
        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);
//        char* extendPage = (char*)malloc(BLCKSZ);
        char extendPage[BLCKSZ+16];
        _buff.copy(extendPage, BLCKSZ);

        mdextend(smgrReln, (ForkNumber)_forknum, (BlockNumber)_blknum, extendPage, skipFsync);
//        free(extendPage);
//        printf("%s end\n", __func__);
//        fflush(stdout);
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
RpcServerLoop(void){
    int port = 9090;

//    TSimpleServer server(
    TThreadedServer server(
            std::make_shared<DataPageAccessProcessor>(std::make_shared<DataPageAccessHandler>()),
            std::make_shared<TServerSocket>(port), //port
            std::make_shared<TBufferedTransportFactory>(),
            std::make_shared<TBinaryProtocolFactory>());

    server.serve();
}
