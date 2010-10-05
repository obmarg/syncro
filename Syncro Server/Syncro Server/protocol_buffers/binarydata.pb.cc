// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "binarydata.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace syncro {
namespace pb {

void protobuf_ShutdownFile_binarydata_2eproto() {
  delete BinaryDataRequest::default_instance_;
  delete BinaryPacketHeader::default_instance_;
}

void protobuf_AddDesc_binarydata_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  BinaryDataRequest::default_instance_ = new BinaryDataRequest();
  BinaryPacketHeader::default_instance_ = new BinaryPacketHeader();
  BinaryDataRequest::default_instance_->InitAsDefaultInstance();
  BinaryPacketHeader::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_binarydata_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_binarydata_2eproto {
  StaticDescriptorInitializer_binarydata_2eproto() {
    protobuf_AddDesc_binarydata_2eproto();
  }
} static_descriptor_initializer_binarydata_2eproto_;


// ===================================================================

const ::std::string BinaryDataRequest::_default_file_name_;
#ifndef _MSC_VER
const int BinaryDataRequest::kFileNameFieldNumber;
const int BinaryDataRequest::kFolderIdFieldNumber;
#endif  // !_MSC_VER

BinaryDataRequest::BinaryDataRequest()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void BinaryDataRequest::InitAsDefaultInstance() {
}

BinaryDataRequest::BinaryDataRequest(const BinaryDataRequest& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void BinaryDataRequest::SharedCtor() {
  _cached_size_ = 0;
  file_name_ = const_cast< ::std::string*>(&_default_file_name_);
  folder_id_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BinaryDataRequest::~BinaryDataRequest() {
  SharedDtor();
}

void BinaryDataRequest::SharedDtor() {
  if (file_name_ != &_default_file_name_) {
    delete file_name_;
  }
  if (this != default_instance_) {
  }
}

void BinaryDataRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const BinaryDataRequest& BinaryDataRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_binarydata_2eproto();  return *default_instance_;
}

BinaryDataRequest* BinaryDataRequest::default_instance_ = NULL;

BinaryDataRequest* BinaryDataRequest::New() const {
  return new BinaryDataRequest;
}

void BinaryDataRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (file_name_ != &_default_file_name_) {
        file_name_->clear();
      }
    }
    folder_id_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool BinaryDataRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional string file_name = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_file_name()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_folder_id;
        break;
      }
      
      // required int32 folder_id = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_folder_id:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &folder_id_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void BinaryDataRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional string file_name = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->file_name(), output);
  }
  
  // required int32 folder_id = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->folder_id(), output);
  }
  
}

int BinaryDataRequest::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional string file_name = 1;
    if (has_file_name()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->file_name());
    }
    
    // required int32 folder_id = 2;
    if (has_folder_id()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->folder_id());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BinaryDataRequest::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const BinaryDataRequest*>(&from));
}

void BinaryDataRequest::MergeFrom(const BinaryDataRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_file_name(from.file_name());
    }
    if (from._has_bit(1)) {
      set_folder_id(from.folder_id());
    }
  }
}

void BinaryDataRequest::CopyFrom(const BinaryDataRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BinaryDataRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000002) != 0x00000002) return false;
  
  return true;
}

void BinaryDataRequest::Swap(BinaryDataRequest* other) {
  if (other != this) {
    std::swap(file_name_, other->file_name_);
    std::swap(folder_id_, other->folder_id_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string BinaryDataRequest::GetTypeName() const {
  return "syncro.pb.BinaryDataRequest";
}


// ===================================================================

bool BinaryPacketHeader_SectionType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}

#ifndef _MSC_VER
const BinaryPacketHeader_SectionType BinaryPacketHeader::START;
const BinaryPacketHeader_SectionType BinaryPacketHeader::MIDDLE;
const BinaryPacketHeader_SectionType BinaryPacketHeader::END;
const BinaryPacketHeader_SectionType BinaryPacketHeader::SectionType_MIN;
const BinaryPacketHeader_SectionType BinaryPacketHeader::SectionType_MAX;
const int BinaryPacketHeader::SectionType_ARRAYSIZE;
#endif  // _MSC_VER
#ifndef _MSC_VER
const int BinaryPacketHeader::kFileSizeFieldNumber;
const int BinaryPacketHeader::kFileOffsetFieldNumber;
const int BinaryPacketHeader::kHashSizeFieldNumber;
const int BinaryPacketHeader::kBinaryPacketTypeFieldNumber;
#endif  // !_MSC_VER

BinaryPacketHeader::BinaryPacketHeader()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void BinaryPacketHeader::InitAsDefaultInstance() {
}

BinaryPacketHeader::BinaryPacketHeader(const BinaryPacketHeader& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void BinaryPacketHeader::SharedCtor() {
  _cached_size_ = 0;
  file_size_ = 0;
  file_offset_ = 0;
  hash_size_ = 0;
  binary_packet_type_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

BinaryPacketHeader::~BinaryPacketHeader() {
  SharedDtor();
}

void BinaryPacketHeader::SharedDtor() {
  if (this != default_instance_) {
  }
}

void BinaryPacketHeader::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const BinaryPacketHeader& BinaryPacketHeader::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_binarydata_2eproto();  return *default_instance_;
}

BinaryPacketHeader* BinaryPacketHeader::default_instance_ = NULL;

BinaryPacketHeader* BinaryPacketHeader::New() const {
  return new BinaryPacketHeader;
}

void BinaryPacketHeader::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    file_size_ = 0;
    file_offset_ = 0;
    hash_size_ = 0;
    binary_packet_type_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool BinaryPacketHeader::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // optional int32 file_size = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &file_size_)));
          _set_bit(0);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_file_offset;
        break;
      }
      
      // optional int32 file_offset = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_file_offset:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &file_offset_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_hash_size;
        break;
      }
      
      // optional int32 hash_size = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_hash_size:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &hash_size_)));
          _set_bit(2);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(32)) goto parse_binary_packet_type;
        break;
      }
      
      // optional .syncro.pb.BinaryPacketHeader.SectionType binary_packet_type = 4;
      case 4: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_binary_packet_type:
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::syncro::pb::BinaryPacketHeader_SectionType_IsValid(value)) {
            set_binary_packet_type(static_cast< ::syncro::pb::BinaryPacketHeader_SectionType >(value));
          }
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectAtEnd()) return true;
        break;
      }
      
      default: {
      handle_uninterpreted:
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_END_GROUP) {
          return true;
        }
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
        break;
      }
    }
  }
  return true;
#undef DO_
}

void BinaryPacketHeader::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // optional int32 file_size = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->file_size(), output);
  }
  
  // optional int32 file_offset = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->file_offset(), output);
  }
  
  // optional int32 hash_size = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->hash_size(), output);
  }
  
  // optional .syncro.pb.BinaryPacketHeader.SectionType binary_packet_type = 4;
  if (_has_bit(3)) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      4, this->binary_packet_type(), output);
  }
  
}

int BinaryPacketHeader::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // optional int32 file_size = 1;
    if (has_file_size()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->file_size());
    }
    
    // optional int32 file_offset = 2;
    if (has_file_offset()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->file_offset());
    }
    
    // optional int32 hash_size = 3;
    if (has_hash_size()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->hash_size());
    }
    
    // optional .syncro.pb.BinaryPacketHeader.SectionType binary_packet_type = 4;
    if (has_binary_packet_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::EnumSize(this->binary_packet_type());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BinaryPacketHeader::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const BinaryPacketHeader*>(&from));
}

void BinaryPacketHeader::MergeFrom(const BinaryPacketHeader& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_file_size(from.file_size());
    }
    if (from._has_bit(1)) {
      set_file_offset(from.file_offset());
    }
    if (from._has_bit(2)) {
      set_hash_size(from.hash_size());
    }
    if (from._has_bit(3)) {
      set_binary_packet_type(from.binary_packet_type());
    }
  }
}

void BinaryPacketHeader::CopyFrom(const BinaryPacketHeader& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BinaryPacketHeader::IsInitialized() const {
  
  return true;
}

void BinaryPacketHeader::Swap(BinaryPacketHeader* other) {
  if (other != this) {
    std::swap(file_size_, other->file_size_);
    std::swap(file_offset_, other->file_offset_);
    std::swap(hash_size_, other->hash_size_);
    std::swap(binary_packet_type_, other->binary_packet_type_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string BinaryPacketHeader::GetTypeName() const {
  return "syncro.pb.BinaryPacketHeader";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)
