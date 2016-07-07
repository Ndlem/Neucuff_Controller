#ifndef _ROS_naoqi_bridge_msgs_BodyPoseWithSpeedAction_h
#define _ROS_naoqi_bridge_msgs_BodyPoseWithSpeedAction_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "naoqi_bridge_msgs/BodyPoseWithSpeedActionGoal.h"
#include "naoqi_bridge_msgs/BodyPoseWithSpeedActionResult.h"
#include "naoqi_bridge_msgs/BodyPoseWithSpeedActionFeedback.h"

namespace naoqi_bridge_msgs
{

  class BodyPoseWithSpeedAction : public ros::Msg
  {
    public:
      naoqi_bridge_msgs::BodyPoseWithSpeedActionGoal action_goal;
      naoqi_bridge_msgs::BodyPoseWithSpeedActionResult action_result;
      naoqi_bridge_msgs::BodyPoseWithSpeedActionFeedback action_feedback;

    BodyPoseWithSpeedAction():
      action_goal(),
      action_result(),
      action_feedback()
    {
    }

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      offset += this->action_goal.serialize(outbuffer + offset);
      offset += this->action_result.serialize(outbuffer + offset);
      offset += this->action_feedback.serialize(outbuffer + offset);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      offset += this->action_goal.deserialize(inbuffer + offset);
      offset += this->action_result.deserialize(inbuffer + offset);
      offset += this->action_feedback.deserialize(inbuffer + offset);
     return offset;
    }

    const char * getType(){ return "naoqi_bridge_msgs/BodyPoseWithSpeedAction"; };
    const char * getMD5(){ return "8843d4cb535c2767e978466b886561fb"; };

  };

}
#endif