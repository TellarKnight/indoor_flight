import rospy
import geometry_msgs

# pub = rospy.Publisher("topic_name",msg_type,queue_size=None)
# pub.publish(msg) # 发布消息
# pub.unregister() #停止发布
#######################SUBSCRIBE#############################
def callback(msg):
    distance = msg
    rospy.loginfo('Listener:Position:',distance,msg)

def listener():
    rospy.init_node("indoor_groundstation",anonymous=True)
    rospy.Subscriber("position_info",callback)
    rospy.spin()

if __name__ == '__main__':
    listener()
