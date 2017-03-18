# FlexiPlot

Simple Qt based plotting tool for micro-controllers

## Sending Data to FlexiPlot

### Line chart

Sample data sent from uC to FlexiPlot
Basically, it will send a real time update of two separate data points to flexi for plotting on the same plane with ID "P0"

```
{P0|Roll|255,0,0|25|Pitch|0,255,0|56}
```

Note a packet consists of 
-ID of plot
-Name of real time value (Roll)
-Color in RGB format (Red)
-Value (25)

-Name of real time value
-Color in RGB format
-Value

And you can repeat this pattern as much as you like.

It also supports XY plane plotting. A sample packet for plotting quadratic:

```
{P3|Quadratic|255,0,0|-5 25 -4 16 -3 9 -2 4 -1 1 0 0 1 1 2 4 3 9 4 16 5 25}
```

### Bar Graphs

An example packet for plotting to a bar graph:
Plotting with automatic color assignment
```
{P0|Jan;Feb;Mar|Jane|25 15 17|John|8 9 23}
```

P0 = ID of plot
Jan;Feb:Mar = Are the columns for bar graph
Jane = Is the set name for bar graph, every set expects as many values as columns
25 15 17 = Values for Jane for columsn Jan, Feb and Mar.
John= Is the set name for bar graph, every set expects as many values as columns
8 9 23 = Values for John for columsn Jan, Feb and Mar.

If you want to use custom set of colors, you can just specify them in the packet:
```
{P0|Jan;Feb;Mar|Jane|255,0,0|25 15 17|John|0,255,0|8 9 23}
```


# Screenshots
![image](https://cloud.githubusercontent.com/assets/7244389/24076638/d2f9edaa-0c2d-11e7-811f-6b93956c5f48.png)
![image](https://cloud.githubusercontent.com/assets/7244389/24076772/1805ef0e-0c31-11e7-9859-1a62b90e2288.png)
![image](https://cloud.githubusercontent.com/assets/7244389/6420839/bc6029c6-bec1-11e4-81fd-333fa1047a5d.png)
