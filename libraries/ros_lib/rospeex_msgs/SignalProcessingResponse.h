#ifndef _ROS_rospeex_msgs_SignalProcessingResponse_h
#define _ROS_rospeex_msgs_SignalProcessingResponse_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "rospeex_msgs/SpeechRecognitionHeader.h"

namespace rospeex_msgs
{

  class SignalProcessingResponse : public ros::Msg
  {
    public:
      rospeex_msgs::SpeechRecognitionHeader header;
      const char* data;

    SignalProcessingResponse():
      header(),
      data("")
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
     return offset;
    }

    const char * getType(){ return "rospeex_msgs/SignalProcessingResponse"; };
    const char * getMD5(){ return "665730baf05dc730a5a639a4813bf44a"; };

  };

}
#endif