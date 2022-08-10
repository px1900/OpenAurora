/**
 * Autogenerated by Thrift Compiler (0.14.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef DataPageAccess_H
#define DataPageAccess_H

#include <thrift/TDispatchProcessor.h>
#include <thrift/async/TConcurrentClientSyncInfo.h>
#include <memory>
#include "tutorial_types.h"

namespace tutorial {

#ifdef _MSC_VER
  #pragma warning( push )
  #pragma warning (disable : 4250 ) //inheriting methods via dominance 
#endif

/**
 * Ahh, now onto the cool part, defining a service. Services just need a name
 * and can optionally inherit from another service using the extends keyword.
 */
class DataPageAccessIf {
 public:
  virtual ~DataPageAccessIf() {}

  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param _reln
   * @param _forknum
   * @param _blknum
   */
  virtual void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum) = 0;
  virtual int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum) = 0;
  virtual int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum) = 0;
  virtual void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo) = 0;
  virtual void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync) = 0;

  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  virtual void zip() = 0;
};

class DataPageAccessIfFactory {
 public:
  typedef DataPageAccessIf Handler;

  virtual ~DataPageAccessIfFactory() {}

  virtual DataPageAccessIf* getHandler(const ::apache::thrift::TConnectionInfo& connInfo) = 0;
  virtual void releaseHandler(DataPageAccessIf* /* handler */) = 0;
};

class DataPageAccessIfSingletonFactory : virtual public DataPageAccessIfFactory {
 public:
  DataPageAccessIfSingletonFactory(const ::std::shared_ptr<DataPageAccessIf>& iface) : iface_(iface) {}
  virtual ~DataPageAccessIfSingletonFactory() {}

  virtual DataPageAccessIf* getHandler(const ::apache::thrift::TConnectionInfo&) {
    return iface_.get();
  }
  virtual void releaseHandler(DataPageAccessIf* /* handler */) {}

 protected:
  ::std::shared_ptr<DataPageAccessIf> iface_;
};

class DataPageAccessNull : virtual public DataPageAccessIf {
 public:
  virtual ~DataPageAccessNull() {}
  void RpcMdRead(_Page& /* _return */, const _Smgr_Relation& /* _reln */, const int32_t /* _forknum */, const int64_t /* _blknum */) {
    return;
  }
  int32_t RpcMdNblocks(const _Smgr_Relation& /* _reln */, const int32_t /* _forknum */) {
    int32_t _return = 0;
    return _return;
  }
  int32_t RpcMdExists(const _Smgr_Relation& /* _reln */, const int32_t /* _forknum */) {
    int32_t _return = 0;
    return _return;
  }
  void RpcMdCreate(const _Smgr_Relation& /* _reln */, const int32_t /* _forknum */, const int32_t /* _isRedo */) {
    return;
  }
  void RpcMdExtend(const _Smgr_Relation& /* _reln */, const int32_t /* _forknum */, const int32_t /* _blknum */, const _Page& /* _buff */, const int32_t /* skipFsync */) {
    return;
  }
  void zip() {
    return;
  }
};

typedef struct _DataPageAccess_RpcMdRead_args__isset {
  _DataPageAccess_RpcMdRead_args__isset() : _reln(false), _forknum(false), _blknum(false) {}
  bool _reln :1;
  bool _forknum :1;
  bool _blknum :1;
} _DataPageAccess_RpcMdRead_args__isset;

class DataPageAccess_RpcMdRead_args {
 public:

  DataPageAccess_RpcMdRead_args(const DataPageAccess_RpcMdRead_args&);
  DataPageAccess_RpcMdRead_args& operator=(const DataPageAccess_RpcMdRead_args&);
  DataPageAccess_RpcMdRead_args() : _forknum(0), _blknum(0) {
  }

  virtual ~DataPageAccess_RpcMdRead_args() noexcept;
  _Smgr_Relation _reln;
  int32_t _forknum;
  int64_t _blknum;

  _DataPageAccess_RpcMdRead_args__isset __isset;

  void __set__reln(const _Smgr_Relation& val);

  void __set__forknum(const int32_t val);

  void __set__blknum(const int64_t val);

  bool operator == (const DataPageAccess_RpcMdRead_args & rhs) const
  {
    if (!(_reln == rhs._reln))
      return false;
    if (!(_forknum == rhs._forknum))
      return false;
    if (!(_blknum == rhs._blknum))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdRead_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdRead_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdRead_pargs {
 public:


  virtual ~DataPageAccess_RpcMdRead_pargs() noexcept;
  const _Smgr_Relation* _reln;
  const int32_t* _forknum;
  const int64_t* _blknum;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DataPageAccess_RpcMdRead_result__isset {
  _DataPageAccess_RpcMdRead_result__isset() : success(false) {}
  bool success :1;
} _DataPageAccess_RpcMdRead_result__isset;

class DataPageAccess_RpcMdRead_result {
 public:

  DataPageAccess_RpcMdRead_result(const DataPageAccess_RpcMdRead_result&);
  DataPageAccess_RpcMdRead_result& operator=(const DataPageAccess_RpcMdRead_result&);
  DataPageAccess_RpcMdRead_result() : success() {
  }

  virtual ~DataPageAccess_RpcMdRead_result() noexcept;
  _Page success;

  _DataPageAccess_RpcMdRead_result__isset __isset;

  void __set_success(const _Page& val);

  bool operator == (const DataPageAccess_RpcMdRead_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdRead_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdRead_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DataPageAccess_RpcMdRead_presult__isset {
  _DataPageAccess_RpcMdRead_presult__isset() : success(false) {}
  bool success :1;
} _DataPageAccess_RpcMdRead_presult__isset;

class DataPageAccess_RpcMdRead_presult {
 public:


  virtual ~DataPageAccess_RpcMdRead_presult() noexcept;
  _Page* success;

  _DataPageAccess_RpcMdRead_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DataPageAccess_RpcMdNblocks_args__isset {
  _DataPageAccess_RpcMdNblocks_args__isset() : _reln(false), _forknum(false) {}
  bool _reln :1;
  bool _forknum :1;
} _DataPageAccess_RpcMdNblocks_args__isset;

class DataPageAccess_RpcMdNblocks_args {
 public:

  DataPageAccess_RpcMdNblocks_args(const DataPageAccess_RpcMdNblocks_args&);
  DataPageAccess_RpcMdNblocks_args& operator=(const DataPageAccess_RpcMdNblocks_args&);
  DataPageAccess_RpcMdNblocks_args() : _forknum(0) {
  }

  virtual ~DataPageAccess_RpcMdNblocks_args() noexcept;
  _Smgr_Relation _reln;
  int32_t _forknum;

  _DataPageAccess_RpcMdNblocks_args__isset __isset;

  void __set__reln(const _Smgr_Relation& val);

  void __set__forknum(const int32_t val);

  bool operator == (const DataPageAccess_RpcMdNblocks_args & rhs) const
  {
    if (!(_reln == rhs._reln))
      return false;
    if (!(_forknum == rhs._forknum))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdNblocks_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdNblocks_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdNblocks_pargs {
 public:


  virtual ~DataPageAccess_RpcMdNblocks_pargs() noexcept;
  const _Smgr_Relation* _reln;
  const int32_t* _forknum;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DataPageAccess_RpcMdNblocks_result__isset {
  _DataPageAccess_RpcMdNblocks_result__isset() : success(false) {}
  bool success :1;
} _DataPageAccess_RpcMdNblocks_result__isset;

class DataPageAccess_RpcMdNblocks_result {
 public:

  DataPageAccess_RpcMdNblocks_result(const DataPageAccess_RpcMdNblocks_result&);
  DataPageAccess_RpcMdNblocks_result& operator=(const DataPageAccess_RpcMdNblocks_result&);
  DataPageAccess_RpcMdNblocks_result() : success(0) {
  }

  virtual ~DataPageAccess_RpcMdNblocks_result() noexcept;
  int32_t success;

  _DataPageAccess_RpcMdNblocks_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const DataPageAccess_RpcMdNblocks_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdNblocks_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdNblocks_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DataPageAccess_RpcMdNblocks_presult__isset {
  _DataPageAccess_RpcMdNblocks_presult__isset() : success(false) {}
  bool success :1;
} _DataPageAccess_RpcMdNblocks_presult__isset;

class DataPageAccess_RpcMdNblocks_presult {
 public:


  virtual ~DataPageAccess_RpcMdNblocks_presult() noexcept;
  int32_t* success;

  _DataPageAccess_RpcMdNblocks_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DataPageAccess_RpcMdExists_args__isset {
  _DataPageAccess_RpcMdExists_args__isset() : _reln(false), _forknum(false) {}
  bool _reln :1;
  bool _forknum :1;
} _DataPageAccess_RpcMdExists_args__isset;

class DataPageAccess_RpcMdExists_args {
 public:

  DataPageAccess_RpcMdExists_args(const DataPageAccess_RpcMdExists_args&);
  DataPageAccess_RpcMdExists_args& operator=(const DataPageAccess_RpcMdExists_args&);
  DataPageAccess_RpcMdExists_args() : _forknum(0) {
  }

  virtual ~DataPageAccess_RpcMdExists_args() noexcept;
  _Smgr_Relation _reln;
  int32_t _forknum;

  _DataPageAccess_RpcMdExists_args__isset __isset;

  void __set__reln(const _Smgr_Relation& val);

  void __set__forknum(const int32_t val);

  bool operator == (const DataPageAccess_RpcMdExists_args & rhs) const
  {
    if (!(_reln == rhs._reln))
      return false;
    if (!(_forknum == rhs._forknum))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdExists_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdExists_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdExists_pargs {
 public:


  virtual ~DataPageAccess_RpcMdExists_pargs() noexcept;
  const _Smgr_Relation* _reln;
  const int32_t* _forknum;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DataPageAccess_RpcMdExists_result__isset {
  _DataPageAccess_RpcMdExists_result__isset() : success(false) {}
  bool success :1;
} _DataPageAccess_RpcMdExists_result__isset;

class DataPageAccess_RpcMdExists_result {
 public:

  DataPageAccess_RpcMdExists_result(const DataPageAccess_RpcMdExists_result&);
  DataPageAccess_RpcMdExists_result& operator=(const DataPageAccess_RpcMdExists_result&);
  DataPageAccess_RpcMdExists_result() : success(0) {
  }

  virtual ~DataPageAccess_RpcMdExists_result() noexcept;
  int32_t success;

  _DataPageAccess_RpcMdExists_result__isset __isset;

  void __set_success(const int32_t val);

  bool operator == (const DataPageAccess_RpcMdExists_result & rhs) const
  {
    if (!(success == rhs.success))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdExists_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdExists_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

typedef struct _DataPageAccess_RpcMdExists_presult__isset {
  _DataPageAccess_RpcMdExists_presult__isset() : success(false) {}
  bool success :1;
} _DataPageAccess_RpcMdExists_presult__isset;

class DataPageAccess_RpcMdExists_presult {
 public:


  virtual ~DataPageAccess_RpcMdExists_presult() noexcept;
  int32_t* success;

  _DataPageAccess_RpcMdExists_presult__isset __isset;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DataPageAccess_RpcMdCreate_args__isset {
  _DataPageAccess_RpcMdCreate_args__isset() : _reln(false), _forknum(false), _isRedo(false) {}
  bool _reln :1;
  bool _forknum :1;
  bool _isRedo :1;
} _DataPageAccess_RpcMdCreate_args__isset;

class DataPageAccess_RpcMdCreate_args {
 public:

  DataPageAccess_RpcMdCreate_args(const DataPageAccess_RpcMdCreate_args&);
  DataPageAccess_RpcMdCreate_args& operator=(const DataPageAccess_RpcMdCreate_args&);
  DataPageAccess_RpcMdCreate_args() : _forknum(0), _isRedo(0) {
  }

  virtual ~DataPageAccess_RpcMdCreate_args() noexcept;
  _Smgr_Relation _reln;
  int32_t _forknum;
  int32_t _isRedo;

  _DataPageAccess_RpcMdCreate_args__isset __isset;

  void __set__reln(const _Smgr_Relation& val);

  void __set__forknum(const int32_t val);

  void __set__isRedo(const int32_t val);

  bool operator == (const DataPageAccess_RpcMdCreate_args & rhs) const
  {
    if (!(_reln == rhs._reln))
      return false;
    if (!(_forknum == rhs._forknum))
      return false;
    if (!(_isRedo == rhs._isRedo))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdCreate_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdCreate_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdCreate_pargs {
 public:


  virtual ~DataPageAccess_RpcMdCreate_pargs() noexcept;
  const _Smgr_Relation* _reln;
  const int32_t* _forknum;
  const int32_t* _isRedo;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdCreate_result {
 public:

  DataPageAccess_RpcMdCreate_result(const DataPageAccess_RpcMdCreate_result&);
  DataPageAccess_RpcMdCreate_result& operator=(const DataPageAccess_RpcMdCreate_result&);
  DataPageAccess_RpcMdCreate_result() {
  }

  virtual ~DataPageAccess_RpcMdCreate_result() noexcept;

  bool operator == (const DataPageAccess_RpcMdCreate_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdCreate_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdCreate_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdCreate_presult {
 public:


  virtual ~DataPageAccess_RpcMdCreate_presult() noexcept;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};

typedef struct _DataPageAccess_RpcMdExtend_args__isset {
  _DataPageAccess_RpcMdExtend_args__isset() : _reln(false), _forknum(false), _blknum(false), _buff(false), skipFsync(false) {}
  bool _reln :1;
  bool _forknum :1;
  bool _blknum :1;
  bool _buff :1;
  bool skipFsync :1;
} _DataPageAccess_RpcMdExtend_args__isset;

class DataPageAccess_RpcMdExtend_args {
 public:

  DataPageAccess_RpcMdExtend_args(const DataPageAccess_RpcMdExtend_args&);
  DataPageAccess_RpcMdExtend_args& operator=(const DataPageAccess_RpcMdExtend_args&);
  DataPageAccess_RpcMdExtend_args() : _forknum(0), _blknum(0), _buff(), skipFsync(0) {
  }

  virtual ~DataPageAccess_RpcMdExtend_args() noexcept;
  _Smgr_Relation _reln;
  int32_t _forknum;
  int32_t _blknum;
  _Page _buff;
  int32_t skipFsync;

  _DataPageAccess_RpcMdExtend_args__isset __isset;

  void __set__reln(const _Smgr_Relation& val);

  void __set__forknum(const int32_t val);

  void __set__blknum(const int32_t val);

  void __set__buff(const _Page& val);

  void __set_skipFsync(const int32_t val);

  bool operator == (const DataPageAccess_RpcMdExtend_args & rhs) const
  {
    if (!(_reln == rhs._reln))
      return false;
    if (!(_forknum == rhs._forknum))
      return false;
    if (!(_blknum == rhs._blknum))
      return false;
    if (!(_buff == rhs._buff))
      return false;
    if (!(skipFsync == rhs.skipFsync))
      return false;
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdExtend_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdExtend_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdExtend_pargs {
 public:


  virtual ~DataPageAccess_RpcMdExtend_pargs() noexcept;
  const _Smgr_Relation* _reln;
  const int32_t* _forknum;
  const int32_t* _blknum;
  const _Page* _buff;
  const int32_t* skipFsync;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdExtend_result {
 public:

  DataPageAccess_RpcMdExtend_result(const DataPageAccess_RpcMdExtend_result&);
  DataPageAccess_RpcMdExtend_result& operator=(const DataPageAccess_RpcMdExtend_result&);
  DataPageAccess_RpcMdExtend_result() {
  }

  virtual ~DataPageAccess_RpcMdExtend_result() noexcept;

  bool operator == (const DataPageAccess_RpcMdExtend_result & /* rhs */) const
  {
    return true;
  }
  bool operator != (const DataPageAccess_RpcMdExtend_result &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_RpcMdExtend_result & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_RpcMdExtend_presult {
 public:


  virtual ~DataPageAccess_RpcMdExtend_presult() noexcept;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);

};


class DataPageAccess_zip_args {
 public:

  DataPageAccess_zip_args(const DataPageAccess_zip_args&);
  DataPageAccess_zip_args& operator=(const DataPageAccess_zip_args&);
  DataPageAccess_zip_args() {
  }

  virtual ~DataPageAccess_zip_args() noexcept;

  bool operator == (const DataPageAccess_zip_args & /* rhs */) const
  {
    return true;
  }
  bool operator != (const DataPageAccess_zip_args &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const DataPageAccess_zip_args & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};


class DataPageAccess_zip_pargs {
 public:


  virtual ~DataPageAccess_zip_pargs() noexcept;

  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

};

class DataPageAccessClient : virtual public DataPageAccessIf {
 public:
  DataPageAccessClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
    setProtocol(prot);
  }
  DataPageAccessClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param _reln
   * @param _forknum
   * @param _blknum
   */
  void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum);
  void send_RpcMdRead(const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum);
  void recv_RpcMdRead(_Page& _return);
  int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum);
  void send_RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum);
  int32_t recv_RpcMdNblocks();
  int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum);
  void send_RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum);
  int32_t recv_RpcMdExists();
  void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo);
  void send_RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo);
  void recv_RpcMdCreate();
  void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync);
  void send_RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync);
  void recv_RpcMdExtend();
  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  void zip();
  void send_zip();
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
};

class DataPageAccessProcessor : public ::apache::thrift::TDispatchProcessor {
 protected:
  ::std::shared_ptr<DataPageAccessIf> iface_;
  virtual bool dispatchCall(::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, const std::string& fname, int32_t seqid, void* callContext);
 private:
  typedef  void (DataPageAccessProcessor::*ProcessFunction)(int32_t, ::apache::thrift::protocol::TProtocol*, ::apache::thrift::protocol::TProtocol*, void*);
  typedef std::map<std::string, ProcessFunction> ProcessMap;
  ProcessMap processMap_;
  void process_RpcMdRead(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_RpcMdNblocks(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_RpcMdExists(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_RpcMdCreate(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_RpcMdExtend(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
  void process_zip(int32_t seqid, ::apache::thrift::protocol::TProtocol* iprot, ::apache::thrift::protocol::TProtocol* oprot, void* callContext);
 public:
  DataPageAccessProcessor(::std::shared_ptr<DataPageAccessIf> iface) :
    iface_(iface) {
    processMap_["RpcMdRead"] = &DataPageAccessProcessor::process_RpcMdRead;
    processMap_["RpcMdNblocks"] = &DataPageAccessProcessor::process_RpcMdNblocks;
    processMap_["RpcMdExists"] = &DataPageAccessProcessor::process_RpcMdExists;
    processMap_["RpcMdCreate"] = &DataPageAccessProcessor::process_RpcMdCreate;
    processMap_["RpcMdExtend"] = &DataPageAccessProcessor::process_RpcMdExtend;
    processMap_["zip"] = &DataPageAccessProcessor::process_zip;
  }

  virtual ~DataPageAccessProcessor() {}
};

class DataPageAccessProcessorFactory : public ::apache::thrift::TProcessorFactory {
 public:
  DataPageAccessProcessorFactory(const ::std::shared_ptr< DataPageAccessIfFactory >& handlerFactory) :
      handlerFactory_(handlerFactory) {}

  ::std::shared_ptr< ::apache::thrift::TProcessor > getProcessor(const ::apache::thrift::TConnectionInfo& connInfo);

 protected:
  ::std::shared_ptr< DataPageAccessIfFactory > handlerFactory_;
};

class DataPageAccessMultiface : virtual public DataPageAccessIf {
 public:
  DataPageAccessMultiface(std::vector<std::shared_ptr<DataPageAccessIf> >& ifaces) : ifaces_(ifaces) {
  }
  virtual ~DataPageAccessMultiface() {}
 protected:
  std::vector<std::shared_ptr<DataPageAccessIf> > ifaces_;
  DataPageAccessMultiface() {}
  void add(::std::shared_ptr<DataPageAccessIf> iface) {
    ifaces_.push_back(iface);
  }
 public:
  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param _reln
   * @param _forknum
   * @param _blknum
   */
  void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->RpcMdRead(_return, _reln, _forknum, _blknum);
    }
    ifaces_[i]->RpcMdRead(_return, _reln, _forknum, _blknum);
    return;
  }

  int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->RpcMdNblocks(_reln, _forknum);
    }
    return ifaces_[i]->RpcMdNblocks(_reln, _forknum);
  }

  int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->RpcMdExists(_reln, _forknum);
    }
    return ifaces_[i]->RpcMdExists(_reln, _forknum);
  }

  void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->RpcMdCreate(_reln, _forknum, _isRedo);
    }
    ifaces_[i]->RpcMdCreate(_reln, _forknum, _isRedo);
  }

  void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync) {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->RpcMdExtend(_reln, _forknum, _blknum, _buff, skipFsync);
    }
    ifaces_[i]->RpcMdExtend(_reln, _forknum, _blknum, _buff, skipFsync);
  }

  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  void zip() {
    size_t sz = ifaces_.size();
    size_t i = 0;
    for (; i < (sz - 1); ++i) {
      ifaces_[i]->zip();
    }
    ifaces_[i]->zip();
  }

};

// The 'concurrent' client is a thread safe client that correctly handles
// out of order responses.  It is slower than the regular client, so should
// only be used when you need to share a connection among multiple threads
class DataPageAccessConcurrentClient : virtual public DataPageAccessIf {
 public:
  DataPageAccessConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(prot);
  }
  DataPageAccessConcurrentClient(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot, std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync) : sync_(sync)
{
    setProtocol(iprot,oprot);
  }
 private:
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> prot) {
  setProtocol(prot,prot);
  }
  void setProtocol(std::shared_ptr< ::apache::thrift::protocol::TProtocol> iprot, std::shared_ptr< ::apache::thrift::protocol::TProtocol> oprot) {
    piprot_=iprot;
    poprot_=oprot;
    iprot_ = iprot.get();
    oprot_ = oprot.get();
  }
 public:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getInputProtocol() {
    return piprot_;
  }
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> getOutputProtocol() {
    return poprot_;
  }
  /**
   * A method definition looks like C code. It has a return type, arguments,
   * and optionally a list of exceptions that it may throw. Note that argument
   * lists and exception lists are specified using the exact same syntax as
   * field lists in struct or exception definitions.
   * 
   * @param _reln
   * @param _forknum
   * @param _blknum
   */
  void RpcMdRead(_Page& _return, const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum);
  int32_t send_RpcMdRead(const _Smgr_Relation& _reln, const int32_t _forknum, const int64_t _blknum);
  void recv_RpcMdRead(_Page& _return, const int32_t seqid);
  int32_t RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum);
  int32_t send_RpcMdNblocks(const _Smgr_Relation& _reln, const int32_t _forknum);
  int32_t recv_RpcMdNblocks(const int32_t seqid);
  int32_t RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum);
  int32_t send_RpcMdExists(const _Smgr_Relation& _reln, const int32_t _forknum);
  int32_t recv_RpcMdExists(const int32_t seqid);
  void RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo);
  int32_t send_RpcMdCreate(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _isRedo);
  void recv_RpcMdCreate(const int32_t seqid);
  void RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync);
  int32_t send_RpcMdExtend(const _Smgr_Relation& _reln, const int32_t _forknum, const int32_t _blknum, const _Page& _buff, const int32_t skipFsync);
  void recv_RpcMdExtend(const int32_t seqid);
  /**
   * This method has a oneway modifier. That means the client only makes
   * a request and does not listen for any response at all. Oneway methods
   * must be void.
   */
  void zip();
  void send_zip();
 protected:
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> piprot_;
  std::shared_ptr< ::apache::thrift::protocol::TProtocol> poprot_;
  ::apache::thrift::protocol::TProtocol* iprot_;
  ::apache::thrift::protocol::TProtocol* oprot_;
  std::shared_ptr<::apache::thrift::async::TConcurrentClientSyncInfo> sync_;
};

#ifdef _MSC_VER
  #pragma warning( pop )
#endif

} // namespace

#endif
