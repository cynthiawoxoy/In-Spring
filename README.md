# Game Engine Programming Project Proposal

Write the proposal for your project that adheres to the [guidelines in the syllabus](https://docs.google.com/spreadsheets/d/16_N4rq2cUvFVjczFWrn0veSouctNHb8Fo4ANjF5FmC8/edit?usp=sharing). 

### Grading
As stated in the syllabus, your proposal is worth 10% of your project grade.

### Guidance

Remember that you have some flexibility in whether to focus on the Game or the Engine, but you must have at least 5 complexity points for both and a total of 20. You may fork the class game (the shmup), but to get any game points you must change or improve it.

**Engine Tasks**

To assist with task engine task selection and scoring, see this [table of canditate engine tasks](https://docs.google.com/spreadsheets/d/16_N4rq2cUvFVjczFWrn0veSouctNHb8Fo4ANjF5FmC8/edit?usp=sharing). The table may include tasks that your classmates have proposed, and those are fair game.

***Important:*** If you are planning an engine task that is not in the table above, you must ask about it in the [#project-questions](https://discord.com/channels/1461480080130703628/1461485336306909326) channel on Discord. Within 48 hours, the instructor will
- approve the task and assign a point value or
- suggest changes and assign a point value or
- reject the task

**Game Tasks** 

Game task are more open ended. Estimate game task complexity by taking the number of hours you think it will take to implement and dividing it by 2. The instructor will adjust your estimates when providing feedback. This adjustment is for normalization to ensure fairness when grading. It does not mean your estimate was "wrong" (although software estimates usually *are* wrong).

It's a good idea to list more points than you need for this so that if your points are adjusted down you don't receive a grade for insufficient complexity. You don't have to implement everything you propose here!

***Your proposal must follow this format:***

# [ In Spring ]
*Xinyi(Cynthia) Wang*

## Description

*Player will control an animal jumping through the world, using its tail to trigger specific switches that cause according flower to bloom. Game ends when all the flowers in the world are in blossom. If chain physics is possible, the tail naturally follows the player's movements and swing through space. To add some difficulty to the game, only specific part of player's long tail can activate matched switches(use color of patterns to indicate the match). Collision detection with quad tree improvement will be performed in the process.*

## Game Tasks
*Complexity Point Subtotal: [8]*

#### Task 1 - charged jump [1] Complexity Points

*Different from a normal jump, player can do a charged jump for larger velocity or acceleration.*

*Charged jump affects the velocity or acceleration of player, therefore, affects area the player's tail could sweep through.*

#### Task 2 - Draw Flowers [1-3] Complexity Points

*Use individual petal and leaf images to form flowers. Set plant components to according locations. It might take longer than 2hrs if aiming for flowers' aesthetic lookings.*

#### Task 3.1 - Rotation Animation [1] Complexity Points

*Allow entities to rotate an angle.*

*Flower petals will use rotation animation when they bloom.*

#### Task 3.2 - Rotation Animation, advanced version [2] Complexity Points

*For different flowers or plants, using combination of rotations to control subpart of plants, so that plants of different species each have customized animations of blossoms. This function with Rotation Animation in 2.1 together form 3 points*

*Flower petals will use rotation animation when they bloom.*

#### Task 4 - Radiative Particle Animation [1] Complexity Points

*Many particles will be emitted through one point towards all directions and gradually fade away*

*Animation for blooming flowers.*

#### Task 5 - Radial color Animation [2] Complexity Points

*In a masked area, color of the world gradually changes in radial effects.*

*Flower petals will use radial color animation when they bloom.*

#### Other alternatives:

*Hitpoints, reduces if player falls out of the world.*

## Engine Tasks
*Complexity Point Subtotal: [15]*

#### Task 1 - Quad Tree [9] Complexity Points

*Use Quad Tree to improve collision detection efficiency.*

*The player use its long tail to collide with switches to bloom flowers. There will be collision detection for tail parts and switches.*

#### Task 2.1 - Chain Physics [6] Complexity Points

*Two or more "entities" are chained to each other and their movements are being influenced by their connections besides the general effects from velocity and gravity.*

*Chain physics can be used to create a good animation performance for player's long tail.*

#### Task 2.2 - Platform System [6] Complexity Points (alternative for 2.1)

*Platforms that can move and allow players to latch on it*

*This is an alternative for 2.1 if Chain Physics is hard to implement. In this case, player will jump between platforms to reach for switches, and their body, instead of their tail, touch and activate switches.*

#### Alternative tasks in case task 1 or task 2 will not be implemented: 

#### Task 1 - Text dialogue popup [3] Complexity Points

*Text dialogue popup. The dialogue will change after mouse clicking it.*

*Conversation between player and the flowers could perform as a guide of gameplay or game narrative.*

#### Others
*one of memory allocator, sound emitter, audio improvements, wobble animation, debugger outputs, since they are always useful.*
