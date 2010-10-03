// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "binarydata.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace syncro {
namespace pb {

namespace {

const ::google::protobuf::Descriptor* BinaryDataRequest_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BinaryDataRequest_reflection_ = NULL;
const ::google::protobuf::Descriptor* BinaryPacketHeader_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  BinaryPacketHeader_reflection_ = NULL;
const ::google::protobuf::EnumDescriptor* BinaryPacketHeader_SectionType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_binarydata_2eproto() {
  protobuf_AddDesc_binarydata_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "binarydata.proto");
  GOOGLE_CHECK(file != NULL);
  BinaryDataRequest_descriptor_ = file->message_type(0);
  static const int BinaryDataRequest_offsets_[1] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryDataRequest, file_name_),
  };
  BinaryDataRequest_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BinaryDataRequest_descriptor_,
      BinaryDataRequest::default_instance_,
      BinaryDataRequest_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryDataRequest, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryDataRequest, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BinaryDataRequest));
  BinaryPacketHeader_descriptor_ = file->message_type(1);
  static const int BinaryPacketHeader_offsets_[4] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryPacketHeader, file_size_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryPacketHeader, file_offset_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryPacketHeader, hash_size_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryPacketHeader, binary_packet_type_),
  };
  BinaryPacketHeader_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      BinaryPacketHeader_descriptor_,
      BinaryPacketHeader::default_instance_,
      BinaryPacketHeader_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryPacketHeader, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(BinaryPacketHeader, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(BinaryPacketHeader));
  BinaryPacketHeader_SectionType_descriptor_ = BinaryPacketHeader_descriptor_->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_binarydata_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BinaryDataRequest_descriptor_, &BinaryDataRequest::default_instance());
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    BinaryPacketHeader_descriptor_, &BinaryPacketHeader::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_binarydata_2eproto() {
  delete BinaryDataRequest::default_instance_;
  delete BinaryDataRequest_reflection_;
  delete BinaryPacketHeader::default_instance_;
  delete BinaryPacketHeader_reflection_;
}

void protobuf_AddDesc_binarydata_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\020binarydata.proto\022\tsyncro.pb\"&\n\021BinaryD"
    "ataRequest\022\021\n\tfile_name\030\001 \001(\t\"\305\001\n\022Binary"
    "PacketHeader\022\021\n\tfile_size\030\001 \001(\005\022\023\n\013file_"
    "offset\030\002 \001(\005\022\021\n\thash_size\030\003 \001(\005\022E\n\022binar"
    "y_packet_type\030\004 \001(\0162).syncro.pb.BinaryPa"
    "cketHeader.SectionType\"-\n\013SectionType\022\t\n"
    "\005START\020\000\022\n\n\006MIDDLE\020\001\022\007\n\003END\020\002B\030\n\026uk.me.g"
    "rambo.syncro.pb", 295);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "binarydata.proto", &protobuf_RegisterTypes);
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
#endif  // !_MSC_VER

BinaryDataRequest::BinaryDataRequest()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BinaryDataRequest::InitAsDefaultInstance() {
}

BinaryDataRequest::BinaryDataRequest(const BinaryDataRequest& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void BinaryDataRequest::SharedCtor() {
  _cached_size_ = 0;
  file_name_ = const_cast< ::std::string*>(&_default_file_name_);
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
const ::google::protobuf::Descriptor* BinaryDataRequest::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BinaryDataRequest_descriptor_;
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
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
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
          ::google::protobuf::internal::WireFormat::VerifyUTF8String(
            this->file_name().data(), this->file_name().length(),
            ::google::protobuf::internal::WireFormat::PARSE);
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
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
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
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->file_name().data(), this->file_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->file_name(), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BinaryDataRequest::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional string file_name = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8String(
      this->file_name().data(), this->file_name().length(),
      ::google::protobuf::internal::WireFormat::SERIALIZE);
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        1, this->file_name(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
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
    
  }
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BinaryDataRequest::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BinaryDataRequest* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BinaryDataRequest*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void BinaryDataRequest::MergeFrom(const BinaryDataRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_file_name(from.file_name());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BinaryDataRequest::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void BinaryDataRequest::CopyFrom(const BinaryDataRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool BinaryDataRequest::IsInitialized() const {
  
  return true;
}

void BinaryDataRequest::Swap(BinaryDataRequest* other) {
  if (other != this) {
    std::swap(file_name_, other->file_name_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BinaryDataRequest::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BinaryDataRequest_descriptor_;
  metadata.reflection = BinaryDataRequest_reflection_;
  return metadata;
}


// ===================================================================

const ::google::protobuf::EnumDescriptor* BinaryPacketHeader_SectionType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BinaryPacketHeader_SectionType_descriptor_;
}
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
  : ::google::protobuf::Message() {
  SharedCtor();
}

void BinaryPacketHeader::InitAsDefaultInstance() {
}

BinaryPacketHeader::BinaryPacketHeader(const BinaryPacketHeader& from)
  : ::google::protobuf::Message() {
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
const ::google::protobuf::Descriptor* BinaryPacketHeader::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return BinaryPacketHeader_descriptor_;
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
  mutable_unknown_fields()->Clear();
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
          } else {
            mutable_unknown_fields()->AddVarint(4, value);
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
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, mutable_unknown_fields()));
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
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* BinaryPacketHeader::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // optional int32 file_size = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->file_size(), target);
  }
  
  // optional int32 file_offset = 2;
  if (_has_bit(1)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(2, this->file_offset(), target);
  }
  
  // optional int32 hash_size = 3;
  if (_has_bit(2)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(3, this->hash_size(), target);
  }
  
  // optional .syncro.pb.BinaryPacketHeader.SectionType binary_packet_type = 4;
  if (_has_bit(3)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      4, this->binary_packet_type(), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
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
  if (!unknown_fields().empty()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        unknown_fields());
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void BinaryPacketHeader::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const BinaryPacketHeader* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const BinaryPacketHeader*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
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
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void BinaryPacketHeader::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
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
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata BinaryPacketHeader::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = BinaryPacketHeader_descriptor_;
  metadata.reflection = BinaryPacketHeader_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)
