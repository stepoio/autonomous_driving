# This Python file uses the following encoding: utf-8
"""autogenerated by genpy from platooning/lv_broadcast.msg. Do not edit."""
import sys
python3 = True if sys.hexversion > 0x03000000 else False
import genpy
import struct


class lv_broadcast(genpy.Message):
  _md5sum = "a181b9158cc00e160e1fda722b669eac"
  _type = "platooning/lv_broadcast"
  _has_header = False #flag to mark the presence of a Header object
  _full_text = """uint32 src_vehicle
uint32 platoon_id
float32 ipd
float32 ps
uint32[] followers"""
  __slots__ = ['src_vehicle','platoon_id','ipd','ps','followers']
  _slot_types = ['uint32','uint32','float32','float32','uint32[]']

  def __init__(self, *args, **kwds):
    """
    Constructor. Any message fields that are implicitly/explicitly
    set to None will be assigned a default value. The recommend
    use is keyword arguments as this is more robust to future message
    changes.  You cannot mix in-order arguments and keyword arguments.

    The available fields are:
       src_vehicle,platoon_id,ipd,ps,followers

    :param args: complete set of field values, in .msg order
    :param kwds: use keyword arguments corresponding to message field names
    to set specific fields.
    """
    if args or kwds:
      super(lv_broadcast, self).__init__(*args, **kwds)
      #message fields cannot be None, assign default values for those that are
      if self.src_vehicle is None:
        self.src_vehicle = 0
      if self.platoon_id is None:
        self.platoon_id = 0
      if self.ipd is None:
        self.ipd = 0.
      if self.ps is None:
        self.ps = 0.
      if self.followers is None:
        self.followers = []
    else:
      self.src_vehicle = 0
      self.platoon_id = 0
      self.ipd = 0.
      self.ps = 0.
      self.followers = []

  def _get_types(self):
    """
    internal API method
    """
    return self._slot_types

  def serialize(self, buff):
    """
    serialize message into buffer
    :param buff: buffer, ``StringIO``
    """
    try:
      _x = self
      buff.write(_get_struct_2I2f().pack(_x.src_vehicle, _x.platoon_id, _x.ipd, _x.ps))
      length = len(self.followers)
      buff.write(_struct_I.pack(length))
      pattern = '<%sI'%length
      buff.write(struct.pack(pattern, *self.followers))
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize(self, str):
    """
    unpack serialized message in str into this message instance
    :param str: byte array of serialized message, ``str``
    """
    try:
      end = 0
      _x = self
      start = end
      end += 16
      (_x.src_vehicle, _x.platoon_id, _x.ipd, _x.ps,) = _get_struct_2I2f().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sI'%length
      start = end
      end += struct.calcsize(pattern)
      self.followers = struct.unpack(pattern, str[start:end])
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill


  def serialize_numpy(self, buff, numpy):
    """
    serialize message with numpy array types into buffer
    :param buff: buffer, ``StringIO``
    :param numpy: numpy python module
    """
    try:
      _x = self
      buff.write(_get_struct_2I2f().pack(_x.src_vehicle, _x.platoon_id, _x.ipd, _x.ps))
      length = len(self.followers)
      buff.write(_struct_I.pack(length))
      pattern = '<%sI'%length
      buff.write(self.followers.tostring())
    except struct.error as se: self._check_types(struct.error("%s: '%s' when writing '%s'" % (type(se), str(se), str(locals().get('_x', self)))))
    except TypeError as te: self._check_types(ValueError("%s: '%s' when writing '%s'" % (type(te), str(te), str(locals().get('_x', self)))))

  def deserialize_numpy(self, str, numpy):
    """
    unpack serialized message in str into this message instance using numpy for array types
    :param str: byte array of serialized message, ``str``
    :param numpy: numpy python module
    """
    try:
      end = 0
      _x = self
      start = end
      end += 16
      (_x.src_vehicle, _x.platoon_id, _x.ipd, _x.ps,) = _get_struct_2I2f().unpack(str[start:end])
      start = end
      end += 4
      (length,) = _struct_I.unpack(str[start:end])
      pattern = '<%sI'%length
      start = end
      end += struct.calcsize(pattern)
      self.followers = numpy.frombuffer(str[start:end], dtype=numpy.uint32, count=length)
      return self
    except struct.error as e:
      raise genpy.DeserializationError(e) #most likely buffer underfill

_struct_I = genpy.struct_I
def _get_struct_I():
    global _struct_I
    return _struct_I
_struct_2I2f = None
def _get_struct_2I2f():
    global _struct_2I2f
    if _struct_2I2f is None:
        _struct_2I2f = struct.Struct("<2I2f")
    return _struct_2I2f
