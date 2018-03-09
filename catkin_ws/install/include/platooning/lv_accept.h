// Generated by gencpp from file platooning/lv_accept.msg
// DO NOT EDIT!


#ifndef PLATOONING_MESSAGE_LV_ACCEPT_H
#define PLATOONING_MESSAGE_LV_ACCEPT_H


#include <string>
#include <vector>
#include <map>

#include <ros/types.h>
#include <ros/serialization.h>
#include <ros/builtin_message_traits.h>
#include <ros/message_operations.h>


namespace platooning
{
template <class ContainerAllocator>
struct lv_accept_
{
  typedef lv_accept_<ContainerAllocator> Type;

  lv_accept_()
    : src_vehicle(0)
    , dst_vehicle(0)
    , platoon_id(0)  {
    }
  lv_accept_(const ContainerAllocator& _alloc)
    : src_vehicle(0)
    , dst_vehicle(0)
    , platoon_id(0)  {
  (void)_alloc;
    }



   typedef uint32_t _src_vehicle_type;
  _src_vehicle_type src_vehicle;

   typedef uint32_t _dst_vehicle_type;
  _dst_vehicle_type dst_vehicle;

   typedef uint32_t _platoon_id_type;
  _platoon_id_type platoon_id;





  typedef boost::shared_ptr< ::platooning::lv_accept_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::platooning::lv_accept_<ContainerAllocator> const> ConstPtr;

}; // struct lv_accept_

typedef ::platooning::lv_accept_<std::allocator<void> > lv_accept;

typedef boost::shared_ptr< ::platooning::lv_accept > lv_acceptPtr;
typedef boost::shared_ptr< ::platooning::lv_accept const> lv_acceptConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::platooning::lv_accept_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::platooning::lv_accept_<ContainerAllocator> >::stream(s, "", v);
return s;
}

} // namespace platooning

namespace ros
{
namespace message_traits
{



// BOOLTRAITS {'IsFixedSize': True, 'IsMessage': True, 'HasHeader': False}
// {'std_msgs': ['/opt/ros/lunar/share/std_msgs/cmake/../msg'], 'platooning': ['/home/stepo/workspace/autonomesfahren/Gruppe-C/catkin_ws/src/platooning/msg']}

// !!!!!!!!!!! ['__class__', '__delattr__', '__dict__', '__doc__', '__eq__', '__format__', '__getattribute__', '__hash__', '__init__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_parsed_fields', 'constants', 'fields', 'full_name', 'has_header', 'header_present', 'names', 'package', 'parsed_fields', 'short_name', 'text', 'types']




template <class ContainerAllocator>
struct IsFixedSize< ::platooning::lv_accept_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::platooning::lv_accept_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::platooning::lv_accept_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::platooning::lv_accept_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::platooning::lv_accept_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::platooning::lv_accept_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::platooning::lv_accept_<ContainerAllocator> >
{
  static const char* value()
  {
    return "171c710bbe681f26bc25a8cb194a204b";
  }

  static const char* value(const ::platooning::lv_accept_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0x171c710bbe681f26ULL;
  static const uint64_t static_value2 = 0xbc25a8cb194a204bULL;
};

template<class ContainerAllocator>
struct DataType< ::platooning::lv_accept_<ContainerAllocator> >
{
  static const char* value()
  {
    return "platooning/lv_accept";
  }

  static const char* value(const ::platooning::lv_accept_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::platooning::lv_accept_<ContainerAllocator> >
{
  static const char* value()
  {
    return "uint32 src_vehicle\n\
uint32 dst_vehicle\n\
uint32 platoon_id\n\
";
  }

  static const char* value(const ::platooning::lv_accept_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::platooning::lv_accept_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.src_vehicle);
      stream.next(m.dst_vehicle);
      stream.next(m.platoon_id);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct lv_accept_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::platooning::lv_accept_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::platooning::lv_accept_<ContainerAllocator>& v)
  {
    s << indent << "src_vehicle: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.src_vehicle);
    s << indent << "dst_vehicle: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.dst_vehicle);
    s << indent << "platoon_id: ";
    Printer<uint32_t>::stream(s, indent + "  ", v.platoon_id);
  }
};

} // namespace message_operations
} // namespace ros

#endif // PLATOONING_MESSAGE_LV_ACCEPT_H
