# final_project
(1) how to setup and run your program 

1. decode

We have many stages in our final, each stage we use the thread to implement it. Also for the value of the ping, we also 
use the thread to collect the data. The openMV will send the messege to uart char by char. So we first of all decode the
char, and store each value of the parameter. After saving the parameter, we use the parameter to judege which stage are 
we in. 

2. openMV

We add three different functions together, the first one is the line detection, In order to make it look at the correct 
line we cut the detection area of the line and adjust the merge value. The other two function is the aprilTag and face 
detection. I did not adjust much in this two part.

3. stage  1,3,5 following the line

we want to make the car follow the line. So first of all we need to make the open MV only see the line I want. I figure out
that we have to see too high, or it will see a lot of not necessary line. Also , we change the merge value, so that we will
 not see alot of line in one line. We use uart to send the message. we look at the x1, y1, x2, y2, of the line. If the mean
of the line are not in the middle, if the different is at left we turn right, if the value at right we turn left. So that
the car will keep following the line. 

4. stage 2

we want to make the car approach the april tag. First of all, we have to send the angle of y and distance z,x to the uart. 
we will use these parameter to judge if the car is approaching the apriltag. if the angle of the apriltag is larger than 355
 or less than 10 degree. we move foward. If larger, we first turn and move fowrd. If smaller, we turn and approach a little. 
So that it approach the tag. However,if the z coordination is smaller than 5 we drive around.

5. stage 4

Our openMV will send back the percentage of the view that it judge as human. So we use uart to send the value to the mbed.
If the persentage is higher than 90. We call it a face. This time we judge the face and ping together. If we see the face 
and the ping is close enough, we will turen right.

6. stage 6

we want to make the car approach the april tag. First of all, we have to send the angle of y and distance z,x to the uart. 
we will use these parameter to judge if the car is approaching the apriltag. if the angle of the apriltag is larger than 355
 or less than 10 degree. we move foward. If larger, we first turn and move fowrd. If smaller, we turn and approach a little. 
So that it approach the tag. However,if the z coordination is smaller than 5 we stop the car forever.

(2) what are the results

I have six different stages. The first one is line following. The car will follow the line to approach an Apriltag.
After the car is close enough to the Apriltag, it will start to calibrate the location. When it move to the location 
that I assign for the car, it will start to move around that Apriltag. This moving around is stage 2. After it move
around, it will continue to follow another line, this is stage 3. The line will lead it to a human's photo. If the 
photo is recongnize as a human, and the ping is small enough, we will turn right to follow another line to one AprilTag,
this is stage 4. It will calibrate the location again. When it move to the right position it stop, this is stage 5. We 
will use xbee to send the messege when we change our stage.
