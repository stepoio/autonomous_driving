// Auto-generated. Do not edit!

// (in-package platooning.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;

//-----------------------------------------------------------

class platooningToggle {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.enable_platooning = null;
      this.inner_platoon_distance = null;
      this.platoon_speed = null;
    }
    else {
      if (initObj.hasOwnProperty('enable_platooning')) {
        this.enable_platooning = initObj.enable_platooning
      }
      else {
        this.enable_platooning = false;
      }
      if (initObj.hasOwnProperty('inner_platoon_distance')) {
        this.inner_platoon_distance = initObj.inner_platoon_distance
      }
      else {
        this.inner_platoon_distance = 0.0;
      }
      if (initObj.hasOwnProperty('platoon_speed')) {
        this.platoon_speed = initObj.platoon_speed
      }
      else {
        this.platoon_speed = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type platooningToggle
    // Serialize message field [enable_platooning]
    bufferOffset = _serializer.bool(obj.enable_platooning, buffer, bufferOffset);
    // Serialize message field [inner_platoon_distance]
    bufferOffset = _serializer.float32(obj.inner_platoon_distance, buffer, bufferOffset);
    // Serialize message field [platoon_speed]
    bufferOffset = _serializer.float32(obj.platoon_speed, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type platooningToggle
    let len;
    let data = new platooningToggle(null);
    // Deserialize message field [enable_platooning]
    data.enable_platooning = _deserializer.bool(buffer, bufferOffset);
    // Deserialize message field [inner_platoon_distance]
    data.inner_platoon_distance = _deserializer.float32(buffer, bufferOffset);
    // Deserialize message field [platoon_speed]
    data.platoon_speed = _deserializer.float32(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    return 9;
  }

  static datatype() {
    // Returns string type for a message object
    return 'platooning/platooningToggle';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return 'd873b59209a1794389a9e491f37fe75f';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    bool enable_platooning
    float32 inner_platoon_distance
    float32 platoon_speed
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new platooningToggle(null);
    if (msg.enable_platooning !== undefined) {
      resolved.enable_platooning = msg.enable_platooning;
    }
    else {
      resolved.enable_platooning = false
    }

    if (msg.inner_platoon_distance !== undefined) {
      resolved.inner_platoon_distance = msg.inner_platoon_distance;
    }
    else {
      resolved.inner_platoon_distance = 0.0
    }

    if (msg.platoon_speed !== undefined) {
      resolved.platoon_speed = msg.platoon_speed;
    }
    else {
      resolved.platoon_speed = 0.0
    }

    return resolved;
    }
};

module.exports = platooningToggle;