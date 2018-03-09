// Generated by gencpp from file platooning/platooningToggle.msg
// DO NOT EDIT!


#ifndef PLATOONING_MESSAGE_PLATOONINGTOGGLE_H
#define PLATOONING_MESSAGE_PLATOONINGTOGGLE_H


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
struct platooningToggle_
{
  typedef platooningToggle_<ContainerAllocator> Type;

  platooningToggle_()
    : enable_platooning(false)
    , inner_platoon_distance(0.0)
    , platoon_speed(0.0)  {
    }
  platooningToggle_(const ContainerAllocator& _alloc)
    : enable_platooning(false)
    , inner_platoon_distance(0.0)
    , platoon_speed(0.0)  {
  (void)_alloc;
    }



   typedef uint8_t _enable_platooning_type;
  _enable_platooning_type enable_platooning;

   typedef float _inner_platoon_distance_type;
  _inner_platoon_distance_type inner_platoon_distance;

   typedef float _platoon_speed_type;
  _platoon_speed_type platoon_speed;





  typedef boost::shared_ptr< ::platooning::platooningToggle_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::platooning::platooningToggle_<ContainerAllocator> const> ConstPtr;

}; // struct platooningToggle_

typedef ::platooning::platooningToggle_<std::allocator<void> > platooningToggle;

typedef boost::shared_ptr< ::platooning::platooningToggle > platooningTogglePtr;
typedef boost::shared_ptr< ::platooning::platooningToggle const> platooningToggleConstPtr;

// constants requiring out of line definition



template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const ::platooning::platooningToggle_<ContainerAllocator> & v)
{
ros::message_operations::Printer< ::platooning::platooningToggle_<ContainerAllocator> >::stream(s, "", v);
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
struct IsFixedSize< ::platooning::platooningToggle_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsFixedSize< ::platooning::platooningToggle_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::platooning::platooningToggle_<ContainerAllocator> >
  : TrueType
  { };

template <class ContainerAllocator>
struct IsMessage< ::platooning::platooningToggle_<ContainerAllocator> const>
  : TrueType
  { };

template <class ContainerAllocator>
struct HasHeader< ::platooning::platooningToggle_<ContainerAllocator> >
  : FalseType
  { };

template <class ContainerAllocator>
struct HasHeader< ::platooning::platooningToggle_<ContainerAllocator> const>
  : FalseType
  { };


template<class ContainerAllocator>
struct MD5Sum< ::platooning::platooningToggle_<ContainerAllocator> >
{
  static const char* value()
  {
    return "d873b59209a1794389a9e491f37fe75f";
  }

  static const char* value(const ::platooning::platooningToggle_<ContainerAllocator>&) { return value(); }
  static const uint64_t static_value1 = 0xd873b59209a17943ULL;
  static const uint64_t static_value2 = 0x89a9e491f37fe75fULL;
};

template<class ContainerAllocator>
struct DataType< ::platooning::platooningToggle_<ContainerAllocator> >
{
  static const char* value()
  {
    return "platooning/platooningToggle";
  }

  static const char* value(const ::platooning::platooningToggle_<ContainerAllocator>&) { return value(); }
};

template<class ContainerAllocator>
struct Definition< ::platooning::platooningToggle_<ContainerAllocator> >
{
  static const char* value()
  {
    return "bool enable_platooning\n\
float32 inner_platoon_distance\n\
float32 platoon_speed\n\
";
  }

  static const char* value(const ::platooning::platooningToggle_<ContainerAllocator>&) { return value(); }
};

} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

  template<class ContainerAllocator> struct Serializer< ::platooning::platooningToggle_<ContainerAllocator> >
  {
    template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
    {
      stream.next(m.enable_platooning);
      stream.next(m.inner_platoon_distance);
      stream.next(m.platoon_speed);
    }

    ROS_DECLARE_ALLINONE_SERIALIZER
  }; // struct platooningToggle_

} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::platooning::platooningToggle_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const ::platooning::platooningToggle_<ContainerAllocator>& v)
  {
    s << indent << "enable_platooning: ";
    Printer<uint8_t>::stream(s, indent + "  ", v.enable_platooning);
    s << indent << "inner_platoon_distance: ";
    Printer<float>::stream(s, indent + "  ", v.inner_platoon_distance);
    s << indent << "platoon_speed: ";
    Printer<float>::stream(s, indent + "  ", v.platoon_speed);
  }
};

} // namespace message_operations
} // namespace ros

#endif // PLATOONING_MESSAGE_PLATOONINGTOGGLE_H