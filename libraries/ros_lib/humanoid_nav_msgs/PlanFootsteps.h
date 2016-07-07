#ifndef _ROS_SERVICE_PlanFootsteps_h
#define _ROS_SERVICE_PlanFootsteps_h
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "geometry_msgs/Pose2D.h"
#include "humanoid_nav_msgs/StepTarget.h"

namespace humanoid_nav_msgs
{

static const char PLANFOOTSTEPS[] = "humanoid_nav_msgs/PlanFootsteps";

  class PlanFootstepsRequest : public ros::Msg
  {
    public:
      geometry_msgs::Pose2D start;
      geometry_msgs::Pose2D goal;

    PlanFootstepsRequest():
      start(),
      goal()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->start.serialize(outbuffer + offset);
      offset += this->goal.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->start.deserialize(inbuffer + offset);
      offset += this->goal.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return PLANFOOTSTEPS; };
    const char * getMD5(){ return "5e8ecd9fb61e382b8974a904baeee367"; };

  };

  class PlanFootstepsResponse : public ros::Msg
  {
    public:
      bool result;
      uint8_t footsteps_length;
      humanoid_nav_msgs::StepTarget st_footsteps;
      humanoid_nav_msgs::StepTarget * footsteps;
      float costs;
      float final_eps;
      float planning_time;
      int64_t expanded_states;

    PlanFootstepsResponse():
      result(0),
      footsteps_length(0), footsteps(NULL),
      costs(0),
      final_eps(0),
      planning_time(0),
      expanded_states(0)
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
      *(outbuffer + offset++) = footsteps_length;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      for( uint8_t i = 0; i < footsteps_length; i++){
      offset += this->footsteps[i].serialize(outbuffer + offset);
      }
      offset += serializeAvrFloat64(outbuffer + offset, this->costs);
      offset += serializeAvrFloat64(outbuffer + offset, this->final_eps);
      offset += serializeAvrFloat64(outbuffer + offset, this->planning_time);
      union {
        int64_t real;
        uint64_t base;
      } u_expanded_states;
      u_expanded_states.real = this->expanded_states;
      *(outbuffer + offset + 0) = (u_expanded_states.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_expanded_states.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_expanded_states.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_expanded_states.base >> (8 * 3)) & 0xFF;
      *(outbuffer + offset + 4) = (u_expanded_states.base >> (8 * 4)) & 0xFF;
      *(outbuffer + offset + 5) = (u_expanded_states.base >> (8 * 5)) & 0xFF;
      *(outbuffer + offset + 6) = (u_expanded_states.base >> (8 * 6)) & 0xFF;
      *(outbuffer + offset + 7) = (u_expanded_states.base >> (8 * 7)) & 0xFF;
      offset += sizeof(this->expanded_states);
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
      uint8_t footsteps_lengthT = *(inbuffer + offset++);
      if(footsteps_lengthT > footsteps_length)
        this->footsteps = (humanoid_nav_msgs::StepTarget*)realloc(this->footsteps, footsteps_lengthT * sizeof(humanoid_nav_msgs::StepTarget));
      offset += 3;
      footsteps_length = footsteps_lengthT;
      for( uint8_t i = 0; i < footsteps_length; i++){
      offset += this->st_footsteps.deserialize(inbuffer + offset);
        memcpy( &(this->footsteps[i]), &(this->st_footsteps), sizeof(humanoid_nav_msgs::StepTarget));
      }
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->costs));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->final_eps));
      offset += deserializeAvrFloat64(inbuffer + offset, &(this->planning_time));
      union {
        int64_t real;
        uint64_t base;
      } u_expanded_states;
      u_expanded_states.base = 0;
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 3))) << (8 * 3);
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 4))) << (8 * 4);
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 5))) << (8 * 5);
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 6))) << (8 * 6);
      u_expanded_states.base |= ((uint64_t) (*(inbuffer + offset + 7))) << (8 * 7);
      this->expanded_states = u_expanded_states.real;
      offset += sizeof(this->expanded_states);
     return offset;
    }

    const char * getType(){ return PLANFOOTSTEPS; };
    const char * getMD5(){ return "1af07cd1d4ffe34a9a731e87aa13835c"; };

  };

  class PlanFootsteps {
    public:
    typedef PlanFootstepsRequest Request;
    typedef PlanFootstepsResponse Response;
  };

}
#endif
