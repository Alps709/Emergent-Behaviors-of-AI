# Emergent-Behaviors-of-AI
This program implements the famous bird flocking behaviours first created by Craig Reynolds.
https://en.wikipedia.org/wiki/Boids

It is created using C++ and OpenGL to render the boids in a window.

# Controls:

1 - Seek

2 - Arrive

3 - Flocking

4 - (Crowd) Path Following

c - Containment


# Implemented behaviours:

### Simple behaviours:

Seek

Arrive

Containment

Path following

### Complex:

Flocking

Group path following


# How each behaviour was implemented:

## Seek:
Find the vector from the boid position to the target position by subtracting the boid position vector from the target position vector.
This gives you the desired directional vector to the target. You can then find the vector for the steering force by substracting the boid velocity from the desired direction vector.
You then apply the directional steering force vector to the acceleration vector of the boid, so that it accelerates towards the target.
The new acceleration vector of the boid must be recalculated starting from 0, each update.

## Arrive:
Done the same as Seek, except it limits the magnitude of the desired vector based on the boid's distance from the target. 
It then subtracts the boid velocity from the desired vector to get the steering force in the same way as Seek. 
So if the desired vector is limited to a magnitude of 0.1, then a current velocity of 1 is subtracted, 
the resulting vector has a magnitude of 0.9 in the opposite direction as a counter force. (In this example the directions of the two vectors are the same).
So that this resulting steering force counters the current velocity the closer it gets to the target position, eventually reaching a velocity of 0, when it reaches the target.
The boid also has a max force limit, so if the incoming velocity is too high, 
the counter force may be limited to a point where it is not strong enough to counter the velocity in time, and it will over shoot the target before coming back around and stopping.

## Containment:
For containment I check whether each axis position of the boid is outside of the container, and if it is, 
I then set the position on that axis back to the edge of the container, and then reverse the boids velocity on that axis.

## Path following:
It finds the next predicted position of the boid and then finds the closest normal to that predicted position, from the multiple path segments.
Once it finds the closest normal to any of the path segments, it then finds a position further along the path from that normal to seek towards. 
We want to Seek a bit ahead of the normal so that if the boid is perpendicular to the path, it will turn to move along the path rather than going back and forward over one spot on it.

## Flocking:
Flocking uses 3 combined behaviours.

1. Alignment: This is done by getting the velocities of all other boids within the current boids alignment view radius, 
and then summing all those velocities together into a vector, then dividing that vector by the number of boids that it used. 
This gives you an average velocity of all those boids within the view radius, which you then find a steering force for by subtracting your current velocity from this new average velocity.

2. Separation: This is done by finding all position vectors of boids within the view radius, that point from each of those boids to the current boid.
So for each other boid that the current boid can see, you subtract the other boids position from the current one, this gives you a vector pointing from the other boid to the current boid.
You do this for each other boid the current one can see, and you sum them all together. 
You then divide that summed vector by the amount of boids it could see, and this gives you the average of all vectors pointing from the other boids to the current one.
If you then subtract the velocity of the current boid, from the averaged velocity, then it will give you a steering force pointing the current boid away from all the other boids.

3. Cohesion: This is done by doing the exact same as separation, but then reversing the resulting vector, so it then gives you the steering force towards the average of all the other boids.
This is balanced against separation by having a different larger view radius specifically for cohesion.

These three behaviours combined result in a flocking behaviour.


