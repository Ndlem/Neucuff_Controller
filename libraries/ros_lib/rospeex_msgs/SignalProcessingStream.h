#ifndef _ROS_rospeex_msgs_SignalProcessingStream_h
#define _ROS_rospeex_msgs_SignalProcessingStream_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "std_msgs/Header.h"

namespace rospeex_msgs
{

  class SignalProcessingStream : public ros::Msg
  {
    public:
      std_msgs::Header header;
      uint32_t packet_type;
      const char* packet_data;
      enum { START =  0 };
      enum { DATA =  1 };
      enum { END =  2 };
      enum { CANCEL =  3 };

    SignalProcessingStream():
      header(),
      packet_type(0),
      packet_data("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      *(outbuffer + offset + 0) = (this->packet_type >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->packet_type >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->packet_type >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->packet_type >> (8 * 3)) & 0xFF;
      offset += sizeof(this->packet_type);
      uint32_t length_packet_data = strlen(this->packet_data);
      memcpy(outbuffer + offset, &length_packet_data, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->packet_data, length_packet_data);
      offset += length_packet_data;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      this->packet_type =  ((uint32_t) (*(inbuffer + offset)));
      this->packet_type |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      this->packet_type |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      this->packet_type |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      offset += sizeof(this->packet_type);
      uint32_t length_packet_data;
      memcpy(&length_packet_data, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_packet_data; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_packet_data-1]=0;
      this->packet_data = (char *)(inbuffer + offset-1);
      offset += length_packet_data;
     return offset;
    }

    const char * getType(){ return "rospeex_msgs/SignalProcessingStream"; };
    const char * getMD5(){ return "4c18d356dc98d560eb09170317682408"; };

  };

}
#endif