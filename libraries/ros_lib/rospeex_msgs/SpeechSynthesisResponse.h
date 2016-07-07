#ifndef _ROS_rospeex_msgs_SpeechSynthesisResponse_h
#define _ROS_rospeex_msgs_SpeechSynthesisResponse_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "rospeex_msgs/SpeechSynthesisHeader.h"

namespace rospeex_msgs
{

  class SpeechSynthesisResponse : public ros::Msg
  {
    public:
      rospeex_msgs::SpeechSynthesisHeader header;
      const char* data;
      const char* memo;

    SpeechSynthesisResponse():
      header(),
      data(""),
      memo("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->header.serialize(outbuffer + offset);
      uint32_t length_data = strlen(this->data);
      memcpy(outbuffer + offset, &length_data, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->data, length_data);
      offset += length_data;
      uint32_t length_memo = strlen(this->memo);
      memcpy(outbuffer + offset, &length_memo, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->memo, length_memo);
      offset += length_memo;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->header.deserialize(inbuffer + offset);
      uint32_t length_data;
      memcpy(&length_data, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_data; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_data-1]=0;
      this->data = (char *)(inbuffer + offset-1);
      offset += length_data;
      uint32_t length_memo;
      memcpy(&length_memo, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_memo; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_memo-1]=0;
      this->memo = (char *)(inbuffer + offset-1);
      offset += length_memo;
     return offset;
    }

    const char * getType(){ return "rospeex_msgs/SpeechSynthesisResponse"; };
    const char * getMD5(){ return "5b78953c7f8d4481830a981478194361"; };

  };

}
#endif