// Generated by the protocol buffer compiler.  DO NOT EDIT!

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "handshake.pb.h"
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
// @@protoc_insertion_point(includes)

namespace syncro {
namespace pb {

void protobuf_ShutdownFile_handshake_2eproto() {
  delete HandshakeRequest::default_instance_;
  delete HandshakeResponse::default_instance_;
}

void protobuf_AddDesc_handshake_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  HandshakeRequest::default_instance_ = new HandshakeRequest();
  HandshakeResponse::default_instance_ = new HandshakeResponse();
  HandshakeRequest::default_instance_->InitAsDefaultInstance();
  HandshakeResponse::default_instance_->InitAsDefaultInstance();
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_handshake_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_handshake_2eproto {
  StaticDescriptorInitializer_handshake_2eproto() {
    protobuf_AddDesc_handshake_2eproto();
  }
} static_descriptor_initializer_handshake_2eproto_;


// ===================================================================

const ::std::string HandshakeRequest::_default_magic_;
#ifndef _MSC_VER
const int HandshakeRequest::kMagicFieldNumber;
const int HandshakeRequest::kClientVerMajorFieldNumber;
const int HandshakeRequest::kClientVerMinorFieldNumber;
#endif  // !_MSC_VER

HandshakeRequest::HandshakeRequest()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void HandshakeRequest::InitAsDefaultInstance() {
}

HandshakeRequest::HandshakeRequest(const HandshakeRequest& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void HandshakeRequest::SharedCtor() {
  _cached_size_ = 0;
  magic_ = const_cast< ::std::string*>(&_default_magic_);
  client_ver_major_ = 0;
  client_ver_minor_ = 0;
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

HandshakeRequest::~HandshakeRequest() {
  SharedDtor();
}

void HandshakeRequest::SharedDtor() {
  if (magic_ != &_default_magic_) {
    delete magic_;
  }
  if (this != default_instance_) {
  }
}

void HandshakeRequest::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const HandshakeRequest& HandshakeRequest::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_handshake_2eproto();  return *default_instance_;
}

HandshakeRequest* HandshakeRequest::default_instance_ = NULL;

HandshakeRequest* HandshakeRequest::New() const {
  return new HandshakeRequest;
}

void HandshakeRequest::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (magic_ != &_default_magic_) {
        magic_->clear();
      }
    }
    client_ver_major_ = 0;
    client_ver_minor_ = 0;
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool HandshakeRequest::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string magic = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_magic()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(16)) goto parse_client_ver_major;
        break;
      }
      
      // optional int32 client_ver_major = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_client_ver_major:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &client_ver_major_)));
          _set_bit(1);
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(24)) goto parse_client_ver_minor;
        break;
      }
      
      // optional int32 client_ver_minor = 3;
      case 3: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_VARINT) {
         parse_client_ver_minor:
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &client_ver_minor_)));
          _set_bit(2);
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

void HandshakeRequest::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string magic = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->magic(), output);
  }
  
  // optional int32 client_ver_major = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(2, this->client_ver_major(), output);
  }
  
  // optional int32 client_ver_minor = 3;
  if (_has_bit(2)) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(3, this->client_ver_minor(), output);
  }
  
}

int HandshakeRequest::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string magic = 1;
    if (has_magic()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->magic());
    }
    
    // optional int32 client_ver_major = 2;
    if (has_client_ver_major()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->client_ver_major());
    }
    
    // optional int32 client_ver_minor = 3;
    if (has_client_ver_minor()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->client_ver_minor());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HandshakeRequest::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const HandshakeRequest*>(&from));
}

void HandshakeRequest::MergeFrom(const HandshakeRequest& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_magic(from.magic());
    }
    if (from._has_bit(1)) {
      set_client_ver_major(from.client_ver_major());
    }
    if (from._has_bit(2)) {
      set_client_ver_minor(from.client_ver_minor());
    }
  }
}

void HandshakeRequest::CopyFrom(const HandshakeRequest& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HandshakeRequest::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void HandshakeRequest::Swap(HandshakeRequest* other) {
  if (other != this) {
    std::swap(magic_, other->magic_);
    std::swap(client_ver_major_, other->client_ver_major_);
    std::swap(client_ver_minor_, other->client_ver_minor_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string HandshakeRequest::GetTypeName() const {
  return "syncro.pb.HandshakeRequest";
}


// ===================================================================

const ::std::string HandshakeResponse::_default_magic_;
const ::std::string HandshakeResponse::_default_uuid_;
#ifndef _MSC_VER
const int HandshakeResponse::kMagicFieldNumber;
const int HandshakeResponse::kUuidFieldNumber;
#endif  // !_MSC_VER

HandshakeResponse::HandshakeResponse()
  : ::google::protobuf::MessageLite() {
  SharedCtor();
}

void HandshakeResponse::InitAsDefaultInstance() {
}

HandshakeResponse::HandshakeResponse(const HandshakeResponse& from)
  : ::google::protobuf::MessageLite() {
  SharedCtor();
  MergeFrom(from);
}

void HandshakeResponse::SharedCtor() {
  _cached_size_ = 0;
  magic_ = const_cast< ::std::string*>(&_default_magic_);
  uuid_ = const_cast< ::std::string*>(&_default_uuid_);
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

HandshakeResponse::~HandshakeResponse() {
  SharedDtor();
}

void HandshakeResponse::SharedDtor() {
  if (magic_ != &_default_magic_) {
    delete magic_;
  }
  if (uuid_ != &_default_uuid_) {
    delete uuid_;
  }
  if (this != default_instance_) {
  }
}

void HandshakeResponse::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const HandshakeResponse& HandshakeResponse::default_instance() {
  if (default_instance_ == NULL) protobuf_AddDesc_handshake_2eproto();  return *default_instance_;
}

HandshakeResponse* HandshakeResponse::default_instance_ = NULL;

HandshakeResponse* HandshakeResponse::New() const {
  return new HandshakeResponse;
}

void HandshakeResponse::Clear() {
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (_has_bit(0)) {
      if (magic_ != &_default_magic_) {
        magic_->clear();
      }
    }
    if (_has_bit(1)) {
      if (uuid_ != &_default_uuid_) {
        uuid_->clear();
      }
    }
  }
  ::memset(_has_bits_, 0, sizeof(_has_bits_));
}

bool HandshakeResponse::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!(EXPRESSION)) return false
  ::google::protobuf::uint32 tag;
  while ((tag = input->ReadTag()) != 0) {
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required string magic = 1;
      case 1: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_magic()));
        } else {
          goto handle_uninterpreted;
        }
        if (input->ExpectTag(18)) goto parse_uuid;
        break;
      }
      
      // optional string uuid = 2;
      case 2: {
        if (::google::protobuf::internal::WireFormatLite::GetTagWireType(tag) ==
            ::google::protobuf::internal::WireFormatLite::WIRETYPE_LENGTH_DELIMITED) {
         parse_uuid:
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_uuid()));
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

void HandshakeResponse::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // required string magic = 1;
  if (_has_bit(0)) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      1, this->magic(), output);
  }
  
  // optional string uuid = 2;
  if (_has_bit(1)) {
    ::google::protobuf::internal::WireFormatLite::WriteString(
      2, this->uuid(), output);
  }
  
}

int HandshakeResponse::ByteSize() const {
  int total_size = 0;
  
  if (_has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    // required string magic = 1;
    if (has_magic()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->magic());
    }
    
    // optional string uuid = 2;
    if (has_uuid()) {
      total_size += 1 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->uuid());
    }
    
  }
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = total_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void HandshakeResponse::CheckTypeAndMergeFrom(
    const ::google::protobuf::MessageLite& from) {
  MergeFrom(*::google::protobuf::down_cast<const HandshakeResponse*>(&from));
}

void HandshakeResponse::MergeFrom(const HandshakeResponse& from) {
  GOOGLE_CHECK_NE(&from, this);
  if (from._has_bits_[0 / 32] & (0xffu << (0 % 32))) {
    if (from._has_bit(0)) {
      set_magic(from.magic());
    }
    if (from._has_bit(1)) {
      set_uuid(from.uuid());
    }
  }
}

void HandshakeResponse::CopyFrom(const HandshakeResponse& from) {
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool HandshakeResponse::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000001) != 0x00000001) return false;
  
  return true;
}

void HandshakeResponse::Swap(HandshakeResponse* other) {
  if (other != this) {
    std::swap(magic_, other->magic_);
    std::swap(uuid_, other->uuid_);
    std::swap(_has_bits_[0], other->_has_bits_[0]);
    std::swap(_cached_size_, other->_cached_size_);
  }
}

::std::string HandshakeResponse::GetTypeName() const {
  return "syncro.pb.HandshakeResponse";
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb
}  // namespace syncro

// @@protoc_insertion_point(global_scope)