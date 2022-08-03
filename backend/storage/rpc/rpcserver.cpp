// This autogenerated skeleton file illustrates how to build a server.
// You should copy it to another filename to avoid overwriting it.

#include "c.h"
#include "postgres.h"


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
    void RpcFileClose(const _File _fd) {
        // Your implementation goes here
        printf("RpcFileClose\n");
        FileClose(_fd);
    }

    void RpcTablespaceCreateDbspace(const _Oid _spcnode, const _Oid _dbnode, const bool isRedo) {
        // Your implementation goes here
        printf("RpcTablespaceCreateDbspace\n");
        TablespaceCreateDbspace(_spcnode, _dbnode, isRedo);
    }

    _File RpcPathNameOpenFile(const _Path& _path, const _Flag _flag) {
        // Your implementation goes here
        printf("RpcPathNameOpenFile, path = %s, flag = %d, with_create = %d\n", _path.c_str(), _flag, _flag&O_CREAT);
        _File result =  PathNameOpenFile(_path.c_str(), _flag);
        printf("[%s] result = %d\n", __func__ , result);
        return result;
    }

    // todo
    int32_t RpcFileWrite(const _File _fd, const _Page& _page, const int32_t _amount, const _Off_t _seekpos, const int32_t _wait_event_info) {
        // Your implementation goes here
        printf("RpcFileWrite\n");
//        char buff[BLCKSZ+8];
//        _page.copy(buff, BLCKSZ);
//        return FileWrite(_fd, buff, _amount, _seekpos, _wait_event_info);

        int writeLen = 0;
        char * page = (char *) malloc(BLCKSZ);
        _page.copy(page, BLCKSZ);

//        RpcRelation relNode = ParseRpcRequestPath(FilePathName(_fd), _seekpos);
//        if (relNode.parseSucc) {
//            writeLen = RpcFileWriteWithCache(_fd, page, relNode, _seekpos);
//        } else {
            // We arrived here only when some rare cases occur, for example, have backendID etc.
            writeLen = FileWrite(_fd, page, _amount, _seekpos, _wait_event_info);
//        }

        free(page);
        return writeLen;
    }

    void RpcFilePathName(_Path& _return, const _File _fd) {
        // Your implementation goes here
        printf("RpcFilePathName\n");
        char * filename = FilePathName(_fd);
        _return.assign(filename);
        return;
    }

    // todo
    void RpcFileRead(_Page& _return, const _File _fd, const int32_t _amount, const _Off_t _seekpos, const int32_t _wait_event_info) {
        // Your implementation goes here
        printf("RpcFileRead, fd = %d, amount = %d, seekpos = %d\n", _fd, _amount, _seekpos);
        printf("path = %s\n", FilePathName(_fd));
        fflush(stdout);
        char *page = (char *)malloc(BLCKSZ);
        int readLen = 0;

//        RpcRelation relNode = ParseRpcRequestPath(FilePathName(_fd), _seekpos);
//        printf("[%s]parse succ=%d, spc = %d, db = %d, rel = %d, fork = %d, block = %d\n",
//               __func__, relNode.parseSucc, relNode.spcNode, relNode.dbNode, relNode.relNode, relNode.forkNum, relNode.blockNum);
//        if (relNode.parseSucc) {
//            readLen = RpcFileReadWithCache(_fd, page, relNode, _seekpos);
//        } else {
            // We arrived here only when some rare cases occur, for example, have backendID etc.
            readLen = FileRead(_fd, page, _amount, _seekpos, _wait_event_info);
//        }

        _return.assign(page, readLen);
        free(page);
        return;
    }

    // todo
    int32_t RpcFileTruncate(const _File _fd, const _Off_t _offset) {
        // Your implementation goes here
        printf("RpcFileTruncate\n");
        return FileTruncate(_fd, _offset, WAIT_EVENT_DATA_FILE_TRUNCATE);
    }

    _Off_t RpcFileSize(const _File _fd) {
        // Your implementation goes here
        printf("RpcFileSize\n");
        return FileSize(_fd);
    }

    int32_t RpcFilePrefetch(const _File _fd, const _Off_t _offset, const int32_t _amount, const int32_t wait_event_info) {
        // Your implementation goes here
        printf("RpcFilePrefetch\n");
        return FilePrefetch(_fd, _offset, _amount, wait_event_info);
    }

    void RpcFileWriteback(const _File _fd, const _Off_t _offset, const _Off_t nbytes, const int32_t wait_event_info) {
        // Your implementation goes here
        printf("RpcFileWriteback\n");
        FileWriteback(_fd, _offset, nbytes, wait_event_info);
        return;
    }

    int32_t RpcUnlink(const _Path& _path) {
        // Your implementation goes here
        printf("RpcUnlink\n");
        return unlink(_path.c_str());
    }

    int32_t RpcFtruncate(const _File _fd, const _Off_t _offset) {
        // Your implementation goes here
        printf("RpcFtruncate\n");
        return ftruncate(_fd, _offset);
    }

    void RpcInitFile(_Page& _return, const _Path& _path) {
        // Your implementation goes here
        printf("RpcInitFile\n");
    }

    _File RpcOpenTransientFile(const _Path& _filename, const int32_t _fileflags) {
        // Your implementation goes here
        printf("RpcOpenTransientFile\n");
        return OpenTransientFile(_filename.c_str(), _fileflags);
    }

    int32_t RpcCloseTransientFile(const _File _fd) {
        // Your implementation goes here
        printf("RpcCloseTransientFile\n");
        return CloseTransientFile(_fd);
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
        return FileSync(_fd, _wait_event_info);
    }

    void RpcPgPRead(_Page& _return, const _File _fd, const int32_t _seg_bytes, const _Off_t _start_off) {
        // Your implementation goes here
        printf("RpcPgPRead\n");
        char *p = (char*)malloc(_seg_bytes+64);
        pg_pread(_fd, p, _seg_bytes, _start_off);
        _return.assign(p, _seg_bytes);
        free(p);
        return;
    }

    int32_t RpcPgPWrite(const _File _fd, const _Page& _page, const int32_t _amount, const _Off_t _offset) {
        // Your implementation goes here
        printf("RpcPgPWrite\n");
        return pg_pwrite(_fd, _page.c_str(), _amount, _offset);
    }

    int32_t RpcClose(const _File _fd) {
        // Your implementation goes here
        printf("RpcClose\n");
        return close(_fd);
    }

    int32_t RpcBasicOpenFile(const _Path& _path, const int32_t _flags) {
        // Your implementation goes here
        printf("RpcBasicOpenFile\n");
        return BasicOpenFile(_path.c_str(), _flags);
    }

    int32_t RpcPgFdatasync(const _File _fd) {
        // Your implementation goes here
        printf("RpcPgFdatasync\n");
        return pg_fdatasync(_fd);
    }

    int32_t RpcPgFsyncNoWritethrough(const _File _fd) {
        // Your implementation goes here
        printf("RpcPgFsyncNoWritethrough\n");
        return pg_fsync_no_writethrough(_fd);
    }

    int32_t RpcLseek(const int32_t _fd, const _Off_t _offset, const int32_t _flag) {
        // Your implementation goes here
        printf("RpcLseek\n");
        return lseek(_fd, _offset, _flag);
    }

    void RpcStat(_Stat_Resp& _return, const _Path& _path) {
        // Your implementation goes here
        printf("RpcStat\n");
        struct stat result;
        _return._result = stat(_path.c_str(), &result);
        _return._stat_mode = result.st_mode;
        return;
    }

    int32_t RpcDirectoryIsEmpty(const _Path& _path) {
        // Your implementation goes here
        printf("RpcDirectoryIsEmpty\n");
        return directory_is_empty(_path.c_str());
    }

    int32_t RpcCopyDir(const _Path& _src, const _Path& _dst) {
        // Your implementation goes here
        printf("RpcCopyDir\n");
        char* src = (char*) malloc(1024);
        char* dst = (char*) malloc(1024);
//        printf("_src = %s, dst = %s\n", _src.c_str(), _dst.c_str());
        _src.copy(src, _src.length());
        _dst.copy(dst, _dst.length());
        src[_src.length()] = 0;
        dst[_dst.length()] = 0;
//        printf("src = %s, dst = %s\n", src, dst);
        copydir(src, dst, false);
        free(src);
        free(dst);
        return 0;
    }

    void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum) {
        printf("RpcMdRead");
    }

    int32_t RpcMdExist(const _Smgr_Relation& _reln, const int32_t _forknum) {
        // Your implementation goes here
        printf("RpcMdExist\n");
    }

    int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum) {
        // Your implementation goes here
        printf("RpcMdNblocks\n");
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

    printf("%s start\n", __func__ );
    TThreadedServer server(
            std::make_shared<DataPageAccessProcessor>(std::make_shared<DataPageAccessHandler>()),
            std::make_shared<TServerSocket>(port), //port
            std::make_shared<TBufferedTransportFactory>(),
            std::make_shared<TBinaryProtocolFactory>());

    server.serve();
}
