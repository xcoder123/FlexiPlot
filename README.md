# FlexiPlot
Simple Qt based plotting tool for micro-controllers


Sample data sent from uC to FlexiPlot
Basically, it will send a real time update of two separate data points to flexi for plotting on the same plane with ID "P0"

{P0|Roll|255,0,0|25|Pitch|0,255,0|56}

Note a packet consists of 
-ID of plot
-Name of real time value (Roll)
-Color in RGB format (Red)
-Value (25)

-Name of real time value
-Color in RGB format
-Value

And you can repeat this pattern as much as you like.

It also supports XY plane plotting. For more details in settings tab hover over properties and you will see examples of packets.


# Screenshot
![image](https://cloud.githubusercontent.com/assets/7244389/24076638/d2f9edaa-0c2d-11e7-811f-6b93956c5f48.png)
![image](https://cloud.githubusercontent.com/assets/7244389/6420839/bc6029c6-bec1-11e4-81fd-333fa1047a5d.png)
