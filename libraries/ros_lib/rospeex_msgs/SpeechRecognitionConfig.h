#ifndef _ROS_SERVICE_SpeechRecognitionConfig_h
#define _ROS_SERVICE_SpeechRecognitionConfig_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace rospeex_msgs
{

static const char SPEECHRECOGNITIONCONFIG[] = "rospeex_msgs/SpeechRecognitionConfig";

  class SpeechRecognitionConfigRequest : public ros::Msg
  {
    public:
      const char* engine;
      const char* language;

    SpeechRecognitionConfigRequest():
      engine(""),
      language("")
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_engine = strlen(this->engine);
      memcpy(outbuffer + offset, &length_engine, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->engine, length_engine);
      offset += length_engine;
      uint32_t length_language = strlen(this->language);
      memcpy(outbuffer + offset, &length_language, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->language, length_language);
      offset += length_language;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_engine;
      memcpy(&length_engine, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_engine; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_engine-1]=0;
      this->engine = (char *)(inbuffer + offset-1);
      offset += length_engine;
      uint32_t length_language;
      memcpy(&length_language, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_language; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_language-1]=0;
      this->language = (char *)(inbuffer + offset-1);
      offset += length_language;
     return offset;
    }

    const char * getType(){ return SPEECHRECOGNITIONCONFIG; };
    const char * getMD5(){ return "917f81c82a3f1256fb139388ff9e634c"; };

  };

  class SpeechRecognitionConfigResponse : public ros::Msg
  {
    public:
      bool result;

    SpeechRecognitionConfigResponse():
      result(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_result;
      u_result.real = this->result;
      *(outbuffer + offset + 0) = (u_result.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->result);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      union {
        bool real;
        uint8_t base;
      } u_result;
      u_result.base = 0;
      u_result.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->result = u_result.real;
      offset += sizeof(this->result);
     return offset;
    }

    const char * getType(){ return SPEECHRECOGNITIONCONFIG; };
    const char * getMD5(){ return "eb13ac1f1354ccecb7941ee8fa2192e8"; };

  };

  class SpeechRecognitionConfig {
    public:
    typedef SpeechRecognitionConfigRequest Request;
    typedef SpeechRecognitionConfigResponse Response;
  };

}
#endif
