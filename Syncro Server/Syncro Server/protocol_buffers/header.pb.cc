// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "header.pb.h"
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

const ::google::protobuf::Descriptor* PacketHeader_descriptor_ = NULL;
const ::google::protobuf::internal::GeneratedMessageReflection*
  PacketHeader_reflection_ = NULL;

}  // namespace


void protobuf_AssignDesc_header_2eproto() {
  protobuf_AddDesc_header_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "header.proto");
  GOOGLE_CHECK(file != NULL);
  PacketHeader_descriptor_ = file->message_type(0);
  static const int PacketHeader_offsets_[2] = {
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PacketHeader, packet_type_),
    GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PacketHeader, subpacket_sizes_),
  };
  PacketHeader_reflection_ =
    new ::google::protobuf::internal::GeneratedMessageReflection(
      PacketHeader_descriptor_,
      PacketHeader::default_instance_,
      PacketHeader_offsets_,
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PacketHeader, _has_bits_[0]),
      GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(PacketHeader, _unknown_fields_),
      -1,
      ::google::protobuf::DescriptorPool::generated_pool(),
      ::google::protobuf::MessageFactory::generated_factory(),
      sizeof(PacketHeader));
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_header_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedMessage(
    PacketHeader_descriptor_, &PacketHeader::default_instance());
}

}  // namespace

void protobuf_ShutdownFile_header_2eproto() {
  delete PacketHeader::default_instance_;
  delete PacketHeader_reflection_;
}

void protobuf_AddDesc_header_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\014header.proto\022\tsyncro.pb\"<\n\014PacketHeade"
    "r\022\023\n\013packet_type\030\001 \002(\005\022\027\n\017subpacket_size"
    "s\030\002 \003(\005B\030\n\026uk.me.grambo.syncro.pb", 113);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "header.proto", &protobuf_RegisterTypes);
  PacketHeader::default_instance_ = new PacketHeader();
  PacketHeader::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_header_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_header_2eproto {
  StaticDescriptorInitializer_header_2eproto() {
    protobuf_AddDesc_header_2eproto();
  }
} static_descriptor_initializer_header_2eproto_;


// ===================================================================

#ifndef _MSC_VER
const int PacketHeader::kPacketTypeFieldNumber;
const int PacketHeader::kSubpacketSizesFieldNumber;
#endif  // !_MSC_VER

PacketHeader::PacketHeader()
  : ::google::protobuf::Message() {
  SharedCtor();
}

void PacketHeader::InitAsDefaultInstance() {
}

PacketHeader::PacketHeader(const PacketHeader& from)
  : ::google::protobuf::Message() {
  SharedCtor();
  MergeFrom(from);
}

void PacketHeader::SharedCtor() {
  _cached_size_ = 0;
  packet_type_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

PacketHeader::~PacketHeader() {
  SharedDtor();
}

void PacketHeader::SharedDtor() {
  if (this != default_instance_) {
  }
}

void PacketHeader::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* PacketHeader::descriptor() {
  protobuf_AssignDescriptorsOnce();
  return PacketHeader_descriptor_;
}

const PacketHeader& PacketHeader::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_header_2eproto();  return *default_instance_;
}

PacketHeader* PacketHeader::default_instance_ = NULL;

PacketHeader* PacketHeader::New() const {
  return new PacketHeader;
}

void PacketHeader::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    packet_type_ = 0;
  }
  subpacket_sizes_.Clear();
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
  mutable_unknown_fields()->Clear();
}

bool PacketHeader::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required int32 packet_type = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &packet_type_)));
          _set_bit(0);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_subpacket_sizes;
        break;
      }
      
      // repeated int32 subpacket_sizes = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_subpacket_sizes:
          DO_((::google::protobuf::internal::WireFormatLite::ReadRepeatedPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 1, 16, input, this->mutable_subpacket_sizes())));
        } else if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag)
                   == ::google::protobuf::internal::WireFormatLite::
                      WIRETYPE_LENGTH_DELIMITED) {
          DO_((::google::protobuf::internal::WireFormatLite::ReadPackedPrimitiveNoInline<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, this->mutable_subpacket_sizes())));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_subpacket_sizes;
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

void PacketHeader::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required int32 packet_type = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(1, this->packet_type(), output);
  }
  
  // repeated int32 subpacket_sizes = 2;
  for (int i = 0; i < this->subpacket_sizes_size(); i++) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(
      2, this->subpacket_sizes(i), output);
  }
  
  if (!unknown_fields().empty()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        unknown_fields(), output);
  }
}

::google::protobuf::uint8* PacketHeader::SerializeWithCachedSizesToArray(
    ::google::protobuf::uint8* target) const {
  // required int32 packet_type = 1;
  if (_has_bit(0)) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(1, this->packet_type(), target);
  }
  
  // repeated int32 subpacket_sizes = 2;
  for (int i = 0; i < this->subpacket_sizes_size(); i++) {
    target = ::google::protobuf::internal::WireFormatLite::
      WriteInt32ToArray(2, this->subpacket_sizes(i), target);
  }
  
  if (!unknown_fields().empty()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        unknown_fields(), target);
  }
  return target;
}

int PacketHeader::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required int32 packet_type = 1;
    if (has_packet_type()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->packet_type());
    }
    
  }
  // repeated int32 subpacket_sizes = 2;
  {
    int data_size = 0;
    for (int i = 0; i < this->subpacket_sizes_size(); i++) {
      data_size += ::google::protobuf::internal::WireFormatLite::
        Int32Size(this->subpacket_sizes(i));
    }
    total_size += 1 * this->subpacket_sizes_size() + data_size;
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

void PacketHeader::MergeFrom(const ::google::protobuf::Message& from) {
  GOOGLE_CHECK_NE(&from, this);
  const PacketHeader* source =
    ::google::protobuf::internal::dynamic_cast_if_available<const PacketHeader*>(
      &from);
  if (source == NULL) {
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
    MergeFrom(*source);
  }
}

void PacketHeader::MergeFrom(const PacketHeader& from) {
  GOOGLE_CHECK_NE(&from, this);
  subpacket_sizes_.MergeFrom(from.subpacket_sizes_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_packet_type(from.packet_type());
    }
  }
  mutable_unknown_fields()->MergeFrom(from.unknown_fields());
}

void PacketHeader::CopyFrom(const ::google::protobuf::Message& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void PacketHeader::CopyFrom(const PacketHeader& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool PacketHeader::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void PacketHeader::Swap(PacketHeader* other) {
  if (other != this) {
    std::swap(packet_type_, other->packet_type_);
    subpacket_sizes_.Swap(&other->subpacket_sizes_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    _unknown_fields_.Swap(&other->_unknown_fields_);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::google::protobuf::Metadata PacketHeader::GetMetadata() const {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::Metadata metadata;
  metadata.descriptor = PacketHeader_descriptor_;
  metadata.reflection = PacketHeader_reflection_;
  return metadata;
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)
