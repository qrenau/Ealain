# Ealain

Camera Simulation Tool to Generate Instances for Multiple Classes of Optimisation Problem

## Compilation

In order to compile the project, run the following command: 
```
mkdir build && cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make
```
Example codes and how to run them can be found in the classes of optimisation section.

## Instance scenarios
Instances are defined as rectangles.
These rectangles can be filled by walls.
The walls will block the line of sight of cameras and a camera on top of a wall does not sense anything.
Walls are generated using a csv file where the first column represents the **x** coordinate while the second column represent the **y** coordinate.

Pixels in an instance are assigned a probability of detection.
We consider a pixel covered when the probability of detection is greater than a predefined threshold.
This threshold is left to be defined by the user.


## Ealain modules
### Camera models
Camera models define how pixels are sensed in the instance.
Ealain provides two built-in two-dimensional camera models:
- a perfect camera that can sense anything with a probability of 1 at 360 degrees on a range *r*;
- a camera that can sense at 360 degrees with a detection probability decreasing with the range *r*.

More models can be implemented.

### Camera groups
Camera groups aggregate cameras and their detection into a group.
The aggregation type will affect how the probability of detection of the group will be computed.
For example:
- **Min**, if two cameras detect the same pixel, the minimum probability of detection will be assigned to the group;
- **AtLeastone** will compute the probability of having at least one detection on a pixel assuming independence of camera detections.

More groups can be implemented.

### Cost computation
The cost computation of an instance will give a value of the current evaluated solution.
Ealain comes with a built-in cost computation which is the **coverage**.
Given camera types and locations, the coverage will output the number of pixels covered by the current solution.
The coverage can then be used to define optimisation functions, for example:
- minimise the number of not covered pixels;
- minimise the number of cameras while reaching a coverage of X%.

More cost computation modules can be implemented.

### Constraints
Constraints can be added to any instance scenario.
Implemented constraints are polygon and location-based, i.e., they define a polygon in the instance.
The constraint module outputs the information for each camera on whether a camera is in a polygon and the distance between the camera and the polygon.
This information can be used to create different types of constraints such as:
- Constraints Out, i.e., no camera should be in this polygon at the end of the optimisation process;
- Constraints In, i.e., at least one camera should be in this polygon at the end of the optimisation process.

The constraint handling is left to the user.
Other constraint types can be implemented.

### Solution representation
The localisation of the cameras in the instance can be represented in two ways:
- a numerical way where coordinates are normalised between 0 and 1;
- a discrete way represented by an array of the length of the number of pixels composed of 0 and 1 where 1 represents the location of a camera.

## Classes of optimisation that can be generated
Ealain can generate 6 classes of optimisation problems.
For the classes listed below, the example codes are provided for a squared instance of 50 pixels and two cameras where the objective is to minimise the number of pixels not covered.
The encoding used is numerical but it works similarly for a discrete encoding.

### Single objective optimisation
Single objective functions is the simplest class of instances that can be generated.
Given the definition of an instance, of cameras, camera groups, and cost, the code outputs a single value to optimise.
```
./example_so 50 2 x0 y0 x1 y1
```

### Multi-objective optimisation
Multi-objective functions can be generated in different ways by adding a cost to cameras.
This cost is the only difference with single objective instances in Ealain.
The way the cost of a camera is computed is left to the user.
In our example, a cost is added to the type of camera used in the code.
The example below launches the computation one one camera of each type and outputs the fitness and the cost of the solution.
```
./example_mo 50 1 1 xO yO xB yB
```

### Constrained optimisation
Constrained optimisation can be merged with other classes of optimisation problems.
For example, we add a constraint to a single objective problem by generating a forbidden square in the instance.
This square is defined in the code by its extreme points forming the edges.
The code is used in a similar manner as the single objective one and outputs the value of the fitness accounting for the constraint handling:
```
./example_co 50 2 x0 y0 x1 y1
```

### Multi-fidelity optimisation
Ealain can be used to generate the same problem instances with different levels of granularity.
The level of granularity impacts both the quality of the solution and the computation time.
A finer discretisation will be more precise but computationally expensive.
These instances can be generated by using the discretisation parameter that will influence the number of pixels in the instance.
The example uses a discretisation equal to the size of the instance s=1000 and a smaller discretisation of s/2.
```
./example_mf 1000 2 x0 y0 x1 y1
```
The output is the ratio of the number of pixels not covered by the total number of pixels for each discretisation and the associated computation time.


### Drift in optimisation
Ealain can be used to generate a stream of instances that exhibit data-drift or have a dynamically changing fitness landscape.
Both types of drift can be generated in the same way, i.e., using Constraints In.
By adding constraints in the instance, it is possible to have control on the drift.
The same code can be used for data-drift or dynamic optimisation: data-drift is obtained by modifying each instance while dynamic optimisation is obtained by changing the function during the run.
The code can be launched by specifying the size of the instance, the number of constraints and camera localisations.
```
./example_drift_num 50 2 1 x0 y0 x1 y1
```



Further information can be found in the GECCO poster "Ealain: A Camera Simulation Tool to Generate Instances for
Multiple Classes of Optimisation Problem", Quentin Renau, Johann Dreo, and Emma Hart
