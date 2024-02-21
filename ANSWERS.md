
# Answer Document

##### Andrew ID: srameshs

## Part 1: Practice Problems (15%)
### Question 1


Given a pointer to struct D called “d”, what expression will access “data”, the member of struct A? You may assume that all pointers are initialized and are valid.

```cpp
struct A {
    float data;
};

struct B {
    A a;    
};

struct C {
    B* b;
};

struct D {
    C c;
};
```

#### Answer
To access the member data of struct A given a pointer to struct D called d, you would use the following expression:
```cpp
d->c.b->a.data;
```

### Question 2


Given the following structs, predict the memory layout of a struct A named “stack”. You may assume that it is a 32-bit machine.


```cpp
struct A{
    float a, b, c;
    B* ptr;
    B mem;
};

struct B {
    float d, e;
};

A stack;
```


| Address | Expressions
| --- | --- |
| A+0x0 | stack, stack.a |
| A+0x | |
| A+0x | | 
| A+0xC | |
| A+0x | |
| A+0x | |


#### Answer


| Address | Expressions
| --- | --- |
| A+0x0 | stack, stack.a |
| A+0x4 | stack.b |
| A+0x8 | stack.c | 
| A+0xC | stack.ptr |
| A+0x10 | stack.mem.d |
| A+0x14 | stack.mem.e |


### Question 3

Given the functions below, what is the value stored  in the points a and b after calling foo?

```cpp
struct point {
    float x, y;
};

void foo(point& p1, point p2) {
    point p3;
    p3.x = p1.x + p2.x;
    p3.y = p1.y + p2.y;
    bar(p3);
    p1.x -= p3.x;
    p1.y -= p3.y;
}

void bar(point p3) {
    p3.x *= .5f;
    p3.y *= .5f;
}

point a, b;
a.x = b.y = 1;
a.y = b.x = 0;
foo(a, b);
```

#### Answer


After calling the function foo(a, b), the value stored in the points a and b is:
```cpp
a.x = 0
a.y = -1
b.x = 0
b.y = 1
```

## Part 2: Coding Questions

### Question 1 


Write an overview of how your AI works, including how it detects where projectiles will fall and how it chooses where to go.

#### Answer

- The AI employs projectile motion equations to predict where projectiles will fall, allowing it to identify safe and dangerous areas on the ground
- By determining safe spots and calculating the nearest safe spot for each player, the AI guides player movement to avoid danger zones and survive within the game environment

**Control Function (control()):**
- It first clears existing safe spots and recalculates them using determineSafeSpots()
- For each player, it calculates the nearest safe spot based on the player's current position and the list of safe spots
- It adjusts the player's speed based on the distance to the nearest safe spot, ensuring that players move efficiently while avoiding danger zones
- The movement direction is determined based on the relative position of the player to the nearest safe spot. For example, if the safe spot is to the right of the player, the player moves towards the right; otherwise, it moves towards the left

**Track Projectile Function (trackProjectile()):**
- It utilizes projectile motion equations to calculate the time of impact, considering parameters such as the projectile's initial position, velocity, and gravitational constant
- Specifically, it solves for the roots of the quadratic equation derived from the projectile motion equations to determine when the projectile will hit the ground
- If the discriminant of the quadratic equation is non-negative, it calculates the roots and chooses the positive root, representing the future impact time. Otherwise, it indicates no impact
- The function returns a Prediction struct containing the time (t) and position (x) of the predicted impact

**Determining Safe Spots (determineSafeSpots()):**
- It iterates through the ground positions and predicts the impact of projectiles at each position using the trackProjectile() function
- If a ground position is within the explosion range of a projectile and the predicted impact time is within a safe threshold (here, 5 seconds), the position is marked as unsafe
- Safe spots are positions on the ground that are not within the explosion range of any projectile and have a safe predicted impact time


### Question 2

What challenges did you face when writing an AI?
#### Answer

I had initially faced the challenge where the player just moved only to the left in g.playerSpeed(). This was because, in my control function, I was only evaluating `safeSpot > playerPtr->x` and `safeSpot < playerPtr->x` to calculate the direction in which the player should move. I was not considering the case where the current player position could be a safe spot, resulting in the direction being equal to zero, allowing it to not move anywhere. But, by not including that case, the player was constantly moving to the left as the `safeSpot` even though if equal, it was being considered less than the current player position.


Another issue I had faced was that the player was moving, but was not moving enough. So, even if it moved to avoid the projectile, it was still destroyed because of the size of the explosion. I solved this issue in the `determineSafeSpots()` function by also evaluating that the `distance>=0`. [distance -> distance between the predicted impact point and the current ground position]. Previously, I was only evaluating this: `distance <= g.explosionSize && pred.t <= 5`.


The final issue I encountered was making the player move faster to the safe spot. this was adjusted by modifying the `playerSpeed: float playerSpeed = std::min(maxSpeed, distanceToSafeSpot / t);` which calculates the player's speed based on the distance to the nearest safe spot and the time interval between control updates, ensuring that the player moves towards safety effectively and without exceeding the maximum allowed speed.

### Question 3

How well does your AI work on a Hard scenario? If it doesn’t work, why? If it does, try harder scenarios and see when it does fail and explain why?


#### Answer
My AI is not working in the Hard scenario, the player once spawned is not tracking the projectile from the second enemy on the left. In the hard level, the AI's struggle stems from its inability to accurately determine safe locations to move, mainly due to limitations in the `determineSafeSpots` function. This function doesn't effectively consider the faster and more random enemy shots (specified by `e.firingSpeed = 3e-2f` and `e.firingRandomness = .8f`). Consequently, the AI fails to react properly to incoming threats, particularly those from the second enemy, resulting in frequent player deaths caused by enemy projectiles.deaths.

### Question 4

What did you think of the assignment and did it meet its goals? Why or why not?


#### Answer
This assignment was a good way to get more acclimated with concepts from C++. Personally, as someone with experience in C++, this assignment was a good platform for me to refresh up on concepts that I had worked on. It was also a learning curve for me to understand how the Game interface works and how that interacts with the Controller code. 

I can find myself more in sync with concepts such as object-oriented programming principles, including encapsulation, inheritance, and polymorphism, as well as the use of standard library containers and algorithms. Additionally, working with the Game interface helped me understand how to design and interact with complex systems, especially in the context of game development or simulations. Overall, the assignment provided a valuable opportunity to reinforce my C++ skills and gain practical experience in software development.