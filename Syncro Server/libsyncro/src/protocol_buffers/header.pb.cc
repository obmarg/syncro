// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "header.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace syncro {
namespace pb {

void protobuf_ShutdownFile_header_2eproto() {
  delete PacketHeader::default_instance_;
}

void protobuf_AddDesc_header_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

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
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void PacketHeader::InitAsDefaultInstance() {
}

PacketHeader::PacketHeader(const PacketHeader& from)
  : ::google::protobuf::MessageLite() {
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
        DO_(::google::protobuf::internal::WireFormatLite::SkipField(input, tag));
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
  
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void PacketHeader::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const PacketHeader*>(&from));
}

void PacketHeader::MergeFrom(const PacketHeader& from) {
  GOOGLE_CHECK_NE(&from, this);
  subpacket_sizes_.MergeFrom(from.subpacket_sizes_);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_packet_type(from.packet_type());
    }
  }
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
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string PacketHeader::GetTypeName() const {
  return "syncro.pb.PacketHeader";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)