// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: folders.proto

#ifndef PROTOBUF_folders_2eproto__INCLUDED
#define PROTOBUF_folders_2eproto__INCLUDED

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
void  protobuf_AddDesc_folders_2eproto();
void protobuf_AssignDesc_folders_2eproto();
void protobuf_ShutdownFile_folders_2eproto();

class FolderListRequest;
class FolderInfo;
class FolderList;

// ===================================================================

class FolderListRequest : public ::google::protobuf::MessageLite {
 public:
  FolderListRequest();
  virtual ~FolderListRequest();
  
  FolderListRequest(const FolderListRequest& from);
  
  inline FolderListRequest& operator=(const FolderListRequest& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const FolderListRequest& default_instance();
  
  void Swap(FolderListRequest* other);
  
  // implements Message ----------------------------------------------
  
  FolderListRequest* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const FolderListRequest& from);
  void MergeFrom(const FolderListRequest& from);
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
  
  // optional string search_string = 1;
  inline bool has_search_string() const;
  inline void clear_search_string();
  static const int kSearchStringFieldNumber = 1;
  inline const ::std::string& search_string() const;
  inline void set_search_string(const ::std::string& value);
  inline void set_search_string(const char* value);
  inline void set_search_string(const char* value, size_t size);
  inline ::std::string* mutable_search_string();
  
  // optional bool writable = 2;
  inline bool has_writable() const;
  inline void clear_writable();
  static const int kWritableFieldNumber = 2;
  inline bool writable() const;
  inline void set_writable(bool value);
  
  // optional bool readable = 3;
  inline bool has_readable() const;
  inline void clear_readable();
  static const int kReadableFieldNumber = 3;
  inline bool readable() const;
  inline void set_readable(bool value);
  
  // @@protoc_insertion_point(class_scope:syncro.pb.FolderListRequest)
 private:
  mutable int _cached_size_;
  
  ::std::string* search_string_;
  static const ::std::string _default_search_string_;
  bool writable_;
  bool readable_;
  friend void  protobuf_AddDesc_folders_2eproto();
  friend void protobuf_AssignDesc_folders_2eproto();
  friend void protobuf_ShutdownFile_folders_2eproto();
  
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
  static FolderListRequest* default_instance_;
};
// -------------------------------------------------------------------

class FolderInfo : public ::google::protobuf::MessageLite {
 public:
  FolderInfo();
  virtual ~FolderInfo();
  
  FolderInfo(const FolderInfo& from);
  
  inline FolderInfo& operator=(const FolderInfo& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const FolderInfo& default_instance();
  
  void Swap(FolderInfo* other);
  
  // implements Message ----------------------------------------------
  
  FolderInfo* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const FolderInfo& from);
  void MergeFrom(const FolderInfo& from);
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
  
  // required int32 folder_id = 1;
  inline bool has_folder_id() const;
  inline void clear_folder_id();
  static const int kFolderIdFieldNumber = 1;
  inline ::google::protobuf::int32 folder_id() const;
  inline void set_folder_id(::google::protobuf::int32 value);
  
  // optional string folder_name = 2;
  inline bool has_folder_name() const;
  inline void clear_folder_name();
  static const int kFolderNameFieldNumber = 2;
  inline const ::std::string& folder_name() const;
  inline void set_folder_name(const ::std::string& value);
  inline void set_folder_name(const char* value);
  inline void set_folder_name(const char* value, size_t size);
  inline ::std::string* mutable_folder_name();
  
  // optional bool can_write = 3;
  inline bool has_can_write() const;
  inline void clear_can_write();
  static const int kCanWriteFieldNumber = 3;
  inline bool can_write() const;
  inline void set_can_write(bool value);
  
  // optional bool can_read = 4;
  inline bool has_can_read() const;
  inline void clear_can_read();
  static const int kCanReadFieldNumber = 4;
  inline bool can_read() const;
  inline void set_can_read(bool value);
  
  // @@protoc_insertion_point(class_scope:syncro.pb.FolderInfo)
 private:
  mutable int _cached_size_;
  
  ::google::protobuf::int32 folder_id_;
  ::std::string* folder_name_;
  static const ::std::string _default_folder_name_;
  bool can_write_;
  bool can_read_;
  friend void  protobuf_AddDesc_folders_2eproto();
  friend void protobuf_AssignDesc_folders_2eproto();
  friend void protobuf_ShutdownFile_folders_2eproto();
  
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
  static FolderInfo* default_instance_;
};
// -------------------------------------------------------------------

class FolderList : public ::google::protobuf::MessageLite {
 public:
  FolderList();
  virtual ~FolderList();
  
  FolderList(const FolderList& from);
  
  inline FolderList& operator=(const FolderList& from) {
    CopyFrom(from);
    return *this;
  }
  
  static const FolderList& default_instance();
  
  void Swap(FolderList* other);
  
  // implements Message ----------------------------------------------
  
  FolderList* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const FolderList& from);
  void MergeFrom(const FolderList& from);
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
  
  // repeated .syncro.pb.FolderInfo folders = 1;
  inline int folders_size() const;
  inline void clear_folders();
  static const int kFoldersFieldNumber = 1;
  inline const ::syncro::pb::FolderInfo& folders(int index) const;
  inline ::syncro::pb::FolderInfo* mutable_folders(int index);
  inline ::syncro::pb::FolderInfo* add_folders();
  inline const ::google::protobuf::RepeatedPtrField< ::syncro::pb::FolderInfo >&
      folders() const;
  inline ::google::protobuf::RepeatedPtrField< ::syncro::pb::FolderInfo >*
      mutable_folders();
  
  // @@protoc_insertion_point(class_scope:syncro.pb.FolderList)
 private:
  mutable int _cached_size_;
  
  ::google::protobuf::RepeatedPtrField< ::syncro::pb::FolderInfo > folders_;
  friend void  protobuf_AddDesc_folders_2eproto();
  friend void protobuf_AssignDesc_folders_2eproto();
  friend void protobuf_ShutdownFile_folders_2eproto();
  
  ::google::protobuf::uint32 _has_bits_[(1 + 31) / 32];
  
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
  static FolderList* default_instance_;
};
// ===================================================================


// ===================================================================

// FolderListRequest

// optional string search_string = 1;
inline bool FolderListRequest::has_search_string() const {
  return _has_bit(0);
}
inline void FolderListRequest::clear_search_string() {
  if (search_string_ != &_default_search_string_) {
    search_string_->clear();
  }
  _clear_bit(0);
}
inline const ::std::string& FolderListRequest::search_string() const {
  return *search_string_;
}
inline void FolderListRequest::set_search_string(const ::std::string& value) {
  _set_bit(0);
  if (search_string_ == &_default_search_string_) {
    search_string_ = new ::std::string;
  }
  search_string_->assign(value);
}
inline void FolderListRequest::set_search_string(const char* value) {
  _set_bit(0);
  if (search_string_ == &_default_search_string_) {
    search_string_ = new ::std::string;
  }
  search_string_->assign(value);
}
inline void FolderListRequest::set_search_string(const char* value, size_t size) {
  _set_bit(0);
  if (search_string_ == &_default_search_string_) {
    search_string_ = new ::std::string;
  }
  search_string_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FolderListRequest::mutable_search_string() {
  _set_bit(0);
  if (search_string_ == &_default_search_string_) {
    search_string_ = new ::std::string;
  }
  return search_string_;
}

// optional bool writable = 2;
inline bool FolderListRequest::has_writable() const {
  return _has_bit(1);
}
inline void FolderListRequest::clear_writable() {
  writable_ = false;
  _clear_bit(1);
}
inline bool FolderListRequest::writable() const {
  return writable_;
}
inline void FolderListRequest::set_writable(bool value) {
  _set_bit(1);
  writable_ = value;
}

// optional bool readable = 3;
inline bool FolderListRequest::has_readable() const {
  return _has_bit(2);
}
inline void FolderListRequest::clear_readable() {
  readable_ = false;
  _clear_bit(2);
}
inline bool FolderListRequest::readable() const {
  return readable_;
}
inline void FolderListRequest::set_readable(bool value) {
  _set_bit(2);
  readable_ = value;
}

// -------------------------------------------------------------------

// FolderInfo

// required int32 folder_id = 1;
inline bool FolderInfo::has_folder_id() const {
  return _has_bit(0);
}
inline void FolderInfo::clear_folder_id() {
  folder_id_ = 0;
  _clear_bit(0);
}
inline ::google::protobuf::int32 FolderInfo::folder_id() const {
  return folder_id_;
}
inline void FolderInfo::set_folder_id(::google::protobuf::int32 value) {
  _set_bit(0);
  folder_id_ = value;
}

// optional string folder_name = 2;
inline bool FolderInfo::has_folder_name() const {
  return _has_bit(1);
}
inline void FolderInfo::clear_folder_name() {
  if (folder_name_ != &_default_folder_name_) {
    folder_name_->clear();
  }
  _clear_bit(1);
}
inline const ::std::string& FolderInfo::folder_name() const {
  return *folder_name_;
}
inline void FolderInfo::set_folder_name(const ::std::string& value) {
  _set_bit(1);
  if (folder_name_ == &_default_folder_name_) {
    folder_name_ = new ::std::string;
  }
  folder_name_->assign(value);
}
inline void FolderInfo::set_folder_name(const char* value) {
  _set_bit(1);
  if (folder_name_ == &_default_folder_name_) {
    folder_name_ = new ::std::string;
  }
  folder_name_->assign(value);
}
inline void FolderInfo::set_folder_name(const char* value, size_t size) {
  _set_bit(1);
  if (folder_name_ == &_default_folder_name_) {
    folder_name_ = new ::std::string;
  }
  folder_name_->assign(reinterpret_cast<const char*>(value), size);
}
inline ::std::string* FolderInfo::mutable_folder_name() {
  _set_bit(1);
  if (folder_name_ == &_default_folder_name_) {
    folder_name_ = new ::std::string;
  }
  return folder_name_;
}

// optional bool can_write = 3;
inline bool FolderInfo::has_can_write() const {
  return _has_bit(2);
}
inline void FolderInfo::clear_can_write() {
  can_write_ = false;
  _clear_bit(2);
}
inline bool FolderInfo::can_write() const {
  return can_write_;
}
inline void FolderInfo::set_can_write(bool value) {
  _set_bit(2);
  can_write_ = value;
}

// optional bool can_read = 4;
inline bool FolderInfo::has_can_read() const {
  return _has_bit(3);
}
inline void FolderInfo::clear_can_read() {
  can_read_ = false;
  _clear_bit(3);
}
inline bool FolderInfo::can_read() const {
  return can_read_;
}
inline void FolderInfo::set_can_read(bool value) {
  _set_bit(3);
  can_read_ = value;
}

// -------------------------------------------------------------------

// FolderList

// repeated .syncro.pb.FolderInfo folders = 1;
inline int FolderList::folders_size() const {
  return folders_.size();
}
inline void FolderList::clear_folders() {
  folders_.Clear();
}
inline const ::syncro::pb::FolderInfo& FolderList::folders(int index) const {
  return folders_.Get(index);
}
inline ::syncro::pb::FolderInfo* FolderList::mutable_folders(int index) {
  return folders_.Mutable(index);
}
inline ::syncro::pb::FolderInfo* FolderList::add_folders() {
  return folders_.Add();
}
inline const ::google::protobuf::RepeatedPtrField< ::syncro::pb::FolderInfo >&
FolderList::folders() const {
  return folders_;
}
inline ::google::protobuf::RepeatedPtrField< ::syncro::pb::FolderInfo >*
FolderList::mutable_folders() {
  return &folders_;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_folders_2eproto__INCLUDED
