//
// Created by pang65 on 6/19/22.
//

#ifndef THRIFT_TEST_RPCCLIENT_H
#define THRIFT_TEST_RPCCLIENT_H


#ifdef __cplusplus
extern "C" {
#endif

    void RpcInit(void);
    void RpcFileClose(const int _fd);
    void RpcTablespaceCreateDbspace(const int64_t _spcnode, const int64_t _dbnode, const bool isRedo);
    int RpcPathNameOpenFile(const char* _path, const int32_t _flag);
    int32_t RpcFileWrite(const int _fd, const char* page, const int32_t _amount, const int64_t _seekpos, const int32_t _wait_event_info);
    char* RpcFilePathName(const int _fd);
    int RpcFileRead(char *buff, const int _fd, const int32_t _amount,  const int64_t _seekpos, const int32_t _wait_event_info);
    int32_t RpcFileTruncate(const int _fd, const int64_t _offset);
    int64_t RpcFileSize(const int _fd);
    int32_t RpcFilePrefetch(const int _fd, const int64_t _offset, const int32_t _amount, const int32_t wait_event_info);
    void RpcFileWriteback(const int _fd, const int64_t _offset, const int64_t nbytes, const int32_t wait_event_info);
    int32_t RpcUnlink(const char* _path);
    int32_t RpcFtruncate(const int _fd, const int64_t _offset);
    void RpcInitFile(char* _return, const char* _path);
    int RpcOpenTransientFile(const char* _filename, const int32_t _fileflags);
    int32_t RpcCloseTransientFile(const int _fd);
    int32_t RpcFileSync(const int _fd, const int32_t _wait_event_info);
    int32_t RpcPgPRead(const int _fd, char *p, const int32_t _amount, const int32_t _offset);
    int32_t RpcPgPWrite(const int _fd, char *p, const int32_t _amount, const int32_t _offset);
    int32_t RpcClose(const int _fd);
    int32_t RpcBasicOpenFile(char *path, int32_t _flags, char* file, char* func, int line);
    int32_t RpcPgFdatasync(const int32_t _fd);
    int32_t RpcPgFsyncNoWritethrough(const int32_t _fd);

#ifdef __cplusplus
}
#endif


#endif //THRIFT_TEST_RPCCLIENT_H
