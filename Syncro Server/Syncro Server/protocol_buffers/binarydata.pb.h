// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: binarydata.proto

#ifndef PROTOBUF_binarydata_2eproto__INCLUDED
#define PROTOBUF_binarydata_2eproto__INCLUDED

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
void  protobuf_AddDesc_binarydata_2eproto();
void protobuf_AssignDesc_binarydata_2eproto();
void protobuf_ShutdownFile_binarydata_2eproto();

class BinaryDataRequest;
class BinaryPacketHeader;

enum BinaryPacketHeader_SectionType {
  BinaryPacketHeader_SectionType_START = 0,
  BinaryPacketHeader_SectionType_MIDDLE = 1,
  BinaryPacketHeader_SectionType_END = 2
};
bool BinaryPacketHeader_SectionType_IsValid(int value);
const BinaryPacketHeader_SectionType BinaryPacketHeader_SectionType_SectionType_MIN = BinaryPacketHeader_SectionType_START;
const BinaryPacketHeader_SectionType BinaryPacketHeader_SectionType_SectionType_MAX = BinaryPacketHeader_SectionType_END;
const int BinaryPacketHeader_SectionType_SectionType_ARRAYSIZE = BinaryPacketHeader_SectionType_SectionType_MAX + 1;

// ===================================================================

class BinaryDataRequest : public ::google::protobuf::MessageLite {
 public:
  BinaryDataRequest();
  virtual ~BinaryDataRequest();
  
  BinaryDataRequest(const BinaryDataRequest& from);
  
  inline BinaryDataRequest& operator=(const BinaryDataRequest& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const BinaryDataRequest& default_instance();
  
  void Swap(BinaryDataRequest* other);
  
  // implements Message ----------------------------------------------
  
  BinaryDataRequest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const BinaryDataRequest& from);
  void MergeFrom(const BinaryDataRequest& from);
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
  
  // optional string file_name = 1;
  inline bool has_file_name() const;
  inline void clear_file_name();
  static const int kFileNameFieldNumber = 1;
  inline const ::std::string& file_name() const;
  inline void set_file_name(const ::std::string& value);
  inline void set_file_name(const char* value);
  inline void set_file_name(const char* value, size_t size);
  inline ::std::string* mutable_file_name();
  
  // required int32 folder_id = 2;
  inline bool has_folder_id() const;
  inline void clear_folder_id();
  static const int kFolderIdFieldNumber = 2;
  inline ::google::protobuf::int32 folder_id() const;
  inline void set_folder_id(::google::protobuf::int32 value);
  
  // optional int32 recv_buffer_size = 3;
  inline bool has_recv_buffer_size() const;
  inline void clear_recv_buffer_size();
  static const int kRecvBufferSizeFieldNumber = 3;
  inline ::google::protobuf::int32 recv_buffer_size() const;
  inline void set_recv_buffer_size(::google::protobuf::int32 value);
  
  // @@protoc_insertion_point(class_scope:syncro.pb.BinaryDataRequest)
 private:
  mutable int _cached_size_;
  
  ::std::string* file_name_;
  static const ::std::string _default_file_name_;
  ::google::protobuf::int32 folder_id_;
  ::google::protobuf::int32 recv_buffer_size_;
  friend void  protobuf_AddDesc_binarydata_2eproto();
  friend void protobuf_AssignDesc_binarydata_2eproto();
  friend void protobuf_ShutdownFile_binarydata_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(3 + 31) / 32];
  
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
  static BinaryDataRequest* default_instance_;
};
// -------------------------------------------------------------------

class BinaryPacketHeader : public ::google::protobuf::MessageLite {
 public:
  BinaryPacketHeader();
  virtual ~BinaryPacketHeader();
  
  BinaryPacketHeader(const BinaryPacketHeader& from);
  
  inline BinaryPacketHeader& operator=(const BinaryPacketHeader& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const BinaryPacketHeader& default_instance();
  
  void Swap(BinaryPacketHeader* other);
  
  // implements Message ----------------------------------------------
  
  BinaryPacketHeader* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const BinaryPacketHeader& from);
  void MergeFrom(const BinaryPacketHeader& from);
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
  
  typedef BinaryPacketHeader_SectionType SectionType;
  static const SectionType START = BinaryPacketHeader_SectionType_START;
  static const SectionType MIDDLE = BinaryPacketHeader_SectionType_MIDDLE;
  static const SectionType END = BinaryPacketHeader_SectionType_END;
  static inline bool SectionType_IsValid(int value) {
    return BinaryPacketHeader_SectionType_IsValid(value);
  }
  static const SectionType SectionType_MIN =
    BinaryPacketHeader_SectionType_SectionType_MIN;
  static const SectionType SectionType_MAX =
    BinaryPacketHeader_SectionType_SectionType_MAX;
  static const int SectionType_ARRAYSIZE =
    BinaryPacketHeader_SectionType_SectionType_ARRAYSIZE;
  
  // accessors -------------------------------------------------------
  
  // optional int32 file_size = 1;
  inline bool has_file_size() const;
  inline void clear_file_size();
  static const int kFileSizeFieldNumber = 1;
  inline ::google::protobuf::int32 file_size() const;
  inline void set_file_size(::google::protobuf::int32 value);
  
  // optional int32 file_offset = 2;
  inline bool has_file_offset() const;
  inline void clear_file_offset();
  static const int kFileOffsetFieldNumber = 2;
  inline ::google::protobuf::int32 file_offset() const;
  inline void set_file_offset(::google::protobuf::int32 value);
  
  // optional int32 hash_size = 3;
  inline bool has_hash_size() const;
  inline void clear_hash_size();
  static const int kHashSizeFieldNumber = 3;
  inline ::google::protobuf::int32 hash_size() const;
  inline void set_hash_size(::google::protobuf::int32 value);
  
  // optional .syncro.pb.BinaryPacketHeader.SectionType binary_packet_type = 4;
  inline bool has_binary_packet_type() const;
  inline void clear_binary_packet_type();
  static const int kBinaryPacketTypeFieldNumber = 4;
  inline ::syncro::pb::BinaryPacketHeader_SectionType binary_packet_type() const;
  inline void set_binary_packet_type(::syncro::pb::BinaryPacketHeader_SectionType value);
  
  // @@protoc_insertion_point(class_scope:syncro.pb.BinaryPacketHeader)
 private:
  mutable int _cached_size_;
  
  ::google::protobuf::int32 file_size_;
  ::google::protobuf::int32 file_offset_;
  ::google::protobuf::int32 hash_size_;
  int binary_packet_type_;
  friend void  protobuf_AddDesc_binarydata_2eproto();
  friend void protobuf_AssignDesc_binarydata_2eproto();
  friend void protobuf_ShutdownFile_binarydata_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(4 + 31) / 32];
  
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
  static BinaryPacketHeader* default_instance_;
};
// ===================================================================


// ===================================================================

// BinaryDataRequest

// optional string file_name = 1;
inline bool BinaryDataRequest::has_file_name() const {
  return _has_bit(0);
}
inline void BinaryDataRequest::clear_file_name() {
  if (file_name_ != &_default_file_name_) {
    file_name_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& BinaryDataRequest::file_name() const {
  return *file_name_;
}
inline void BinaryDataRequest::set_file_name(const ::std::string& value) {
  _set_bit(0);
  if (file_name_ == &_default_file_name_) {
    file_name_ = new ::std::string;
  }
  file_name_->assign(value);
}
inline void BinaryDataRequest::set_file_name(const char* value) {
  _set_bit(0);
  if (file_name_ == &_default_file_name_) {
    file_name_ = new ::std::string;
  }
  file_name_->assign(value);
}
inline void BinaryDataRequest::set_file_name(const char* value, size_t size) {
  _set_bit(0);
  if (file_name_ == &_default_file_name_) {
    file_name_ = new ::std::string;
  }
  file_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* BinaryDataRequest::mutable_file_name() {
  _set_bit(0);
  if (file_name_ == &_default_file_name_) {
    file_name_ = new ::std::string;
  }
  return file_name_;
}

// required int32 folder_id = 2;
inline bool BinaryDataRequest::has_folder_id() const {
  return _has_bit(1);
}
inline void BinaryDataRequest::clear_folder_id() {
  folder_id_ = 0;
  _clear_bit(1);
}
inline ::google::protobuf::int32 BinaryDataRequest::folder_id() const {
  return folder_id_;
}
inline void BinaryDataRequest::set_folder_id(::google::protobuf::int32 value) {
  _set_bit(1);
  folder_id_ = value;
}

// optional int32 recv_buffer_size = 3;
inline bool BinaryDataRequest::has_recv_buffer_size() const {
  return _has_bit(2);
}
inline void BinaryDataRequest::clear_recv_buffer_size() {
  recv_buffer_size_ = 0;
  _clear_bit(2);
}
inline ::google::protobuf::int32 BinaryDataRequest::recv_buffer_size() const {
  return recv_buffer_size_;
}
inline void BinaryDataRequest::set_recv_buffer_size(::google::protobuf::int32 value) {
  _set_bit(2);
  recv_buffer_size_ = value;
}

// -------------------------------------------------------------------

// BinaryPacketHeader

// optional int32 file_size = 1;
inline bool BinaryPacketHeader::has_file_size() const {
  return _has_bit(0);
}
inline void BinaryPacketHeader::clear_file_size() {
  file_size_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 BinaryPacketHeader::file_size() const {
  return file_size_;
}
inline void BinaryPacketHeader::set_file_size(::google::protobuf::int32 value) {
  _set_bit(0);
  file_size_ = value;
}

// optional int32 file_offset = 2;
inline bool BinaryPacketHeader::has_file_offset() const {
  return _has_bit(1);
}
inline void BinaryPacketHeader::clear_file_offset() {
  file_offset_ = 0;
  _clear_bit(1);
}
inline ::google::protobuf::int32 BinaryPacketHeader::file_offset() const {
  return file_offset_;
}
inline void BinaryPacketHeader::set_file_offset(::google::protobuf::int32 value) {
  _set_bit(1);
  file_offset_ = value;
}

// optional int32 hash_size = 3;
inline bool BinaryPacketHeader::has_hash_size() const {
  return _has_bit(2);
}
inline void BinaryPacketHeader::clear_hash_size() {
  hash_size_ = 0;
  _clear_bit(2);
}
inline ::google::protobuf::int32 BinaryPacketHeader::hash_size() const {
  return hash_size_;
}
inline void BinaryPacketHeader::set_hash_size(::google::protobuf::int32 value) {
  _set_bit(2);
  hash_size_ = value;
}

// optional .syncro.pb.BinaryPacketHeader.SectionType binary_packet_type = 4;
inline bool BinaryPacketHeader::has_binary_packet_type() const {
  return _has_bit(3);
}
inline void BinaryPacketHeader::clear_binary_packet_type() {
  binary_packet_type_ = 0;
  _clear_bit(3);
}
inline ::syncro::pb::BinaryPacketHeader_SectionType BinaryPacketHeader::binary_packet_type() const {
  return static_cast< ::syncro::pb::BinaryPacketHeader_SectionType >(binary_packet_type_);
}
inline void BinaryPacketHeader::set_binary_packet_type(::syncro::pb::BinaryPacketHeader_SectionType value) {
  GOOGLE_DCHECK(::syncro::pb::BinaryPacketHeader_SectionType_IsValid(value));
  _set_bit(3);
  binary_packet_type_ = value;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_binarydata_2eproto__INCLUDED
