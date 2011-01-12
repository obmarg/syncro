// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: admin.proto

package uk.me.grambo.syncro.pb;

public final class Admin {
  private Admin() {}
  public static void registerAllExtensions(
      com.google.protobuf.ExtensionRegistryLite registry) {
  }
  public static final class AdminParameter extends
      com.google.protobuf.GeneratedMessageLite {
    // Use AdminParameter.newBuilder() to construct.
    private AdminParameter() {
      initFields();
    }
    private AdminParameter(boolean noInit) {}
    
    private static final AdminParameter defaultInstance;
    public static AdminParameter getDefaultInstance() {
      return defaultInstance;
    }
    
    public AdminParameter getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    // required string name = 1;
    public static final int NAME_FIELD_NUMBER = 1;
    private boolean hasName;
    private java.lang.String name_ = "";
    public boolean hasName() { return hasName; }
    public java.lang.String getName() { return name_; }
    
    // optional string string_value = 2;
    public static final int STRING_VALUE_FIELD_NUMBER = 2;
    private boolean hasStringValue;
    private java.lang.String stringValue_ = "";
    public boolean hasStringValue() { return hasStringValue; }
    public java.lang.String getStringValue() { return stringValue_; }
    
    // optional int32 int_value = 3;
    public static final int INT_VALUE_FIELD_NUMBER = 3;
    private boolean hasIntValue;
    private int intValue_ = 0;
    public boolean hasIntValue() { return hasIntValue; }
    public int getIntValue() { return intValue_; }
    
    private void initFields() {
    }
    public final boolean isInitialized() {
      if (!hasName) return false;
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (hasName()) {
        output.writeString(1, getName());
      }
      if (hasStringValue()) {
        output.writeString(2, getStringValue());
      }
      if (hasIntValue()) {
        output.writeInt32(3, getIntValue());
      }
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      if (hasName()) {
        size += com.google.protobuf.CodedOutputStream
          .computeStringSize(1, getName());
      }
      if (hasStringValue()) {
        size += com.google.protobuf.CodedOutputStream
          .computeStringSize(2, getStringValue());
      }
      if (hasIntValue()) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(3, getIntValue());
      }
      memoizedSerializedSize = size;
      return size;
    }
    
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminParameter parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(uk.me.grambo.syncro.pb.Admin.AdminParameter prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          uk.me.grambo.syncro.pb.Admin.AdminParameter, Builder> {
      private uk.me.grambo.syncro.pb.Admin.AdminParameter result;
      
      // Construct using uk.me.grambo.syncro.pb.Admin.AdminParameter.newBuilder()
      private Builder() {}
      
      private static Builder create() {
        Builder builder = new Builder();
        builder.result = new uk.me.grambo.syncro.pb.Admin.AdminParameter();
        return builder;
      }
      
      protected uk.me.grambo.syncro.pb.Admin.AdminParameter internalGetResult() {
        return result;
      }
      
      public Builder clear() {
        if (result == null) {
          throw new IllegalStateException(
            "Cannot call clear() after build().");
        }
        result = new uk.me.grambo.syncro.pb.Admin.AdminParameter();
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(result);
      }
      
      public uk.me.grambo.syncro.pb.Admin.AdminParameter getDefaultInstanceForType() {
        return uk.me.grambo.syncro.pb.Admin.AdminParameter.getDefaultInstance();
      }
      
      public boolean isInitialized() {
        return result.isInitialized();
      }
      public uk.me.grambo.syncro.pb.Admin.AdminParameter build() {
        if (result != null && !isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return buildPartial();
      }
      
      private uk.me.grambo.syncro.pb.Admin.AdminParameter buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        if (!isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return buildPartial();
      }
      
      public uk.me.grambo.syncro.pb.Admin.AdminParameter buildPartial() {
        if (result == null) {
          throw new IllegalStateException(
            "build() has already been called on this Builder.");
        }
        uk.me.grambo.syncro.pb.Admin.AdminParameter returnMe = result;
        result = null;
        return returnMe;
      }
      
      public Builder mergeFrom(uk.me.grambo.syncro.pb.Admin.AdminParameter other) {
        if (other == uk.me.grambo.syncro.pb.Admin.AdminParameter.getDefaultInstance()) return this;
        if (other.hasName()) {
          setName(other.getName());
        }
        if (other.hasStringValue()) {
          setStringValue(other.getStringValue());
        }
        if (other.hasIntValue()) {
          setIntValue(other.getIntValue());
        }
        return this;
      }
      
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        while (true) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              return this;
            default: {
              if (!parseUnknownField(input, extensionRegistry, tag)) {
                return this;
              }
              break;
            }
            case 10: {
              setName(input.readString());
              break;
            }
            case 18: {
              setStringValue(input.readString());
              break;
            }
            case 24: {
              setIntValue(input.readInt32());
              break;
            }
          }
        }
      }
      
      
      // required string name = 1;
      public boolean hasName() {
        return result.hasName();
      }
      public java.lang.String getName() {
        return result.getName();
      }
      public Builder setName(java.lang.String value) {
        if (value == null) {
    throw new NullPointerException();
  }
  result.hasName = true;
        result.name_ = value;
        return this;
      }
      public Builder clearName() {
        result.hasName = false;
        result.name_ = getDefaultInstance().getName();
        return this;
      }
      
      // optional string string_value = 2;
      public boolean hasStringValue() {
        return result.hasStringValue();
      }
      public java.lang.String getStringValue() {
        return result.getStringValue();
      }
      public Builder setStringValue(java.lang.String value) {
        if (value == null) {
    throw new NullPointerException();
  }
  result.hasStringValue = true;
        result.stringValue_ = value;
        return this;
      }
      public Builder clearStringValue() {
        result.hasStringValue = false;
        result.stringValue_ = getDefaultInstance().getStringValue();
        return this;
      }
      
      // optional int32 int_value = 3;
      public boolean hasIntValue() {
        return result.hasIntValue();
      }
      public int getIntValue() {
        return result.getIntValue();
      }
      public Builder setIntValue(int value) {
        result.hasIntValue = true;
        result.intValue_ = value;
        return this;
      }
      public Builder clearIntValue() {
        result.hasIntValue = false;
        result.intValue_ = 0;
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:syncro.pb.AdminParameter)
    }
    
    static {
      defaultInstance = new AdminParameter(true);
      uk.me.grambo.syncro.pb.Admin.internalForceInit();
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:syncro.pb.AdminParameter)
  }
  
  public static final class GenericAdminCommand extends
      com.google.protobuf.GeneratedMessageLite {
    // Use GenericAdminCommand.newBuilder() to construct.
    private GenericAdminCommand() {
      initFields();
    }
    private GenericAdminCommand(boolean noInit) {}
    
    private static final GenericAdminCommand defaultInstance;
    public static GenericAdminCommand getDefaultInstance() {
      return defaultInstance;
    }
    
    public GenericAdminCommand getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    // optional string command = 1;
    public static final int COMMAND_FIELD_NUMBER = 1;
    private boolean hasCommand;
    private java.lang.String command_ = "";
    public boolean hasCommand() { return hasCommand; }
    public java.lang.String getCommand() { return command_; }
    
    // repeated .syncro.pb.AdminParameter params = 3;
    public static final int PARAMS_FIELD_NUMBER = 3;
    private java.util.List<uk.me.grambo.syncro.pb.Admin.AdminParameter> params_ =
      java.util.Collections.emptyList();
    public java.util.List<uk.me.grambo.syncro.pb.Admin.AdminParameter> getParamsList() {
      return params_;
    }
    public int getParamsCount() { return params_.size(); }
    public uk.me.grambo.syncro.pb.Admin.AdminParameter getParams(int index) {
      return params_.get(index);
    }
    
    private void initFields() {
    }
    public final boolean isInitialized() {
      for (uk.me.grambo.syncro.pb.Admin.AdminParameter element : getParamsList()) {
        if (!element.isInitialized()) return false;
      }
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (hasCommand()) {
        output.writeString(1, getCommand());
      }
      for (uk.me.grambo.syncro.pb.Admin.AdminParameter element : getParamsList()) {
        output.writeMessage(3, element);
      }
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      if (hasCommand()) {
        size += com.google.protobuf.CodedOutputStream
          .computeStringSize(1, getCommand());
      }
      for (uk.me.grambo.syncro.pb.Admin.AdminParameter element : getParamsList()) {
        size += com.google.protobuf.CodedOutputStream
          .computeMessageSize(3, element);
      }
      memoizedSerializedSize = size;
      return size;
    }
    
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.GenericAdminCommand parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(uk.me.grambo.syncro.pb.Admin.GenericAdminCommand prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          uk.me.grambo.syncro.pb.Admin.GenericAdminCommand, Builder> {
      private uk.me.grambo.syncro.pb.Admin.GenericAdminCommand result;
      
      // Construct using uk.me.grambo.syncro.pb.Admin.GenericAdminCommand.newBuilder()
      private Builder() {}
      
      private static Builder create() {
        Builder builder = new Builder();
        builder.result = new uk.me.grambo.syncro.pb.Admin.GenericAdminCommand();
        return builder;
      }
      
      protected uk.me.grambo.syncro.pb.Admin.GenericAdminCommand internalGetResult() {
        return result;
      }
      
      public Builder clear() {
        if (result == null) {
          throw new IllegalStateException(
            "Cannot call clear() after build().");
        }
        result = new uk.me.grambo.syncro.pb.Admin.GenericAdminCommand();
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(result);
      }
      
      public uk.me.grambo.syncro.pb.Admin.GenericAdminCommand getDefaultInstanceForType() {
        return uk.me.grambo.syncro.pb.Admin.GenericAdminCommand.getDefaultInstance();
      }
      
      public boolean isInitialized() {
        return result.isInitialized();
      }
      public uk.me.grambo.syncro.pb.Admin.GenericAdminCommand build() {
        if (result != null && !isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return buildPartial();
      }
      
      private uk.me.grambo.syncro.pb.Admin.GenericAdminCommand buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        if (!isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return buildPartial();
      }
      
      public uk.me.grambo.syncro.pb.Admin.GenericAdminCommand buildPartial() {
        if (result == null) {
          throw new IllegalStateException(
            "build() has already been called on this Builder.");
        }
        if (result.params_ != java.util.Collections.EMPTY_LIST) {
          result.params_ =
            java.util.Collections.unmodifiableList(result.params_);
        }
        uk.me.grambo.syncro.pb.Admin.GenericAdminCommand returnMe = result;
        result = null;
        return returnMe;
      }
      
      public Builder mergeFrom(uk.me.grambo.syncro.pb.Admin.GenericAdminCommand other) {
        if (other == uk.me.grambo.syncro.pb.Admin.GenericAdminCommand.getDefaultInstance()) return this;
        if (other.hasCommand()) {
          setCommand(other.getCommand());
        }
        if (!other.params_.isEmpty()) {
          if (result.params_.isEmpty()) {
            result.params_ = new java.util.ArrayList<uk.me.grambo.syncro.pb.Admin.AdminParameter>();
          }
          result.params_.addAll(other.params_);
        }
        return this;
      }
      
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        while (true) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              return this;
            default: {
              if (!parseUnknownField(input, extensionRegistry, tag)) {
                return this;
              }
              break;
            }
            case 10: {
              setCommand(input.readString());
              break;
            }
            case 26: {
              uk.me.grambo.syncro.pb.Admin.AdminParameter.Builder subBuilder = uk.me.grambo.syncro.pb.Admin.AdminParameter.newBuilder();
              input.readMessage(subBuilder, extensionRegistry);
              addParams(subBuilder.buildPartial());
              break;
            }
          }
        }
      }
      
      
      // optional string command = 1;
      public boolean hasCommand() {
        return result.hasCommand();
      }
      public java.lang.String getCommand() {
        return result.getCommand();
      }
      public Builder setCommand(java.lang.String value) {
        if (value == null) {
    throw new NullPointerException();
  }
  result.hasCommand = true;
        result.command_ = value;
        return this;
      }
      public Builder clearCommand() {
        result.hasCommand = false;
        result.command_ = getDefaultInstance().getCommand();
        return this;
      }
      
      // repeated .syncro.pb.AdminParameter params = 3;
      public java.util.List<uk.me.grambo.syncro.pb.Admin.AdminParameter> getParamsList() {
        return java.util.Collections.unmodifiableList(result.params_);
      }
      public int getParamsCount() {
        return result.getParamsCount();
      }
      public uk.me.grambo.syncro.pb.Admin.AdminParameter getParams(int index) {
        return result.getParams(index);
      }
      public Builder setParams(int index, uk.me.grambo.syncro.pb.Admin.AdminParameter value) {
        if (value == null) {
          throw new NullPointerException();
        }
        result.params_.set(index, value);
        return this;
      }
      public Builder setParams(int index, uk.me.grambo.syncro.pb.Admin.AdminParameter.Builder builderForValue) {
        result.params_.set(index, builderForValue.build());
        return this;
      }
      public Builder addParams(uk.me.grambo.syncro.pb.Admin.AdminParameter value) {
        if (value == null) {
          throw new NullPointerException();
        }
        if (result.params_.isEmpty()) {
          result.params_ = new java.util.ArrayList<uk.me.grambo.syncro.pb.Admin.AdminParameter>();
        }
        result.params_.add(value);
        return this;
      }
      public Builder addParams(uk.me.grambo.syncro.pb.Admin.AdminParameter.Builder builderForValue) {
        if (result.params_.isEmpty()) {
          result.params_ = new java.util.ArrayList<uk.me.grambo.syncro.pb.Admin.AdminParameter>();
        }
        result.params_.add(builderForValue.build());
        return this;
      }
      public Builder addAllParams(
          java.lang.Iterable<? extends uk.me.grambo.syncro.pb.Admin.AdminParameter> values) {
        if (result.params_.isEmpty()) {
          result.params_ = new java.util.ArrayList<uk.me.grambo.syncro.pb.Admin.AdminParameter>();
        }
        super.addAll(values, result.params_);
        return this;
      }
      public Builder clearParams() {
        result.params_ = java.util.Collections.emptyList();
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:syncro.pb.GenericAdminCommand)
    }
    
    static {
      defaultInstance = new GenericAdminCommand(true);
      uk.me.grambo.syncro.pb.Admin.internalForceInit();
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:syncro.pb.GenericAdminCommand)
  }
  
  public static final class AdminAck extends
      com.google.protobuf.GeneratedMessageLite {
    // Use AdminAck.newBuilder() to construct.
    private AdminAck() {
      initFields();
    }
    private AdminAck(boolean noInit) {}
    
    private static final AdminAck defaultInstance;
    public static AdminAck getDefaultInstance() {
      return defaultInstance;
    }
    
    public AdminAck getDefaultInstanceForType() {
      return defaultInstance;
    }
    
    // optional bool ok = 1;
    public static final int OK_FIELD_NUMBER = 1;
    private boolean hasOk;
    private boolean ok_ = false;
    public boolean hasOk() { return hasOk; }
    public boolean getOk() { return ok_; }
    
    // optional int32 error_code = 2;
    public static final int ERROR_CODE_FIELD_NUMBER = 2;
    private boolean hasErrorCode;
    private int errorCode_ = 0;
    public boolean hasErrorCode() { return hasErrorCode; }
    public int getErrorCode() { return errorCode_; }
    
    private void initFields() {
    }
    public final boolean isInitialized() {
      return true;
    }
    
    public void writeTo(com.google.protobuf.CodedOutputStream output)
                        throws java.io.IOException {
      getSerializedSize();
      if (hasOk()) {
        output.writeBool(1, getOk());
      }
      if (hasErrorCode()) {
        output.writeInt32(2, getErrorCode());
      }
    }
    
    private int memoizedSerializedSize = -1;
    public int getSerializedSize() {
      int size = memoizedSerializedSize;
      if (size != -1) return size;
    
      size = 0;
      if (hasOk()) {
        size += com.google.protobuf.CodedOutputStream
          .computeBoolSize(1, getOk());
      }
      if (hasErrorCode()) {
        size += com.google.protobuf.CodedOutputStream
          .computeInt32Size(2, getErrorCode());
      }
      memoizedSerializedSize = size;
      return size;
    }
    
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(
        com.google.protobuf.ByteString data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(
        com.google.protobuf.ByteString data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(byte[] data)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(
        byte[] data,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return newBuilder().mergeFrom(data, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(java.io.InputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseDelimitedFrom(java.io.InputStream input)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseDelimitedFrom(
        java.io.InputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      Builder builder = newBuilder();
      if (builder.mergeDelimitedFrom(input, extensionRegistry)) {
        return builder.buildParsed();
      } else {
        return null;
      }
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(
        com.google.protobuf.CodedInputStream input)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input).buildParsed();
    }
    public static uk.me.grambo.syncro.pb.Admin.AdminAck parseFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      return newBuilder().mergeFrom(input, extensionRegistry)
               .buildParsed();
    }
    
    public static Builder newBuilder() { return Builder.create(); }
    public Builder newBuilderForType() { return newBuilder(); }
    public static Builder newBuilder(uk.me.grambo.syncro.pb.Admin.AdminAck prototype) {
      return newBuilder().mergeFrom(prototype);
    }
    public Builder toBuilder() { return newBuilder(this); }
    
    public static final class Builder extends
        com.google.protobuf.GeneratedMessageLite.Builder<
          uk.me.grambo.syncro.pb.Admin.AdminAck, Builder> {
      private uk.me.grambo.syncro.pb.Admin.AdminAck result;
      
      // Construct using uk.me.grambo.syncro.pb.Admin.AdminAck.newBuilder()
      private Builder() {}
      
      private static Builder create() {
        Builder builder = new Builder();
        builder.result = new uk.me.grambo.syncro.pb.Admin.AdminAck();
        return builder;
      }
      
      protected uk.me.grambo.syncro.pb.Admin.AdminAck internalGetResult() {
        return result;
      }
      
      public Builder clear() {
        if (result == null) {
          throw new IllegalStateException(
            "Cannot call clear() after build().");
        }
        result = new uk.me.grambo.syncro.pb.Admin.AdminAck();
        return this;
      }
      
      public Builder clone() {
        return create().mergeFrom(result);
      }
      
      public uk.me.grambo.syncro.pb.Admin.AdminAck getDefaultInstanceForType() {
        return uk.me.grambo.syncro.pb.Admin.AdminAck.getDefaultInstance();
      }
      
      public boolean isInitialized() {
        return result.isInitialized();
      }
      public uk.me.grambo.syncro.pb.Admin.AdminAck build() {
        if (result != null && !isInitialized()) {
          throw newUninitializedMessageException(result);
        }
        return buildPartial();
      }
      
      private uk.me.grambo.syncro.pb.Admin.AdminAck buildParsed()
          throws com.google.protobuf.InvalidProtocolBufferException {
        if (!isInitialized()) {
          throw newUninitializedMessageException(
            result).asInvalidProtocolBufferException();
        }
        return buildPartial();
      }
      
      public uk.me.grambo.syncro.pb.Admin.AdminAck buildPartial() {
        if (result == null) {
          throw new IllegalStateException(
            "build() has already been called on this Builder.");
        }
        uk.me.grambo.syncro.pb.Admin.AdminAck returnMe = result;
        result = null;
        return returnMe;
      }
      
      public Builder mergeFrom(uk.me.grambo.syncro.pb.Admin.AdminAck other) {
        if (other == uk.me.grambo.syncro.pb.Admin.AdminAck.getDefaultInstance()) return this;
        if (other.hasOk()) {
          setOk(other.getOk());
        }
        if (other.hasErrorCode()) {
          setErrorCode(other.getErrorCode());
        }
        return this;
      }
      
      public Builder mergeFrom(
          com.google.protobuf.CodedInputStream input,
          com.google.protobuf.ExtensionRegistryLite extensionRegistry)
          throws java.io.IOException {
        while (true) {
          int tag = input.readTag();
          switch (tag) {
            case 0:
              return this;
            default: {
              if (!parseUnknownField(input, extensionRegistry, tag)) {
                return this;
              }
              break;
            }
            case 8: {
              setOk(input.readBool());
              break;
            }
            case 16: {
              setErrorCode(input.readInt32());
              break;
            }
          }
        }
      }
      
      
      // optional bool ok = 1;
      public boolean hasOk() {
        return result.hasOk();
      }
      public boolean getOk() {
        return result.getOk();
      }
      public Builder setOk(boolean value) {
        result.hasOk = true;
        result.ok_ = value;
        return this;
      }
      public Builder clearOk() {
        result.hasOk = false;
        result.ok_ = false;
        return this;
      }
      
      // optional int32 error_code = 2;
      public boolean hasErrorCode() {
        return result.hasErrorCode();
      }
      public int getErrorCode() {
        return result.getErrorCode();
      }
      public Builder setErrorCode(int value) {
        result.hasErrorCode = true;
        result.errorCode_ = value;
        return this;
      }
      public Builder clearErrorCode() {
        result.hasErrorCode = false;
        result.errorCode_ = 0;
        return this;
      }
      
      // @@protoc_insertion_point(builder_scope:syncro.pb.AdminAck)
    }
    
    static {
      defaultInstance = new AdminAck(true);
      uk.me.grambo.syncro.pb.Admin.internalForceInit();
      defaultInstance.initFields();
    }
    
    // @@protoc_insertion_point(class_scope:syncro.pb.AdminAck)
  }
  
  
  static {
  }
  
  public static void internalForceInit() {}
  
  // @@protoc_insertion_point(outer_class_scope)
}
