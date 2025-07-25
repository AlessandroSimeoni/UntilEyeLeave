# Until Eye Leave
Venture in an unknown world to find your way home, beware of the strange creatures that populate this place and learn how to walk among them to solve puzzles.

The player is a cat that is unfortunately passing on to a better life, we, as a player, experience her journey in the afterlife inside her conscience.

Until Eye Leave is a puzzle action-adventure vertical slice developed in Unreal Engine 5.4 for PC.

This is the last, biggest and most important project of the academic studies.
It was developed during 6 months in a pretty big team of approximately 20 people (10 designers, 3 programmers, 4 concept artists and 3 3D artists).
This project has allowed me to learn different features of Unreal Engine, organize the work among us programmers visualizing it in a Gantt view on github, create technical documentation of the code and communicate effectively between departments in a larger than usual team.

### ***You can download the build [here](https://github.com/AlessandroSimeoni/UntilEyeLeave/releases/tag/ReleaseBuild).***

<a name="What-I-did"></a>
# What I did
For this project, I contributed to several core systems.  
I developed a custom finite state machine (FSM) and designed almost all the main character logic around it.  
I also worked on gameplay objects, the save system, the cutscene system, game settings, level loading logic, and various other supporting features.
> 1. [FSM](#FSM)
> 2. [Character](#Character)
> 3. [Gameplay objects](#Gameplay-objects)
> 4. [Save system](#Save-system)
> 5. [Game settings](#Game-settings)
> 6. [Cutscene system](#Cutscene-system)
> 8. [Level loading](#Level-loading)

<a name="FSM"></a>
## 1. FSM
The project contains a custom finite state machine consisting of two basic scripts.  
You can find them here: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Public/FSM) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Private/FSM).  
There is a StateBase and a StateController.

<a name="State-Base"></a>
### State Base
It's the base class of any state of the FSM.  

Each state has 5 public methods:
* **InitState**: called by the *StateController* to initialize the state variables;
* **EnterState**: called by the *StateController* when entering the state;
* **TickState**: called every frame by the *StateController* if the state can tick;
* **ExitState**: called by the *StateController* when exiting the state;
* **IsStateReady**: this returns the *StateReady* protected boolean;

The class also contains a **protected** method called **StateTransition**; this is used to request a change of state to the *StateController*, passing a *TargetState* as parameter (each state must cache references to the states which it is connected to).  

As for parameters, the class contains:
* **CanTick**: a bool which controls the *TickState* method, if **TRUE** the state can tick, otherwise no;
* **StateController**: each state has a reference to the *StateController* and it is initialized in the *InitState* method;
* **StateReady**: a bool used to know if the state is ready to be entered or not; the *StateController* checks this value when it receives a request to change state and, if the target state is not ready, the state transition will fail (this is useful for states with a cooldown effect).

### State Controller
The state controller is the brain of the FSM.  
It is initialized at the *BeginPlay* with the **Init** method.  

What the **Init** method does is to create and initialize every state that must be controlled.  
The states under its control are listed in the *AvailableStateClasses* array of `TSubclassOf<class UStateBase>` which is populated in the unreal editor (the first state of this array will be the default state).  
The *Init* method creates `UStateBase` objects based on the classes in the array and saves them in the *States* array (which is a `TArray<UStateBase*>`); each state then is initialized by calling the state’s *InitState* method.  
Finally, the first state in the *AvailableStateClasses* array is entered (this is the default state).  
The core method of the state controller is the **ChangeState** method.  
It accepts one input parameter, the *TargetState*.  
It ensures that the *TargetState* is valid and ready before performing the transition.  
When changing state the *CurrentState* variable is updated together with the *PreviousState* and the *NextState* variables (these two are useful so that the states know which state they are coming from or to which they need to make the transition).  
If *TargetState* is the same as *CurrentState* the **ChangeState** method returns false to prevent unnecessary state changes.  
If the transition can occur, the method calls the *ExitState* of the *CurrentState* and the *EnterState* of the *TargetState* and updates the variables.  

Another important method is the **FindStateOfClass**.  
This is used by the states to get a reference to the state object they request (the method accepts a `TSubclassOf<UStateBase>` as input parameter), so that they can cache references to the states they are connected to.  

The *StateController* is an actor component and to tick the *TickComponent* method is used.  
This method will call the *TickState* of the *CurrentState* if the *StateController* can tick (*CanTickState* variable) and if the *CurrentState* can also tick (*CanTick* variable of the state).  

**[⬆ Back to Top](#What-I-did)**

<a name="Character"></a>
## 2. Character
You can find the scripts related to the character here: [.h](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Public/Character) / [.cpp](https://github.com/AlessandroSimeoni/UntilEyeLeave/tree/main/Source/TBO_01/Private/Character).  
The scripts include a *CharacterController*, a *CharacterAnimInstance* and of course the *MainCharacter* script along with all the states scripts.  
The character was one of the first elements developed to allow the level designers to test their levels as early as possible.  
When I created the character I didn't know about the Unreal's Gameplay Ability System (GAS), and I had already prototyped the [FSM](#FSM), so since I had already tested it and didn't have enough time to quickly learn GAS, I decided to continue using it.  
Despite this, the character still incorporates functionalities built on top of GAS, added at a later time by a colleague who also implemented all the creatures in the project using the same system.  
If I had to recreate the character today, I would definitely use GAS thanks to its high flexibility.  

### Main scripts
The *MainCharacter* script handles the interaction logic, the change of the rotation rate based on the device in use (gamepad/keyboard), the ledge grab detection logic and the life system of the player.  
The *CharacterController* script has the core logic and delegates to handle the user inputs.  
The *CharacterAnimInstance* in the *NativeUpdateAnimation* method, the *IdleToRunRatio* parameter which handles the blend from idle to run animation.  

### Character FSM
The FSM is composed of *PlayerStateBase* states which derive from the [StateBase](#State-Base).  
The main difference is that it has references to the *MainCharacter*, the *CharacterController* and the *CharacterAnimInstance*.  

There are a total of 10 scripts for the states:
* **PlayerIdleState**
* **PlayerStandardMoveState**
* **PlayerSlowMoveState**
* **PlayerMidAirState**
* **PlayerLedgeFallingState**
* **PlayerJumpState**
* **PlayerLedgeGrabState**
* **PlayerRollState**
* **PlayerKnockbackState**
* **PlayerCombatState**

Each state has references to the states they are directly connected to. These references are initialized in the *InitState* method.  
As a general rule the *EnterState/ExitState* method is used to subscribe/unsubscribe to the events of the *MainCharacter* and/or the *CharacterController* so that they know how to behave and towards which state call the transition.  
Let's take a quick look at them anyway.  

### Idle State
Pretty simple, the character is in idle animation and the state is listening for the events of movement, attack, jump, roll or knockback if the player is hit by something.

### Standard Move State
The character moves at a specified speed based on the input direction.  
The character rotates with a rotation rate which is based on the device in use (faster control with a gamepad, slower one with the keyboard).  
There is also a logic that handles an abrupt change in the direction (ie: the character is running left but suddenly the user presses the right input direction); in this case the player rotates towards the direction and stops for a brief moment before moving again.  
All this is done in the *AddMovementDirection* method.
In the *TickState* method there are some checks to handle cases such as: falling, stop moving, slow movement or the ledge grab of low steps.

### Slow Move State
It’s the same as the standard one but has a different movement speed.

### Mid Air State
This is the base state for all the mid air states (jump and falling).  
Pretty simple, it has checks to go in the roll, idle or ledge grab states; it also handles the mid air movement which is limited compared to the *StandardMoveState*.

### Ledge Falling State
A mid air state which handles the coyote time to simplify jumps from ledges.

### Jump State
Makes the character jump based on the *Jump* method of Unreal.  
Handles the forward and horizontal movement of the player separately.

### Ledge Grab State
Handles the ledge grab in air and on the ground.  
The core logic is made in blueprint because it was faster to test it, but it can be easily replicated in C++.

### Roll State
The player can roll and he can do it in the direction the user puts in input (instant rotation of the player when entering the state).  
The roll can be interrupted with a jump.  
There is a logic in the *TryEndRolling* method which handles the case where the player is under some object while rolling. This is done to avoid getting stuck in things.  
The logic is kinda simple, it casts a sphere above the player to know if there is something and as long as it returns true, the player remains in this state and it is forcefully moved with a certain speed in a calculated direction.  
There is also a logic that handles the case where it encounters a wall while rolling under an object.  
The logic is not perfect since there are rare cases where the player can go out of map if the colliders are placed in a messy way.

### Knockback State
The knockback was made with GAS by a collegue.
When entering this state the ability is activated from the blueprint version of this state.

### Combat State
Player can attack in mid air and on ground.
The attack combo and animations were handled with GAS in blueprint by a collegue.
The combat state can be interrupted with a roll.


**[⬆ Back to Top](#What-I-did)**

<a name="Gameplay-objects"></a>
## 3. Gameplay objects

**[⬆ Back to Top](#What-I-did)**

<a name="Save-system"></a>
## 4. Save system

**[⬆ Back to Top](#What-I-did)**

<a name="Game-settings"></a>
## 5. Game settings

**[⬆ Back to Top](#What-I-did)**

<a name="Cutscene-system"></a>
## 6. Cutscene system

**[⬆ Back to Top](#What-I-did)**

<a name="Level-loading"></a>
## 7. Level loading

**[⬆ Back to Top](#What-I-did)**
