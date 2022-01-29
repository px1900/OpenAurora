/**
 * Autogenerated by Thrift Compiler (0.14.0)
 *
 * DO NOT EDIT UNLESS YOU ARE SURE THAT YOU KNOW WHAT YOU ARE DOING
 *  @generated
 */
#ifndef tutorial_TYPES_H
#define tutorial_TYPES_H

#include <iosfwd>

#include <thrift/Thrift.h>
#include <thrift/TApplicationException.h>
#include <thrift/TBase.h>
#include <thrift/protocol/TProtocol.h>
#include <thrift/transport/TTransport.h>

#include <functional>
#include <memory>


namespace tutorial {

struct _ForkNumber {
  enum type {
    InvalidForkNumber = -1,
    MAIN_FORKNUM = 0,
    FSM_FORKNUM = 1,
    VISIBILITYMAP_FORKNUM = 2,
    INIT_FORKNUM = 3
  };
};

extern const std::map<int, const char*> __ForkNumber_VALUES_TO_NAMES;

std::ostream& operator<<(std::ostream& out, const _ForkNumber::type& val);

std::string to_string(const _ForkNumber::type& val);

typedef int32_t File;

/**
 * Data structures in thrift do not support unsigned int32, so we use i64 instead.
 * Remember to change the struct being used.
 */
typedef int64_t _Oid;

typedef int64_t _BlockNumber;

class _RelFileNode;

class _Page;

typedef struct __RelFileNode__isset {
  __RelFileNode__isset() : spcNode(false), dbNode(false), relNode(false) {}
  bool spcNode :1;
  bool dbNode :1;
  bool relNode :1;
} __RelFileNode__isset;

class _RelFileNode : public virtual ::apache::thrift::TBase {
 public:

  _RelFileNode(const _RelFileNode&);
  _RelFileNode& operator=(const _RelFileNode&);
  _RelFileNode() : spcNode(0), dbNode(0), relNode(0) {
  }

  virtual ~_RelFileNode() noexcept;
  _Oid spcNode;
  _Oid dbNode;
  _Oid relNode;

  __RelFileNode__isset __isset;

  void __set_spcNode(const _Oid val);

  void __set_dbNode(const _Oid val);

  void __set_relNode(const _Oid val);

  bool operator == (const _RelFileNode & rhs) const
  {
    if (!(spcNode == rhs.spcNode))
      return false;
    if (!(dbNode == rhs.dbNode))
      return false;
    if (!(relNode == rhs.relNode))
      return false;
    return true;
  }
  bool operator != (const _RelFileNode &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const _RelFileNode & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(_RelFileNode &a, _RelFileNode &b);

std::ostream& operator<<(std::ostream& out, const _RelFileNode& obj);

typedef struct __Page__isset {
  __Page__isset() : node(false), forknumber(false), blocknum(false), content(false) {}
  bool node :1;
  bool forknumber :1;
  bool blocknum :1;
  bool content :1;
} __Page__isset;

class _Page : public virtual ::apache::thrift::TBase {
 public:

  _Page(const _Page&);
  _Page& operator=(const _Page&);
  _Page() : forknumber((_ForkNumber::type)0), blocknum(0), content() {
  }

  virtual ~_Page() noexcept;
  _RelFileNode node;
  /**
   * 
   * @see _ForkNumber
   */
  _ForkNumber::type forknumber;
  _BlockNumber blocknum;
  std::string content;

  __Page__isset __isset;

  void __set_node(const _RelFileNode& val);

  void __set_forknumber(const _ForkNumber::type val);

  void __set_blocknum(const _BlockNumber val);

  void __set_content(const std::string& val);

  bool operator == (const _Page & rhs) const
  {
    if (!(node == rhs.node))
      return false;
    if (!(forknumber == rhs.forknumber))
      return false;
    if (!(blocknum == rhs.blocknum))
      return false;
    if (!(content == rhs.content))
      return false;
    return true;
  }
  bool operator != (const _Page &rhs) const {
    return !(*this == rhs);
  }

  bool operator < (const _Page & ) const;

  uint32_t read(::apache::thrift::protocol::TProtocol* iprot);
  uint32_t write(::apache::thrift::protocol::TProtocol* oprot) const;

  virtual void printTo(std::ostream& out) const;
};

void swap(_Page &a, _Page &b);

std::ostream& operator<<(std::ostream& out, const _Page& obj);

} // namespace

#endif