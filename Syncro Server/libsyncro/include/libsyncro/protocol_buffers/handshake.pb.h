// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: handshake.proto

#ifndef PROTOBUF_handshake_2eproto__INCLUDED
#define PROTOBUF_handshake_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2003000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2003000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace syncro {
namespace pb {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_handshake_2eproto();
void protobuf_AssignDesc_handshake_2eproto();
void protobuf_ShutdownFile_handshake_2eproto();

class HandshakeRequest;
class HandshakeResponse;

// ===================================================================

class HandshakeRequest : public ::google::protobuf::MessageLite {
 public:
  HandshakeRequest();
  virtual ~HandshakeRequest();
  
  HandshakeRequest(const HandshakeRequest& from);
  
  inline HandshakeRequest& operator=(const HandshakeRequest& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const HandshakeRequest& default_instance();
  
  void Swap(HandshakeRequest* other);
  
  // implements Message ----------------------------------------------
  
  HandshakeRequest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const HandshakeRequest& from);
  void MergeFrom(const HandshakeRequest& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string magic = 1;
  inline bool has_magic() const;
  inline void clear_magic();
  static const int kMagicFieldNumber = 1;
  inline const ::std::string& magic() const;
  inline void set_magic(const ::std::string& value);
  inline void set_magic(const char* value);
  inline void set_magic(const char* value, size_t size);
  inline ::std::string* mutable_magic();
  
  // optional int32 client_ver_major = 2;
  inline bool has_client_ver_major() const;
  inline void clear_client_ver_major();
  static const int kClientVerMajorFieldNumber = 2;
  inline ::google::protobuf::int32 client_ver_major() const;
  inline void set_client_ver_major(::google::protobuf::int32 value);
  
  // optional int32 client_ver_minor = 3;
  inline bool has_client_ver_minor() const;
  inline void clear_client_ver_minor();
  static const int kClientVerMinorFieldNumber = 3;
  inline ::google::protobuf::int32 client_ver_minor() const;
  inline void set_client_ver_minor(::google::protobuf::int32 value);
  
  // optional string username = 4;
  inline bool has_username() const;
  inline void clear_username();
  static const int kUsernameFieldNumber = 4;
  inline const ::std::string& username() const;
  inline void set_username(const ::std::string& value);
  inline void set_username(const char* value);
  inline void set_username(const char* value, size_t size);
  inline ::std::string* mutable_username();
  
  // optional string password = 5;
  inline bool has_password() const;
  inline void clear_password();
  static const int kPasswordFieldNumber = 5;
  inline const ::std::string& password() const;
  inline void set_password(const ::std::string& value);
  inline void set_password(const char* value);
  inline void set_password(const char* value, size_t size);
  inline ::std::string* mutable_password();
  
  // @@protoc_insertion_point(class_scope:syncro.pb.HandshakeRequest)
 private:
  mutable int _cached_size_;
  
  ::std::string* magic_;
  static const ::std::string _default_magic_;
  ::google::protobuf::int32 client_ver_major_;
  ::google::protobuf::int32 client_ver_minor_;
  ::std::string* username_;
  static const ::std::string _default_username_;
  ::std::string* password_;
  static const ::std::string _default_password_;
  friend void  protobuf_AddDesc_handshake_2eproto();
  friend void protobuf_AssignDesc_handshake_2eproto();
  friend void protobuf_ShutdownFile_handshake_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(5 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static HandshakeRequest* default_instance_;
};
// -------------------------------------------------------------------

class HandshakeResponse : public ::google::protobuf::MessageLite {
 public:
  HandshakeResponse();
  virtual ~HandshakeResponse();
  
  HandshakeResponse(const HandshakeResponse& from);
  
  inline HandshakeResponse& operator=(const HandshakeResponse& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const HandshakeResponse& default_instance();
  
  void Swap(HandshakeResponse* other);
  
  // implements Message ----------------------------------------------
  
  HandshakeResponse* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const HandshakeResponse& from);
  void MergeFrom(const HandshakeResponse& from);
  void Clear();
  bool IsInitialized() const;
  
  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  
  ::std::string GetTypeName() const;
  
  // nested types ----------------------------------------------------
  
  // accessors -------------------------------------------------------
  
  // required string magic = 1;
  inline bool has_magic() const;
  inline void clear_magic();
  static const int kMagicFieldNumber = 1;
  inline const ::std::string& magic() const;
  inline void set_magic(const ::std::string& value);
  inline void set_magic(const char* value);
  inline void set_magic(const char* value, size_t size);
  inline ::std::string* mutable_magic();
  
  // optional string uuid = 2;
  inline bool has_uuid() const;
  inline void clear_uuid();
  static const int kUuidFieldNumber = 2;
  inline const ::std::string& uuid() const;
  inline void set_uuid(const ::std::string& value);
  inline void set_uuid(const char* value);
  inline void set_uuid(const char* value, size_t size);
  inline ::std::string* mutable_uuid();
  
  // @@protoc_insertion_point(class_scope:syncro.pb.HandshakeResponse)
 private:
  mutable int _cached_size_;
  
  ::std::string* magic_;
  static const ::std::string _default_magic_;
  ::std::string* uuid_;
  static const ::std::string _default_uuid_;
  friend void  protobuf_AddDesc_handshake_2eproto();
  friend void protobuf_AssignDesc_handshake_2eproto();
  friend void protobuf_ShutdownFile_handshake_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(2 + 31) / 32];
  
  // WHY DOES & HAVE LOWER PRECEDENCE THAN != !?
  inline bool _has_bit(int index) const {
    return (_has_bits_[index / 32] & (1u << (index % 32))) != 0;
  }
  inline void _set_bit(int index) {
    _has_bits_[index / 32] |= (1u << (index % 32));
  }
  inline void _clear_bit(int index) {
    _has_bits_[index / 32] &= ~(1u << (index % 32));
  }
  
  void InitAsDefaultInstance();
  static HandshakeResponse* default_instance_;
};
// ===================================================================


// ===================================================================

// HandshakeRequest

// required string magic = 1;
inline bool HandshakeRequest::has_magic() const {
  return _has_bit(0);
}
inline void HandshakeRequest::clear_magic() {
  if (magic_ != &_default_magic_) {
    magic_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& HandshakeRequest::magic() const {
  return *magic_;
}
inline void HandshakeRequest::set_magic(const ::std::string& value) {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  magic_->assign(value);
}
inline void HandshakeRequest::set_magic(const char* value) {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  magic_->assign(value);
}
inline void HandshakeRequest::set_magic(const char* value, size_t size) {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  magic_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HandshakeRequest::mutable_magic() {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  return magic_;
}

// optional int32 client_ver_major = 2;
inline bool HandshakeRequest::has_client_ver_major() const {
  return _has_bit(1);
}
inline void HandshakeRequest::clear_client_ver_major() {
  client_ver_major_ = 0;
  _clear_bit(1);
}
inline ::google::protobuf::int32 HandshakeRequest::client_ver_major() const {
  return client_ver_major_;
}
inline void HandshakeRequest::set_client_ver_major(::google::protobuf::int32 value) {
  _set_bit(1);
  client_ver_major_ = value;
}

// optional int32 client_ver_minor = 3;
inline bool HandshakeRequest::has_client_ver_minor() const {
  return _has_bit(2);
}
inline void HandshakeRequest::clear_client_ver_minor() {
  client_ver_minor_ = 0;
  _clear_bit(2);
}
inline ::google::protobuf::int32 HandshakeRequest::client_ver_minor() const {
  return client_ver_minor_;
}
inline void HandshakeRequest::set_client_ver_minor(::google::protobuf::int32 value) {
  _set_bit(2);
  client_ver_minor_ = value;
}

// optional string username = 4;
inline bool HandshakeRequest::has_username() const {
  return _has_bit(3);
}
inline void HandshakeRequest::clear_username() {
  if (username_ != &_default_username_) {
    username_->clear();
  }
  _clear_bit(3);
}
inline const ::std::string& HandshakeRequest::username() const {
  return *username_;
}
inline void HandshakeRequest::set_username(const ::std::string& value) {
  _set_bit(3);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void HandshakeRequest::set_username(const char* value) {
  _set_bit(3);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(value);
}
inline void HandshakeRequest::set_username(const char* value, size_t size) {
  _set_bit(3);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  username_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HandshakeRequest::mutable_username() {
  _set_bit(3);
  if (username_ == &_default_username_) {
    username_ = new ::std::string;
  }
  return username_;
}

// optional string password = 5;
inline bool HandshakeRequest::has_password() const {
  return _has_bit(4);
}
inline void HandshakeRequest::clear_password() {
  if (password_ != &_default_password_) {
    password_->clear();
  }
  _clear_bit(4);
}
inline const ::std::string& HandshakeRequest::password() const {
  return *password_;
}
inline void HandshakeRequest::set_password(const ::std::string& value) {
  _set_bit(4);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  password_->assign(value);
}
inline void HandshakeRequest::set_password(const char* value) {
  _set_bit(4);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  password_->assign(value);
}
inline void HandshakeRequest::set_password(const char* value, size_t size) {
  _set_bit(4);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  password_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HandshakeRequest::mutable_password() {
  _set_bit(4);
  if (password_ == &_default_password_) {
    password_ = new ::std::string;
  }
  return password_;
}

// -------------------------------------------------------------------

// HandshakeResponse

// required string magic = 1;
inline bool HandshakeResponse::has_magic() const {
  return _has_bit(0);
}
inline void HandshakeResponse::clear_magic() {
  if (magic_ != &_default_magic_) {
    magic_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& HandshakeResponse::magic() const {
  return *magic_;
}
inline void HandshakeResponse::set_magic(const ::std::string& value) {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  magic_->assign(value);
}
inline void HandshakeResponse::set_magic(const char* value) {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  magic_->assign(value);
}
inline void HandshakeResponse::set_magic(const char* value, size_t size) {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  magic_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HandshakeResponse::mutable_magic() {
  _set_bit(0);
  if (magic_ == &_default_magic_) {
    magic_ = new ::std::string;
  }
  return magic_;
}

// optional string uuid = 2;
inline bool HandshakeResponse::has_uuid() const {
  return _has_bit(1);
}
inline void HandshakeResponse::clear_uuid() {
  if (uuid_ != &_default_uuid_) {
    uuid_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& HandshakeResponse::uuid() const {
  return *uuid_;
}
inline void HandshakeResponse::set_uuid(const ::std::string& value) {
  _set_bit(1);
  if (uuid_ == &_default_uuid_) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(value);
}
inline void HandshakeResponse::set_uuid(const char* value) {
  _set_bit(1);
  if (uuid_ == &_default_uuid_) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(value);
}
inline void HandshakeResponse::set_uuid(const char* value, size_t size) {
  _set_bit(1);
  if (uuid_ == &_default_uuid_) {
    uuid_ = new ::std::string;
  }
  uuid_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* HandshakeResponse::mutable_uuid() {
  _set_bit(1);
  if (uuid_ == &_default_uuid_) {
    uuid_ = new ::std::string;
  }
  return uuid_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_handshake_2eproto__INCLUDED