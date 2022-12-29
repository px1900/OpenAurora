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
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/concurrency/ThreadManager.h>
#include "storage/fd.h"
#include "commands/tablespace.h"
#include "storage/rpcserver.h"
#include "storage/bufmgr.h"
#include <sys/stat.h>
#include "storage/copydir.h"
#include "storage/md.h"
#include "catalog/catalog.h"
#include "utils/relcache.h"
#include "storage/smgr.h"
#include "utils/rel.h"
#include "tcop/storage_server.h"
#include "access/logindex_hashmap.h"
#include "replication/walreceiver.h"
#include "storage/kv_interface.h"
#include "storage/buf_internals.h"
#include "access/xlog.h"

extern HashMap pageVersionHashMap;
extern HashMap relSizeHashMap;

extern int reachXlogTempEnd;

#define DEBUG_TIMING 1

#ifdef DEBUG_TIMING

#include <sys/time.h>
#include <pthread.h>
#include <cstdlib>

int initialized = 0;
struct timeval output_timing;

pthread_mutex_t timing_mutex = PTHREAD_MUTEX_INITIALIZER;
long readBufferCommon[36];
long readBufferCount[36];

long nblocksTime[16];
long nblocksCount[16];

long existsTime[16];
long existsCount[16];

void PrintTimingResult() {
    struct timeval now;

    if(!initialized){
        gettimeofday(&output_timing, NULL);
        initialized = 1;

        memset(readBufferCount, 0, 16*sizeof(readBufferCount[0]));
        memset(readBufferCommon, 0, 16*sizeof(readBufferCommon[0]));

        memset(nblocksCount, 0, 16*sizeof(nblocksCount[0]));
        memset(nblocksTime, 0, 16*sizeof(nblocksTime[0]));

        memset(existsCount, 0, 16*sizeof(existsCount[0]));
        memset(existsTime, 0, 16*sizeof(existsTime[0]));
    }


    gettimeofday(&now, NULL);

    if(now.tv_sec-output_timing.tv_sec >= 1) {
        for(int i = 0 ; i < 35; i++) {
            if(readBufferCount[i] == 0)
                continue;
            printf("readBufferCommon_%d = %ld\n",i,  readBufferCommon[i]/readBufferCount[i]);
            printf("total_readBufferCommon_%d = %ld, count = %ld\n",i,  readBufferCommon[i], readBufferCount[i]);
            fflush(stdout);
        }

        for(int i = 0 ; i < 9; i++) {
            if(nblocksCount[i] == 0)
                continue;
            printf("nblocks_%d = %ld\n",i,  nblocksTime[i]/nblocksCount[i]);
            printf("nblocks_%d = %ld, count = %ld\n",i,  nblocksTime[i], nblocksCount[i]);
            fflush(stdout);
        }

        for(int i = 0 ; i < 9; i++) {
            if(existsCount[i] == 0)
                continue;
            printf("exists_%d = %ld\n",i,  existsTime[i]/existsCount[i]);
            printf("exists_%d = %ld, count = %ld\n",i,  existsTime[i], existsCount[i]);
            fflush(stdout);
        }

        output_timing = now;
    }
}

#define START_TIMING(start_p)  \
do {                         \
    gettimeofday(start_p, NULL); \
} while(0);

#define RECORD_TIMING(start_p, end_p, global_timing, global_count) \
do { \
    gettimeofday(end_p, NULL); \
    pthread_mutex_lock(&timing_mutex); \
    (*global_timing) += ((*end_p.tv_sec*1000000+*end_p.tv_usec) - (*start_p.tv_sec*1000000+*start_p.tv_usec))/1000; \
    (*global_count)++;                                                               \
    pthread_mutex_unlock(&timing_mutex); \
    PrintTimingResult(); \
    gettimeofday(start_p, NULL); \
} while (0);


#endif

extern XLogRecPtr XLogParseUpto;


void WaitParse(int64_t _lsn) {
    if(WalRcvRunning() == false)
        return;
    XLogRecPtr flushUpto = WalRcv->flushedUpto;
    if((XLogRecPtr)_lsn > XLogParseUpto && XLogParseUpto < flushUpto && XLogParseUpto != 0) {
        printf("%s %d , parameter_lsn = %lu, ParseUpto = %lu, flushUpto = %lu\n",
               __func__ , __LINE__, _lsn, XLogParseUpto, flushUpto);
        fflush(stdout);
        XLogRecPtr targetLsn = flushUpto;
        if(_lsn < targetLsn)
            targetLsn = _lsn;

        XLogRecPtr prevParse = XLogParseUpto;

        int count = 0;
        while(XLogParseUpto < targetLsn-8192 && !reachXlogTempEnd ) {
//            printf("%s %d keep waiting , parameter_lsn = %lu, ParseUpto = %lu, flushUpto = %lu\n",
//                   __func__ , __LINE__, _lsn, XLogParseUpto, flushUpto);
//            fflush(stdout);
            count++;
            if(count % 10 == 0) {
                printf("%s %d keep waiting , parameter_lsn = %lu, ParseUpto = %lu, flushUpto = %lu\n",
                       __func__ , __LINE__, _lsn, XLogParseUpto, flushUpto);
                fflush(stdout);
                count = 0;
            }

//            printf("%s get into sleep\n", __func__ );
//            fflush(stdout);
            usleep(200);
//            if(reachXlogTempEnd && XLogParseUpto != prevParse) {
//                reachXlogTempEnd = 0;
//            }
//            if(reachXlogTempEnd) {
//                waitCount++;
//            }
        }

        if(reachXlogTempEnd) {
            reachXlogTempEnd = 0;
        }
    }
//    printf("%s %d exit\n" ,__func__ , __LINE__);
//    fflush(stdout);
}

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
     void ReadBufferCommon(_Page& _return, const _Smgr_Relation& _reln, const int32_t _relpersistence, const int32_t _forknum, const int32_t _blknum, const int32_t _readBufferMode, const int64_t _lsn) {
        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, blkNum = %d, lsn = %ld\n", __func__ , __FILE__, __LINE__,
               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _blknum, _lsn);
        fflush(stdout);

        WaitParse(_lsn);

#ifdef DEBUG_TIMING
        struct timeval start, end;
        START_TIMING(&start);
#endif
        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
//
//        uint64_t *uintList;
//        int listSize;
//        BufferTag tag;
//        INIT_BUFFERTAG(tag, rnode, (ForkNumber)_forknum, (BlockNumber)_blknum);

        // Get relation size from in-memory hashmap
        // If relation size is smaller than parameter request $_blknum, just return blank page (caused by RpcMdCreate, RpcMdExist)
        /**********************************START IN MEMORY REL SIZE CACHE***************************************/
//        char* blankPage = (char*) malloc(BLCKSZ);
//        MemSet(blankPage, 0, BLCKSZ);
//        struct KeyType key{};
//        key.SpcID = rnode.spcNode;
//        key.DbID = rnode.dbNode;
//        key.RelID = rnode.relNode;
//        key.ForkNum = _forknum;
//
//        uint64_t foundLsnInMemory;
//        int foundPageNumInMemory;
//        int foundInMemory = HashMapFindLowerBoundEntry(key, WalRcv->flushedUpto, &foundLsnInMemory, &foundPageNumInMemory);
//        // If relation size not cached, read from standalone process
//        if(!foundInMemory) {
//            int relSize = SyncGetRelSize(rnode, (ForkNumber)_forknum, 0);
//            printf("%s get relsize=%d from standalone pg\n", __func__ , relSize);
//            bool insertSucc = HashMapInsertKey(key, WalRcv->flushedUpto, relSize);
//            if(insertSucc) {
//                printf("%s insert key successfully\n", __func__ );
//            } else {
//                printf("%s insert key failed\n", __func__ );
//            }
//            // If existing relation is smaller than parameter, just return zero pages
//            if(relSize < _blknum+1) {
//                free(blankPage);
//                _return.assign(blankPage, BLCKSZ);
//                return;
//            }
//        } else { // Get relation size from cache
//            printf("%s get cached relsize=%d\n", __func__ , foundPageNumInMemory);
//            fflush(stdout);
//            if(foundPageNumInMemory < _blknum+1) {
//                free(blankPage);
//                _return.assign(blankPage, BLCKSZ);
//                return;
//            }
//        }
//         free(blankPage);
        /************************************END IN MEMORY REL SIZE CACHE***************************************/

        KeyType key;
        key.SpcID = _reln._spc_node;
        key.DbID = _reln._db_node;
        key.RelID = _reln._rel_node;
        key.ForkNum = _forknum;
        key.BlkNum = _blknum;

        uint64_t  replayedLsn;
        uint64_t *toReplayList;
        int listSize;
        int found = HashMapGetBlockReplayList(pageVersionHashMap, key, _lsn, &replayedLsn, &toReplayList, &listSize);
//        int found = GetListFromRocksdb(tag, &uintList, &listSize);

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &(readBufferCommon[0]), &(readBufferCount[0]))
#endif
        printf("%s found = %d\n", __func__ , found);
        fflush(stdout);


        // If not found list, get the base page from standalone process
        if(!found) {

            BufferTag bufferTag;
            INIT_BUFFERTAG(bufferTag,rnode, (ForkNumber)_forknum, (BlockNumber)_blknum);

            printf("%s %d\n", __func__ , __LINE__);
            fflush(stdout);

            char* buff;
            // Try to get RpcMdExtend page as BasePage
            if(GetPageFromRocksdb(bufferTag, 0, &buff) == 0) {
                printf("%s %d, can't find page in RocksDB-extend, try to find it from standalone\n", __func__ , __LINE__);
                fflush(stdout);
                // If not created by RpcMdExtend, get page from StandAlone process
                buff = (char*) malloc(BLCKSZ);
                GetBasePage(rnode, (ForkNumber)_forknum, (BlockNumber)_blknum, buff);
            }
            printf("%s %d \n", __func__ , __LINE__);
            fflush(stdout);
#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &(readBufferCommon[1]), &(readBufferCount[1]))
#endif
            XLogRecPtr lsn = PageGetLSN(buff);
            printf("%s %d, get basePage version, lsn = %lu\n", __func__ , __LINE__, lsn);
            fflush(stdout);


            HashMapInsertKey(pageVersionHashMap, key, lsn, -1, 1);
            HashMapUpdateReplayedLsn(pageVersionHashMap, key, lsn, false);

#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &(readBufferCommon[2]), &(readBufferCount[2]))
#endif
            // Remove extended page from Rocksdb
            DeletePageFromRocksdb(bufferTag, 0);
#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &(readBufferCommon[3]), &(readBufferCount[3]))
#endif
            // Put base page to Rocksdb
            PutPage2Rocksdb(bufferTag, lsn, buff);

#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &(readBufferCommon[4]), &(readBufferCount[4]))
#endif
            // Set return value from RPC client
            _return.assign(buff, BLCKSZ);

            free(buff);
            printf("%s %d\n", __func__ , __LINE__);
            fflush(stdout);
#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &(readBufferCommon[5]), &(readBufferCount[5]))
#endif
            return;
        }
         printf("%s %d\n", __func__ , __LINE__);
         fflush(stdout);

         // Now we found the target lsn list

        // If the lsn list $currPos is zero (no xlog was replayed)
        // Then this list was created by replay process, which won't consist base page's lsn.
        // So, we need to get base page from standalone process and insert it lsn as
        // the first element of the list. Also, update $currPos to 1 (base page "replayed")
        if(replayedLsn == 0) {
            char* buff = (char*) malloc(BLCKSZ);
            //TODO
            BufferTag bufferTag;
            INIT_BUFFERTAG(bufferTag,rnode, (ForkNumber)_forknum, (BlockNumber)_blknum);
//            if(GetPageFromRocksdb(bufferTag, 0, &buff) == 0) {
//                printf("%s %d, can't find page in RocksDB-extend, try to find it from standalone\n", __func__ , __LINE__);
//                fflush(stdout);
//                // If not created by RpcMdExtend, get page from StandAlone process
//                buff = (char*) malloc(BLCKSZ);
//                GetBasePage(rnode, (ForkNumber)_forknum, (BlockNumber)_blknum, buff);
//            }
            // TODO
            // It's possible that this base page doesn't exist in disk
            // We need to redo at least one lsn
            // Use Exist at first (memory and standalone), if not exist, replay one xlog
            int currRelSize = SyncGetRelSize(rnode, (ForkNumber)_forknum, (BlockNumber)_blknum);
            if(currRelSize == -1 && listSize != 0) { // we don't have base page in disk, should redo at least one lsn
//                if(listSize == 0) {
//                    printf("Error: %s %d, can't get basePage and any lsn\n", __func__ , __LINE__);
//                    fflush(stdout);
//                }
                ApplyOneLsnWithoutBasePage(rnode, (ForkNumber)_forknum, (BlockNumber)_blknum,
                                           toReplayList[0], buff);
            } else if (currRelSize == -1 && listSize == 0) {
                /*
                 * For some forknum != 0 relation, the RpcMdExtend will transfer a page with inner lsn equals to 0
                 * And there won't have corresponding xlog generated with this mdextend operation. Therefore, in the prior
                 * process, it is possible that this lsn list is created by RpcMdExtend, and insert the initial lsn=0
                 * (For forknum = 0 relation, extended pages inner lsn won't be zero), and in this function, we found the replayed
                 * lsn is 0, there are two possible reasons: 1. It was created by StartupXLOG, and mark first lsn as 0 to indicate we need
                 * extract base page lsn when replaying it. 2. RpcMdExtend insert initial lsn = 0 for forknum!=0 relations.
                 *
                 * If this list is created by Startup and replayedLsn = 0, then the replayed list len should larger than 0.
                 * So, when we reached here, it's the basePage created by RpcMdExtend, and it's forknum should not be 0.
                 * We should just get this page from RocksDB with lsn = 0.
                 */
                int foundBasePage = GetPageFromRocksdb(bufferTag, 0, &buff);
                if(!foundBasePage) {
                    printf("%s %d, Error: can't find basePage in disk or rocksdb, and there is no lsn to replay\n", __func__ , __LINE__);
                    fflush(stdout);
                }
            }
            else {
                GetBasePage(rnode, (ForkNumber)_forknum, (BlockNumber)_blknum, buff);
            }
#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &readBufferCommon[6], &readBufferCount[6])
#endif
            XLogRecPtr lsn = PageGetLSN(buff);

            // Put page to Rocksdb
            PutPage2Rocksdb(bufferTag, lsn, buff);

#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &readBufferCommon[7], &readBufferCount[7])
#endif
            // Set first slot's lsn as this page's lsn
            // Set replayed slot
            HashMapUpdateFirstEmptySlot(pageVersionHashMap, key, lsn);

            replayedLsn = lsn;
            printf("%s LINE=%d \n", __func__ , __LINE__);
            fflush(stdout);

            free(buff);

#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &readBufferCommon[8], &readBufferCount[8])
#endif
        }
         printf("%s LINE=%d \n", __func__ , __LINE__);
         fflush(stdout);


        // For now, we got the lsn list and at least one version was replayed
        // No need to replay the xlog
         if(listSize == 0) {
             char* page;
             BufferTag bufferTag;
             INIT_BUFFERTAG(bufferTag,rnode, (ForkNumber)_forknum, (BlockNumber)_blknum);
             if(!GetPageFromRocksdb(bufferTag, replayedLsn, &page)) {
                 // Not found page in rocksdb, report error
                 printf("%s Error, get replayed page failed\n", __func__ );
                 return;
             }

             _return.assign(page, BLCKSZ);
             free(page);
#ifdef DEBUG_TIMING
             RECORD_TIMING(&start, &end, &readBufferCommon[9], &readBufferCount[9])
#endif
             return;
         }

         printf("%s LINE=%d \n", __func__ , __LINE__);
         fflush(stdout);

         // For now, we need to replay several xlogs until we get the expected version
        // The xlog sublist we need to replay is [ uintList[1]+2 , foundPos ]


        BufferTag bufferTag;
        INIT_BUFFERTAG(bufferTag, rnode, (ForkNumber)_forknum, (BlockNumber)_blknum);

         printf("%s LINE=%d \n", __func__ , __LINE__);
         fflush(stdout);

         char* basePage;
         if(GetPageFromRocksdb(bufferTag, replayedLsn, &basePage) == 0) {
             printf("%s Error get base page for replay failed \n", __func__ );
             return;
         }

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &readBufferCommon[10], &readBufferCount[10])
#endif
        printf("%s LINE=%d \n", __func__ , __LINE__);
         fflush(stdout);
        char* page1 = (char*) malloc(BLCKSZ);
        char* page2 = (char*) malloc(BLCKSZ);
        char* tempPage;
        memcpy(page1, basePage, BLCKSZ);
        free(basePage);

         printf("%s LINE=%d \n", __func__ , __LINE__);
         fflush(stdout);

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &readBufferCommon[11], &readBufferCount[11])
#endif

        ApplyLsnList(rnode, (ForkNumber)_forknum, (BlockNumber)_blknum, toReplayList, listSize, page1, page2);
        printf("%s %d, rel = %lu, replayLsnSize = %d targetLsn = %lu, page real lsn = %lu\n",
        __func__, __LINE__, rnode.relNode, listSize, toReplayList[listSize-1], PageGetLSN(page2));
        fflush(stdout);

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &readBufferCommon[12], &readBufferCount[12])
#endif

        PutPage2Rocksdb(bufferTag, toReplayList[listSize-1], page2);

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &readBufferCommon[13], &readBufferCount[13])
#endif
//        for(int i = 0; i < listSize; i++) {
//            //TODO Apply a lsn list, however, the middle of versions will miss
//            //TODO if other second comp node need these middle versions, we can repaly for them
//
//             printf("%s LINE=%d \n", __func__ , __LINE__);
//             fflush(stdout);
//
//             uint64_t currLsn = toReplayList[i];
//             if(currLsn <= replayedLsn)
//                 continue;
//             ApplyOneLsn(rnode, (ForkNumber)_forknum, (BlockNumber)_blknum, currLsn, page1, page2);
//             printf("%s LINE=%d \n", __func__ , __LINE__);
//             fflush(stdout);
//            // Now the page2 is replayed version, put it to rocksdb
//            PutPage2Rocksdb(bufferTag, currLsn, page2);
//            // Set the page2 as the input page for next round
//            tempPage = page1;
//            page1 = page2;
//            page2 = tempPage;
//        }
         printf("%s LINE=%d \n", __func__ , __LINE__);
         fflush(stdout);


         if(listSize > 0) {
             HashMapUpdateReplayedLsn(pageVersionHashMap, key, toReplayList[listSize-1], true);

             free(toReplayList);
         }

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &readBufferCommon[14], &readBufferCount[14])
#endif

        //TODO: Return page2
        // Set the desired page version as return value
        _return.assign(page2, BLCKSZ);

        free(page1);
        free(page2);
//         printf("%s LINE=%d \n", __func__ , __LINE__);
//         fflush(stdout);

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &readBufferCommon[15], &readBufferCount[15])
#endif
//        char buff[BLCKSZ];
//        GetPageByLsn(rnode, (ForkNumber)_forknum, _blknum, 0, buff);
//        _return.assign(buff, BLCKSZ);
//
//        if(PageIsNew(buff)) {
//            printf("%s found page is new\n", __func__ );
//            fflush(stdout);
//        }
//        printf("%s End\n", __func__ );
    }

    void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum, const int64_t _lsn) {
        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, blkNum = %ld\n", __func__ , __FILE__, __LINE__,
               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _blknum);
        fflush(stdout);
        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);

        WaitParse(_lsn);

        char buff[BLCKSZ];
        GetPageByLsn(rnode, (ForkNumber)_forknum, _blknum, 0, buff);
        _return.assign(buff, BLCKSZ);

        printf("%s End\n", __func__ );
//        RelationData relationData;
//        memset(&relationData, 0, sizeof(RelationData));
//        Relation relation = &relationData;
//        relation->rd_smgr = smgrReln;

        // Not guarantee right
//        FormData_pg_class formDataPgClass;
//        formDataPgClass.relpersistence = RELPERSISTENCE_PERMANENT;
//        relation->rd_rel = &formDataPgClass;
//
//        Buffer buff = ReadBufferExtended(relation, (ForkNumber)_forknum, _blknum, RBM_NORMAL, NULL);
//
//        LockBuffer(buff, BUFFER_LOCK_SHARE);
//        char* page;
//        page = BufferGetPage(buff);
//
//        char tempPage[BLCKSZ];
//        memcpy(tempPage, page, BLCKSZ);
//
//        _return.resize(BLCKSZ);
//        _return.assign(tempPage, BLCKSZ);
//
//        UnlockReleaseBuffer(buff);

//        char page[BLCKSZ+16];
//        mdread(smgrReln, (ForkNumber)_forknum, _blknum, page);
//        _return.assign(page, BLCKSZ);
//
//        printf("%s end\n", __func__ );
//        fflush(stdout);
    }

    int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _lsn) {
        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, lsn = %ld tid=%d\n", __func__ , __FILE__, __LINE__,
               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _lsn, gettid());
        fflush(stdout);
        // Your implementation goes here
//        SyncReplayProcess();

#ifdef DEBUG_TIMING
        struct timeval start, end;
        START_TIMING(&start);
#endif
        WaitParse(_lsn);

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &nblocksTime[4], &nblocksCount[4])
#endif
        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;
//        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);

        XLogRecPtr lsn = _lsn;
        KeyType keyType;
        keyType.SpcID = rnode.spcNode;
        keyType.DbID = rnode.dbNode;
        keyType.RelID = rnode.relNode;
        keyType.ForkNum = _forknum;
        keyType.BlkNum = -1;

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &nblocksTime[0], &nblocksCount[0])
#endif
        uint64_t foundLsn;
        int foundPageNum;

        if ( HashMapFindLowerBoundEntry(relSizeHashMap, keyType, lsn, &foundLsn, &foundPageNum) ) {
            printf("%s cached, lsn = %lu, pageNum = %d\n", __func__ , foundLsn, foundPageNum);
            fflush(stdout);
#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &nblocksTime[1], &nblocksCount[1])
#endif
            return foundPageNum;
        }

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &nblocksTime[2], &nblocksCount[2])
#endif
        int relSize = SyncGetRelSize(rnode, (ForkNumber)_forknum, 0);
        printf("%s get relsize=%d from standalone pg\n", __func__ , relSize);
        fflush(stdout);
        bool insertSucc = HashMapInsertKey(relSizeHashMap, keyType, lsn, relSize, true);
        if(insertSucc) {
            printf("%s insert key successfully\n", __func__ );
        } else {
            printf("%s insert key failed\n", __func__ );
        }

//        BlockNumber result = mdnblocks(smgrReln, (ForkNumber)_forknum);
        printf("%s end\n", __func__ );
        fflush(stdout);
#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &nblocksTime[3], &nblocksCount[3])
#endif
        return relSize;
    }

    int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _lsn) {
#ifdef DEBUG_TIMING
        struct timeval start, end;
        START_TIMING(&start);
#endif
        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, lsn = %ld tid=%d\n", __func__ , __FILE__, __LINE__,
               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _lsn, gettid());
        fflush(stdout);

        WaitParse(_lsn);
//        SyncReplayProcess();

        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;

        KeyType key{};
        key.SpcID = rnode.spcNode;
        key.DbID = rnode.dbNode;
        key.RelID = rnode.relNode;
        key.ForkNum = _forknum;
        key.BlkNum = -1;

        uint64_t foundLsn;
        int foundPageNum;
        int found = HashMapFindLowerBoundEntry(relSizeHashMap, key, _lsn, &foundLsn, &foundPageNum);
        if(found) {
#ifdef DEBUG_TIMING
            RECORD_TIMING(&start, &end, &existsTime[0], &existsCount[0])
#endif
            if(foundPageNum == -1)
                return 0;
            else
                return 1;
        }

//        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);
//        int32_t result = mdexists(smgrReln, (ForkNumber)_forknum);

#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &existsTime[1], &existsCount[1])
#endif
        int relSize = SyncGetRelSize(rnode, (ForkNumber)_forknum, _lsn);
        printf("%s get relsize=%d from standalone pg\n", __func__ , relSize);
        bool insertSucc = HashMapInsertKey(relSizeHashMap, key, _lsn, relSize, true);
        if(insertSucc) {
            printf("%s insert key successfully\n", __func__ );
        } else {
            printf("%s insert key failed\n", __func__ );
        }

        printf("%s result = %d end\n", __func__, relSize);
        fflush(stdout);
#ifdef DEBUG_TIMING
        RECORD_TIMING(&start, &end, &existsTime[2], &existsCount[2])
#endif
        return (relSize>=0);
    }

    void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo, const int64_t _lsn) {
        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, lsn = %lu tid=%d\n", __func__ , __FILE__, __LINE__,
               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _lsn, gettid());
        fflush(stdout);

//        SyncReplayProcess();
        WaitParse(_lsn);

        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;

        KeyType key{};
        key.SpcID = rnode.spcNode;
        key.DbID = rnode.dbNode;
        key.RelID = rnode.relNode;
        key.ForkNum = _forknum;
        key.BlkNum = -1;

        uint64_t foundLsn;
        int foundPageNum;
        int found = HashMapFindLowerBoundEntry(relSizeHashMap, key, _lsn, &foundLsn, &foundPageNum);
        if(!found || foundPageNum<0) {
            if (HashMapInsertKey(relSizeHashMap, key, _lsn, 0, true) )
                printf("%s HashMap insert succeed\n", __func__ );
            else
                printf("%s HashMap insert failed\n", __func__ );
        }

//        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);
//        mdcreate(smgrReln, (ForkNumber)_forknum, _isRedo);
//        printf("%s end\n", __func__);
        fflush(stdout);
    }

    void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync, const int64_t _lsn) {
        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, blknum = %d lsn = %ld tid=%d\n", __func__ , __FILE__, __LINE__,
               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _blknum, _lsn, gettid());
        fflush(stdout);

//        SyncReplayProcess();
        WaitParse(_lsn);

        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;

        KeyType key{};
        key.SpcID = rnode.spcNode;
        key.DbID = rnode.dbNode;
        key.RelID = rnode.relNode;
        key.ForkNum = _forknum;
        key.BlkNum = -1;

        uint64_t foundLsn;
        int foundPageNum;
        int found = HashMapFindLowerBoundEntry(relSizeHashMap, key, _lsn, &foundLsn, &foundPageNum);
        printf("%s %d\n", __func__ , __LINE__);
        fflush(stdout);
        if(!found || foundPageNum<_blknum+1) {
            printf("%s %d\n", __func__ , __LINE__);
            fflush(stdout);
            if (HashMapInsertKey(relSizeHashMap, key, _lsn, _blknum+1, true) )
                printf("%s HashMap insert succeed, lsn=%lu, pageNum=%d\n", __func__, _lsn, _blknum+1 );
            else
                printf("%s HashMap insert failed\n", __func__ );
        }

        printf("%s %d\n", __func__ , __LINE__);
        fflush(stdout);
        BufferTag tag;
        INIT_BUFFERTAG(tag, rnode, (ForkNumber)_forknum, (BlockNumber)_blknum);
        char *extendPage = (char*) malloc(BLCKSZ);

        _buff.copy(extendPage, BLCKSZ);
        printf("%s %d, parameter lsn = %lu\n", __func__ , __LINE__, PageGetLSN(extendPage));
        fflush(stdout);
        // Put version:-1 to RocksDB
        PutPage2Rocksdb(tag, 0, extendPage);

        free(extendPage);
//        SMgrRelation smgrReln = smgropen(rnode, InvalidBackendId);
//        char extendPage[BLCKSZ+16];
//        _buff.copy(extendPage, BLCKSZ);
//
//        mdextend(smgrReln, (ForkNumber)_forknum, (BlockNumber)_blknum, extendPage, skipFsync);
        printf("%s end\n", __func__);
        fflush(stdout);
    }


    void RpcTruncate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const int64_t _lsn) {
        printf("%s %s %d , spcID = %ld, dbID = %ld, tabID = %ld, fornum = %d, blknum = %d lsn = %ld tid=%d\n", __func__ , __FILE__, __LINE__,
               _reln._spc_node, _reln._db_node, _reln._rel_node, _forknum, _blknum, _lsn, gettid());
        fflush(stdout);

        WaitParse(_lsn);

        RelFileNode rnode;
        rnode.spcNode = _reln._spc_node;
        rnode.dbNode = _reln._db_node;
        rnode.relNode = _reln._rel_node;

        KeyType key;
        key.SpcID = rnode.spcNode;
        key.DbID = rnode.dbNode;
        key.RelID = rnode.relNode;
        key.ForkNum = _forknum;
        key.BlkNum = -1;

        if (HashMapInsertKey(relSizeHashMap, key, _lsn, _blknum, true) )
            printf("%s HashMap insert succeed\n", __func__ );
        else
            printf("%s HashMap insert failed\n", __func__ );

        printf("%s end\n", __func__);
        fflush(stdout);
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

    std::shared_ptr<concurrency::ThreadFactory> threadFactory = std::make_shared<concurrency::ThreadFactory>(new concurrency::ThreadFactory());
//    std::shared_ptr<concurrency::ThreadManager> threadManager = concurrency::ThreadManager::newThreadManager();
    std::shared_ptr<concurrency::ThreadManager> threadManager = concurrency::ThreadManager::newSimpleThreadManager(50);
    threadManager->threadFactory(threadFactory);
    threadManager->start();


//    TSimpleServer server(
//    TThreadedServer server(
    std::shared_ptr<server::TServer> server;
    server.reset ( new TThreadPoolServer(
            std::make_shared<DataPageAccessProcessor>(std::make_shared<DataPageAccessHandler>()),
            std::make_shared<TServerSocket>(port), //port
            std::make_shared<TBufferedTransportFactory>(),
            std::make_shared<TBinaryProtocolFactory>(),
            threadManager
            ) );

    concurrency::ThreadFactory factory;
    factory.setDetached(false);
    std::shared_ptr<apache::thrift::concurrency::Runnable> serverThreadRunner(server);
    std::shared_ptr<apache::thrift::concurrency::Thread> thread = factory.newThread(serverThreadRunner);
    thread->start();
    concurrency::Monitor gMonitor;
    gMonitor.waitForever();

    server->stop();
    thread->join();
    server.reset();
}
